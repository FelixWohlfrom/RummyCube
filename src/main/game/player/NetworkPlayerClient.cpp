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
