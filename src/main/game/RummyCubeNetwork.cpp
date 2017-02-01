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
 * RummyCubeNetwork.cpp
 *
 *  Created on: 08.01.2013
 *      Author: Felix Wohlfrom
 */

#include "RummyCubeNetwork.h"

#include "player/HumanPlayerServer.h"
#include "player/HumanPlayerClient.h"
#include "player/NetworkPlayerServer.h"
#include "player/NetworkPlayerClient.h"

RummyCubeNetwork::RummyCubeNetwork(NetworkGameSettings& settings,
        std::map<QTcpSocket*, QString> opponentPlayers) :
    RummyCube(), sock(NULL)
{
    // Create the players
    std::list<QTcpSocket*> allSockets;
    for (std::map<QTcpSocket*, QString>::iterator elem(opponentPlayers.begin());
            elem != opponentPlayers.end();
            ++elem)
    {
        allSockets.push_back(elem->first);
    }

    // Human player
    this->humanPlayer = new HumanPlayerServer(stoneManager, settings.stonesAtBeginning,
            settings.sumAtBeginning, settings.stonesInOneRow, allSockets);

    // Opponent players
    this->opponentPlayers.clear();
    for (std::map<QTcpSocket*, QString>::iterator elem(opponentPlayers.begin());
            elem != opponentPlayers.end();
            ++elem)
    {
        this->opponentPlayers.push_back(
                new NetworkPlayerServer(elem->second, stoneManager, *elem->first, allSockets));
    }
}

RummyCubeNetwork::RummyCubeNetwork(NetworkGameSettings& settings, QTcpSocket* sock,
        QString humanPlayer, std::list<QString> opponentPlayerNames) :
    RummyCube(), sock(sock)
{
    // Create human player
    this->humanPlayer = new HumanPlayerClient(stoneManager, *sock, settings.stonesAtBeginning,
            settings.sumAtBeginning, settings.stonesInOneRow);

    // Create opponent players
    // Needed for the step after, list resorting
    std::deque<OpponentPlayer*>::iterator lastElemBeforeCurrPlayer;
    bool skip = false;
    this->opponentPlayers.clear();
    for (std::list<QString>::iterator name(opponentPlayerNames.begin());
            name != opponentPlayerNames.end();
            ++name)
    {
        if ((*name) == humanPlayer)
        {
            skip = true;
            lastElemBeforeCurrPlayer = this->opponentPlayers.end() - 1;
            continue;
        }

        this->opponentPlayers.push_back(
                new NetworkPlayerClient((*name), stoneManager, *sock, settings.stonesAtBeginning,
                        settings.sumAtBeginning, settings.stonesInOneRow, skip));
    }

    // Move opponentPlayers that are after current player to the beginning of list
    while (opponentPlayers.back() != *lastElemBeforeCurrPlayer)
    {
        this->opponentPlayers.push_front(opponentPlayers.back());
        this->opponentPlayers.pop_back();
    }
}

RummyCubeNetwork::~RummyCubeNetwork()
{
    if (sock != NULL)
    {
        if (sock->state() == QTcpSocket::ConnectedState)
        {
            sock->disconnect();
        }
        delete sock;
    }
}
