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
 * NetworkPlayerServer.cpp
 *
 *  Created on: 14.04.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkPlayerServer.h"

#include <QThread>
#include <QCoreApplication>

NetworkPlayerServer::NetworkPlayerServer(QString name, StoneManager& stoneManager, QTcpSocket& sock,
        std::list<QTcpSocket*> allPlayerSocks) :
    NetworkPlayer(name, stoneManager, sock, 0, 0, false), otherPlayerSocks(allPlayerSocks)
{
    // Delete current socket from list of all players
    std::list<QTcpSocket*>::iterator toDelete;
    for (std::list<QTcpSocket*>::iterator
            socket(otherPlayerSocks.begin());
            socket != otherPlayerSocks.end();
            ++socket)
    {
        if ((*socket) == &sock)
        {
            toDelete = socket;
            break;
        }
    }

    otherPlayerSocks.erase(toDelete);

    connect(&sock, SIGNAL(disconnected()),
             this, SLOT(opponentDisconnected()));
}

void NetworkPlayerServer::play()
{
    roundFinished = false;

    while (!roundFinished && !stopPlayerPlaying)
    {
        QThread::msleep(100);
        QCoreApplication::processEvents();
    }

    if (stopPlayerPlaying)
    {
        return;
    }

    // Broadcast current game board status. Don't send all stones so no one
    // knows the stones of the other players
    for (std::list<QTcpSocket*>::iterator
            socket(otherPlayerSocks.begin());
            socket != otherPlayerSocks.end();
            ++socket)
    {
        Network::writeBoardStatus(*(*socket), stoneManager);

        if (playerHasWon)
        {
            Network::write(*(*socket), "playerWon");
        }

        Network::write(*(*socket), "roundFinished");
    }
}

void NetworkPlayerServer::opponentDisconnected()
{
    // Broadcast to other players that the player disconnected
    for (std::list<QTcpSocket*>::iterator
            socket(otherPlayerSocks.begin());
            socket != otherPlayerSocks.end();
            ++socket)
    {
        if ((*socket) != &sock
                && (*socket)->state() != QTcpSocket::ConnectedState)
        {
            Network::write(*(*socket), "disconnected");
            Network::write(*(*socket), this->getPlayerName());
        }
    }

    // Parent method will be called by event handler
    // NetworkPlayer::opponentDisconnected();
}
