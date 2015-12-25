/*
 * NetworkPlayer.cpp
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkPlayer.h"
#include "../../network/NetworkGameClosedEvent.h"

NetworkPlayer::NetworkPlayer(wxString name, StoneManager& stoneManager, wxSocketBase& sock) :
	OpponentPlayer(name, stoneManager, 0, 0, false), playerHasWon(false), roundFinished(false),
	sock(sock), handler(NULL)
{};

NetworkPlayer::~NetworkPlayer() {};

bool NetworkPlayer::hasWon()
{
	return playerHasWon;
}

void NetworkPlayer::setCloseEventHandler(wxEvtHandler* newHandler)
{
	this->handler = newHandler;
}

void NetworkPlayer::parseData()
{
	if (!sock.WaitForRead(0, 100))
	{
		wxMilliSleep(100);
		return;
	}

	wxString msg = Network::read(sock);

	if (msg == _T("boardStatus"))
	{
		Network::readBoardStatus(sock, stoneManager);
	}
	else if (msg == _T("disconnected"))
	{
		wxString playerName = Network::read(sock);
		if (handler != NULL)
		{
			NetworkGameClosedEvent e(wxNETWORK_GAME_CLOSED, playerName);
			handler->ProcessEvent(e);
		}

		stopPlayerPlaying = true;
	}
	else if (msg == _T("playerWon"))
	{
		playerHasWon = true;
	}
	else if (msg == _T("roundFinished"))
	{
		roundFinished = true;
	}
}
