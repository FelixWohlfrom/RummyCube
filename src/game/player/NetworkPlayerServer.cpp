/*
 * NetworkPlayerServer.cpp
 *
 *  Created on: 14.04.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkPlayerServer.h"
#include "../../network/NetworkGameClosedEvent.h"

BEGIN_EVENT_TABLE(NetworkPlayerServer, wxEvtHandler)
	EVT_SOCKET(Network::SOCKET_EVENT, NetworkPlayerServer::OnSocketEvent)
END_EVENT_TABLE()

NetworkPlayerServer::NetworkPlayerServer(wxString name, StoneManager& stoneManager, wxSocketBase& sock, std::list<wxSocketBase*> allPlayerSocks) :
	NetworkPlayer(name, stoneManager, sock), otherPlayerSocks(allPlayerSocks)
{
	// Delete current socket from list of all players
	std::list<wxSocketBase*>::iterator toDelete;
	for (std::list<wxSocketBase*>::iterator it(otherPlayerSocks.begin()); it != otherPlayerSocks.end(); ++it)
	{
		if ((*it) == &sock)
		{
			toDelete = it;
		}
	}

	otherPlayerSocks.erase(toDelete);

	// Start listening to events
	sock.SetEventHandler(*this, Network::SOCKET_EVENT);
	sock.SetNotify(wxSOCKET_LOST_FLAG);
	sock.Notify(true);
}

NetworkPlayerServer::~NetworkPlayerServer()
{
	sock.Destroy();
}

void NetworkPlayerServer::play()
{
	roundFinished = false;

	while (!roundFinished && !stopPlayerPlaying)
	{
		parseData();
		wxYield();
	}

	if (stopPlayerPlaying) return;

	// Broadcast current game board status. Don't send all stones so no one knows the stones of the other players
	for (std::list<wxSocketBase*>::iterator it(otherPlayerSocks.begin()); it != otherPlayerSocks.end(); ++it)
	{
		Network::writeBoardStatus(*(*it), stoneManager);

		if (playerHasWon)
		{
			Network::write(*(*it), _T("playerWon"));
		}

		Network::write(*(*it), _T("roundFinished"));
	}
}

// Socket handler
void NetworkPlayerServer::OnSocketEvent(wxSocketEvent& e)
{
	wxSocketBase* evt_sock = e.GetSocket();
	wxASSERT(evt_sock == &sock);

	if (e.GetSocketEvent() == wxSOCKET_LOST)
	{
		stopPlayerPlaying = true;
		if (playerHasWon) return;

		// Broadcast to other players that the player disconnected
		for (std::list<wxSocketBase*>::iterator it(otherPlayerSocks.begin()); it != otherPlayerSocks.end(); ++it)
		{
			if ((*it) != evt_sock && (*it)->IsConnected())
			{
				Network::write(*(*it), _T("disconnected"));
				Network::write(*(*it), this->getPlayerName());
			}
		}

		if (handler != NULL)
		{
			NetworkGameClosedEvent e(wxNETWORK_GAME_CLOSED, this->getPlayerName());
			handler->AddPendingEvent(e);
		}
	}
}
