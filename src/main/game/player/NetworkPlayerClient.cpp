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
 * NetworkPlayerClient.cpp
 *
 *  Created on: 09.04.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkPlayerClient.h"

#include "../../network/Network.h"

#include <QThread>
#include <QCoreApplication>

NetworkPlayerClient::NetworkPlayerClient(QString name, StoneManager& stoneManager, QTcpSocket& sock,
        int stonesAtBeginning, int sumAtBeginning, bool stonesInOneRow, bool skipAfterCreation) :
    NetworkPlayer(name, stoneManager, sock, stonesAtBeginning, sumAtBeginning, stonesInOneRow),
    skipAfterCreation(skipAfterCreation)
{}

void NetworkPlayerClient::play()
{
    // Skip if variable is set
    if (skipAfterCreation)
    {
        skipAfterCreation = false;
        return;
    }

    roundFinished = false;

    while (!roundFinished && !stopPlayerPlaying)
    {
        QThread::msleep(100);
        QCoreApplication::processEvents();
    }
}
