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
 * NetworkTransmissionTest.cpp
 *
 *  Created on: 08.09.2016
 *      Author: Felix Wohlfrom
 */

#include "NetworkTransmissionTest.h"

#include "../../main/game/RummyCube.h"
#include "../../main/network/Network.h"

NetworkTransmissionTestHandler::NetworkTransmissionTestHandler(QTcpServer* server):
    server(server), serverSocket(NULL)
{}

QTcpSocket* NetworkTransmissionTestHandler::getServerSocket()
{
    return serverSocket;
}

void NetworkTransmissionTestHandler::acceptConnectionEvent()
{
    serverSocket = server->nextPendingConnection();
}

void NetworkTransmissionTest::initTestCase()
{
    server = Network::createServerSocket(NETWORK_GAME_PORT);
    serverHandler = new NetworkTransmissionTestHandler(server);
    QVERIFY2(server->isListening(),
             QString("Could not start server: %1").arg(server->errorString()).toUtf8().constData());
    connect(       server, SIGNAL(newConnection()),
            serverHandler, SLOT(acceptConnectionEvent()));

    clientSocket = Network::createClientSocket("127.0.0.1", NETWORK_GAME_PORT);

    QTest::qWait(100);

    QVERIFY2(serverHandler->getServerSocket() != NULL, "Did not connect client to server.");

    serverSocket = serverHandler->getServerSocket();
}

void NetworkTransmissionTest::cleanupTestCase()
{
    clientSocket->close();
    delete clientSocket;
    delete serverSocket;
    delete server;
    delete serverHandler;
}

void NetworkTransmissionTest::testStringTransmissionShort()
{
    QString transmittedString = "testString";

    // Verify transmission from server to client
    Network::write(*serverSocket, transmittedString);
    QCOMPARE(Network::read(*clientSocket), transmittedString);

    // Verify transmission from client to server
    Network::write(*clientSocket, transmittedString);
    QCOMPARE(Network::read(*serverSocket), transmittedString);
}

void NetworkTransmissionTest::testStringTransmissionLong()
{
    // Create a long string to transmit
    QString transmittedString = "testString-";
    for (int i = 0; i < 15; i++)
    {
        transmittedString += transmittedString;
    }

    int transmittedStrings = transmittedString.split("-").length();

    // Verify transmission from server to client
    Network::write(*serverSocket, transmittedString);
    QString receivedString = Network::read(*clientSocket);
    int receivedStrings = receivedString.split("-").length();
    QVERIFY2(transmittedStrings == receivedStrings,
             QString("Received %1 instead of %2")
             .arg(QString::number(receivedStrings))
             .arg(QString::number(transmittedStrings))
             .toUtf8().constData());

    // Verify transmission from client to server
    Network::write(*clientSocket, transmittedString);
    receivedString = Network::read(*serverSocket);
    receivedStrings = receivedString.split("-").length();
    QVERIFY2(transmittedStrings == receivedStrings,
             QString("Received %1 instead of %2")
             .arg(QString::number(receivedStrings))
             .arg(QString::number(transmittedStrings))
             .toUtf8().constData());
}

QTEST_MAIN(NetworkTransmissionTest)
