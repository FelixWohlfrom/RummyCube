/*
 * HumanPlayerServer.cpp
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#include "HumanPlayerServer.h"

#include "../stones/StoneManager.h"
#include "../../network/Network.h"

#include <wx/datstrm.h>
#include <wx/sckstrm.h>

HumanPlayerServer::HumanPlayerServer(StoneManager& stoneManager, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow, std::list<wxSocketBase*> otherPlayerSocks) :
	HumanPlayer(stoneManager, initialStonesLeftToTake, sumInFirstRound, stonesInOneRow), otherPlayerSocks(otherPlayerSocks) {};

HumanPlayerServer::~HumanPlayerServer() {};

void HumanPlayerServer::roundFinished()
{
	HumanPlayer::roundFinished();

	// Broadcast current game board status. Don't send all stones so no one knows the stones of the other players
	for (std::list<wxSocketBase*>::iterator it(otherPlayerSocks.begin()); it != otherPlayerSocks.end(); ++it)
	{
		Network::writeBoardStatus(*(*it), stoneManager);

		if (this->hasWon())
		{
			Network::write(*(*it), _T("playerWon"));
		}

		Network::write(*(*it), _T("roundFinished"));
	}
}
