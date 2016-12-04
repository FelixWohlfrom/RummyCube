/*
 * HumanPlayerServer.cpp
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#include "HumanPlayerServer.h"

#include "../stones/StoneManager.h"
#include "../../network/Network.h"

HumanPlayerServer::HumanPlayerServer(StoneManager& stoneManager, int stonesAtBeginning,
        int sumAtBeginning, bool stonesInOneRow, std::list<QTcpSocket*> otherPlayerSocks) :
    HumanPlayer(stoneManager, stonesAtBeginning, sumAtBeginning, stonesInOneRow),
    otherPlayerSocks(otherPlayerSocks)
{}

HumanPlayerServer::~HumanPlayerServer() {}

void HumanPlayerServer::roundFinished()
{
    HumanPlayer::roundFinished();

    // Broadcast current game board status. Don't send all stones so no one
    // knows the stones of the other players
    for (std::list<QTcpSocket*>::iterator
            socket(otherPlayerSocks.begin());
            socket != otherPlayerSocks.end();
            ++socket)
    {
        Network::writeBoardStatus(*(*socket), stoneManager);

        if (this->hasWon())
        {
            Network::write(*(*socket), "playerWon");
        }

        Network::write(*(*socket), "roundFinished");
    }
}
