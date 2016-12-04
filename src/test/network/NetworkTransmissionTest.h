/*
 * NetworkTransmissionTest.h
 *
 *  Created on: 08.09.2016
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORK_NETWORKTRANSMISSIONTEST_H_
#define NETWORK_NETWORKTRANSMISSIONTEST_H_

#include <QTest>
#include <QTcpServer>
#include <QTcpSocket>

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
class NetworkTransmissionTestHandler: public QObject
{
    Q_OBJECT

    public:
        /**
         * Ctor, creates a new handler for network transmission tests.
         *
         * @param server The server from which the events should be handled.
         */
        NetworkTransmissionTestHandler(QTcpServer* server);

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
class NetworkTransmissionTest: public QObject
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
        NetworkTransmissionTestHandler* serverHandler;

        /**
         * The server socket.
         */
        QTcpSocket* serverSocket;

        /**
         * The client socket.
         */
        QTcpSocket* clientSocket;

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
};

#endif /* NETWORK_NETWORKTRANSMISSIONTEST_H_ */
