/*
 * HumanPlayerClient.cpp
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#include "HumanPlayerClient.h"

#include "../../network/Network.h"
#include "../stones/StoneManager.h"

#include <wx/datstrm.h>
#include <wx/sckstrm.h>

HumanPlayerClient::HumanPlayerClient(StoneManager& stoneManager, wxSocketBase& sock, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow) :
	HumanPlayer(stoneManager, initialStonesLeftToTake, sumInFirstRound, stonesInOneRow), sock(sock) {};

HumanPlayerClient::~HumanPlayerClient() {};

void HumanPlayerClient::roundFinished()
{
	HumanPlayer::roundFinished();

	Network::writeBoardStatus(sock, stoneManager);

	if (this->hasWon())
	{
		Network::write(sock, _T("playerWon"));
	}

	Network::write(sock, _T("roundFinished"));
}
