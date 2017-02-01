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
 * NetworkPlayer.cpp
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkPlayer.h"

NetworkPlayer::NetworkPlayer(QString name, StoneManager& stoneManager, QTcpSocket& sock,
        int stonesAtBeginning, int sumAtBeginning, bool stonesInOneRow) :
    OpponentPlayer(name, stoneManager, stonesAtBeginning, sumAtBeginning, stonesInOneRow),
    playerHasWon(false), roundFinished(false), sock(sock)
{
    connect(&sock, SIGNAL(readyRead()),
             this, SLOT(parseData()));

    connect(&sock, SIGNAL(disconnected()),
             this, SLOT(opponentDisconnected()));
}

NetworkPlayer::~NetworkPlayer() {}

bool NetworkPlayer::hasWon()
{
    return playerHasWon;
}

QTcpSocket& NetworkPlayer::getSocket()
{
    return sock;
}

void NetworkPlayer::parseData()
{
    while (sock.bytesAvailable() > 0)
    {
        QString msg = Network::read(sock);

        if (msg == "boardStatus")
        {
            try
            {
                Network::readBoardStatus(sock, stoneManager);
            }
            catch (const QString& e) 
            {
                emit connectionError(e);
            }
        }
        else if (msg == "disconnected")
        {
            QString playerName = Network::read(sock);
            // TODO Notify other players and main window once more than two
            // players are supported

            stopPlayerPlaying = true;
        }
        else if (msg == "playerWon")
        {
            playerHasWon = true;
        }
        else if (msg == "roundFinished")
        {
            roundFinished = true;
        }
        else
        {
            emit connectionError(tr("Received invalid command: %1").arg(msg));
        }
    }
}

void NetworkPlayer::opponentDisconnected()
{
    stopPlayerPlaying = true;
    if (playerHasWon)
    {
        return;
    }
}
