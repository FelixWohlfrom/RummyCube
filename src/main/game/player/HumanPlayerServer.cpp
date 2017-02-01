/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2016 Felix Wohlfrom                                    *
 *                                                                       *
 *  This program is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
