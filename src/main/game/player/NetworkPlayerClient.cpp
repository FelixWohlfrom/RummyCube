/*
 * NetworkPlayerClient.cpp
 *
 *  Created on: 09.04.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkPlayerClient.h"

#include "../../network/Network.h"
#include "../../network/NetworkGameClosedEvent.h"

NetworkPlayerClient::NetworkPlayerClient(wxString name, StoneManager& stoneManager, wxSocketBase& sock, NetworkEventHandlerClient& eventHandler, bool skipAfterCreation) :
	NetworkPlayer(name, stoneManager, sock), skipAfterCreation(skipAfterCreation)
{
	eventHandler.addEventReceiver(this);
	handler = &eventHandler;
}

NetworkPlayerClient::~NetworkPlayerClient() {};

void NetworkPlayerClient::play()
{
	// Skip if variable is set
	if (skipAfterCreation)
	{
		skipAfterCreation = false;
		return;
	}

	roundFinished = false;

	while (!roundFinished && !stopPlayerPlaying)
	{
		parseData();
		wxYield();
	}
}

// Socket handler
void NetworkPlayerClient::OnSocketEvent(wxSocketEvent& e)
{
	if (e.GetSocketEvent() == wxSOCKET_LOST)
	{
		stopPlayerPlaying = true;
		if (playerHasWon) return;

		if (handler != NULL)
		{
			NetworkGameClosedEvent e(wxNETWORK_GAME_CLOSED, this->getPlayerName());
			handler->AddPendingEvent(e);
		}
	}
}
