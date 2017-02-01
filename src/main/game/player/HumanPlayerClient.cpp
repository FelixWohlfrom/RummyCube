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
