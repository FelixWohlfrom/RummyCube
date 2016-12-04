/*
 * HumanPlayerClient.cpp
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#include "HumanPlayerClient.h"

#include "../../network/Network.h"
#include "../stones/StoneManager.h"

HumanPlayerClient::HumanPlayerClient(StoneManager& stoneManager, QTcpSocket& sock,
        int stonesAtBeginning, int sumAtBeginning, bool stonesInOneRow) :
    HumanPlayer(stoneManager, stonesAtBeginning, sumAtBeginning, stonesInOneRow), sock(sock),
    isFirstRound(true)
{}

HumanPlayerClient::~HumanPlayerClient() {}

void HumanPlayerClient::newRound()
{
    if (isFirstRound)
    {
        isFirstRound = false;
    }
    else
    {
        HumanPlayer::newRound();
    }
}

void HumanPlayerClient::roundFinished()
{
    HumanPlayer::roundFinished();

    Network::writeBoardStatus(sock, stoneManager);

    if (this->hasWon())
    {
        Network::write(sock, "playerWon");
    }

    Network::write(sock, "roundFinished");
}
