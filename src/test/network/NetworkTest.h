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
 * NetworkTest.h
 *
 *  Created on: 08.09.2016
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORK_NETWORKTEST_H_
#define NETWORK_NETWORKTEST_H_

#include <QTest>
#include <QTcpServer>
#include <QTcpSocket>
#include "../../main/game/stones/StoneManager.h"

#ifdef _DEBUG
    #ifdef _WIN32
        #ifdef _MSC_VER
             // Uncomment this if you installed visual memory
             // leak detector from http://vld.codeplex.com/
            #include <vld.h>
        #endif
    #endif
#endif

/**
 * This class contains the handler for events sent during network transmission tests.
 */
class NetworkTestHandler: public QObject
{
    Q_OBJECT

    public:
        /**
         * Ctor, creates a new handler for network transmission tests.
         *
         * @param server The server from which the events should be handled.
         */
        NetworkTestHandler(QTcpServer* server);

        /**
         * Returns the created server socket.
         */
        QTcpSocket* getServerSocket();

    private:
        /**
         * The server object to which the clients will connect.
         */
        QTcpServer* server;

        /**
         * The server socket which will be created once the client is connected to the server.
         */
        QTcpSocket* serverSocket;

    private slots:
        /**
         * This event is called on server socket once a new client wants to connect.
         */
        void acceptConnectionEvent();
};

/**
 * These testcases verify that network transmission works fine.
 * It sends and receives several data via localhost.
 */
class NetworkTest: public QObject
{
    Q_OBJECT

    private:
        /**
         * The server object to which the clients will connect.
         */
        QTcpServer* server;

        /**
         * A handler for network transmission events received by the server.
         */
        NetworkTestHandler* serverHandler;

        /**
         * The server socket.
         */
        QTcpSocket* serverSocket;

        /**
         * The client socket.
         */
        QTcpSocket* clientSocket;

        /**
         * Compares the stones handled by two network managers.
         *
         * @sourceManager The source stone manager to compare
         * @targetManager The target stone manager to compare
         */
        void compareStoneManagers(StoneManager& sourceManager, StoneManager& targetManager);

    private slots:
        /**
         * Initalize server and client sockets. Will create a server and connect a client to it.
         */
        void initTestCase();

        /**
         * Cleans up network connection.
         */
        void cleanupTestCase();

        /**
         * @test
         * This test sends a short simple string via network and verifies if
         * it is received correctly.
         */
        void testStringTransmissionShort();

        /**
         * @test
         * This test sends a long string via network and verifies if
         * it is received correctly.
         */
        void testStringTransmissionLong();

        /**
         * @test
         * Verifies that reading network addresses works.
         */
        void testReadingNetworkAdress();

        /**
         * Generates a list of network stati to be transmitted over the network.
         */
        void testGameboardStatusTransmission_data();

        /**
         * @test
         * Sends a gameboard status and checks if it is received correctly.
         */
        void testGameboardStatusTransmission();
};

#endif /* NETWORK_NETWORKTEST_H_ */
