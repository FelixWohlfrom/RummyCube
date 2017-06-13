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

#include "NetworkTest.h"

#include "../../main/game/Settings.h"
#include "../../main/game/RummyCube.h"
#include "../../main/network/Network.h"

#define SAVE_GAME_PATH QString("../resources/savegames/")

NetworkTestHandler::NetworkTestHandler(QTcpServer* server):
    server(server), serverSocket(NULL)
{}

QTcpSocket* NetworkTestHandler::getServerSocket()
{
    return serverSocket;
}

void NetworkTestHandler::acceptConnectionEvent()
{
    serverSocket = server->nextPendingConnection();
}

void NetworkTest::initTestCase()
{
    server = Network::createServerSocket(NETWORK_GAME_PORT);
    serverHandler = new NetworkTestHandler(server);
    QVERIFY2(server->isListening(),
             QString("Could not start server: %1").arg(server->errorString()).toUtf8().constData());
    connect(       server, SIGNAL(newConnection()),
            serverHandler, SLOT(acceptConnectionEvent()));

    clientSocket = Network::createClientSocket("127.0.0.1", NETWORK_GAME_PORT);

    QTest::qWait(100);

    QVERIFY2(serverHandler->getServerSocket() != NULL, "Did not connect client to server.");

    serverSocket = serverHandler->getServerSocket();
}

void NetworkTest::cleanupTestCase()
{
    clientSocket->close();
    Settings::cleanup();
    delete clientSocket;
    delete serverSocket;
    delete server;
    delete serverHandler;
}

void NetworkTest::testStringTransmissionShort()
{
    QString transmittedString = "testString";

    // Verify transmission from server to client
    Network::write(*serverSocket, transmittedString);
    QCOMPARE(Network::read(*clientSocket), transmittedString);

    // Verify transmission from client to server
    Network::write(*clientSocket, transmittedString);
    QCOMPARE(Network::read(*serverSocket), transmittedString);
}

void NetworkTest::testStringTransmissionLong()
{
    // Create a long string to transmit
    QString transmittedString = "testString-";
    for (int i = 0; i < 17; i++)
    {
        transmittedString += transmittedString;
    }

    // Verify transmission from server to client
    Network::write(*serverSocket, transmittedString);
    QString receivedString = Network::read(*clientSocket);
    QCOMPARE(transmittedString, receivedString);

    // Verify transmission from client to server
    Network::write(*clientSocket, transmittedString);
    receivedString = Network::read(*serverSocket);
    QCOMPARE(transmittedString, receivedString);
}

void NetworkTest::testReadingNetworkAdress()
{
    QVERIFY2(Network::getNetworkAddresses().length() > 0, "Did not read any network address");
}

void NetworkTest::testGameboardStatusTransmission_data()
{
    QTest::addColumn<QString>("loadGamePath");

    /**
     * This testcase verifies that the transmission of the inital gamestones works correct.
     */
    QTest::newRow("BeforeFirstRound")
        << SAVE_GAME_PATH + "Test_Before_First_Round.rcs";

    /*
     * This testcase verifies a game that was saved after first round and the player didn't
     * connect any stones yet is transmitted successfully.
     */
    QTest::newRow("FirstRoundNoConnectedStones")
        << SAVE_GAME_PATH + "Test_First_Round_No_Connected_Stones.rcs";

    /**
     * This testcase verifies a game that was saved after first round and the player connected
     * some stones is transmitted successfully.
     */
    QTest::newRow("FirstRoundStonesConnected")
        << SAVE_GAME_PATH + "Test_First_Round_Stones_Connected.rcs";
}

void NetworkTest::testGameboardStatusTransmission()
{
    QFETCH(QString, loadGamePath);

    // First create a dummy window
    QWidget dummyWindow;

    // Then create the games that are used for testing
    RummyCube transmittedGame;
    transmittedGame.getStoneManager().createStones(&dummyWindow);
    RummyCube receivedGame;
    receivedGame.getStoneManager().createStones(&dummyWindow);

    QFile loadGame(loadGamePath);
    if (!loadGame.open(QIODevice::ReadOnly))
    {
        QFAIL(QString("Could not open file %1 for loading for transmission. %2")
                        .arg(loadGamePath).arg(loadGame.errorString()).toUtf8());
    }
    else
    {
        QTextStream saveGameStreamReader(&loadGame);
        QString saveGameString = saveGameStreamReader.readAll();

        QXmlStreamReader transmittedReader(saveGameString);
        transmittedReader >> &transmittedGame;

        // Loading failure, show error message and return
        if (transmittedReader.hasError())
        {
            QFAIL(QString("Error during file loading. %1")
                    .arg(transmittedReader.errorString()).toUtf8());
        }

        QXmlStreamReader receivedReader(saveGameString);
        receivedReader >> &receivedGame;

        // Loading failure, show error message and return
        if (receivedReader.hasError())
        {
            QFAIL(QString("Error during file loading. %1")
                    .arg(receivedReader.errorString()).toUtf8());
        }
    }

    // Verify transmission from server to client
    Network::writeBoardStatus(*serverSocket, transmittedGame.getStoneManager());
    QString command = Network::read(*clientSocket);
    QCOMPARE(command, QString("boardStatus"));
    Network::readBoardStatus(*clientSocket, receivedGame.getStoneManager());

    compareStoneManagers(transmittedGame.getStoneManager(), receivedGame.getStoneManager());

    // Verify transmission from client to server
    Network::writeBoardStatus(*clientSocket, transmittedGame.getStoneManager());
    command = Network::read(*serverSocket);
    QCOMPARE(command, QString("boardStatus"));
    Network::readBoardStatus(*serverSocket, receivedGame.getStoneManager());

    compareStoneManagers(transmittedGame.getStoneManager(), receivedGame.getStoneManager());
}

void NetworkTest::compareStoneManagers(StoneManager& sourceManager, StoneManager& targetManager)
{
    QVector<Gamestone*> stones = sourceManager.getStones();
    for (QVector<Gamestone*>::iterator sourceStone(stones.begin());
           sourceStone != stones.end();
           ++sourceStone)
    {
        Gamestone* targetStone = targetManager.getStoneFromInt((*sourceStone)->asInt());

        QVERIFY2(targetStone != NULL, "Source stone not found in target stone manager");

        // Stones on the heap are just visible on the other players heap.
        // The position does not matter there.
        if ((*sourceStone)->getParent() == Gamestone::StoneParent::HEAP)
        {
            QVERIFY2(targetStone->getParent() == Gamestone::StoneParent::HEAP,
                            "Target stone expected on heap, but not found there.");
        }

        // Stones on the holder of the source need to be displayed on the ai holder of the other
        // players.
        if ((*sourceStone)->getParent() == Gamestone::StoneParent::HOLDER)
        {
            QVERIFY2(targetStone->getParent() == Gamestone::StoneParent::AIHOLDER,
                            "Target stone exepected on ai holder, but not found there.");
        }

        // Stones which are on the holder of the target need to be displayed on the ai holder of
        // the source.
        if (targetStone->getParent() == Gamestone::StoneParent::HOLDER)
        {
            QVERIFY2((*sourceStone)->getParent() == Gamestone::StoneParent::AIHOLDER,
                            "Target stone exepected on ai holder, but not found there.");
        }

        // Stones on the gameboard need to be in sync.
        if ((*sourceStone)->getParent() == Gamestone::StoneParent::BOARD)
        {
            QVERIFY2((*sourceStone)->getParent() == Gamestone::StoneParent::BOARD,
                "Target stone exepected on board, but not found there.");

            QCOMPARE((*sourceStone)->pos().x(), targetStone->pos().x());
            QCOMPARE((*sourceStone)->pos().y(), targetStone->pos().y());

            if ((*sourceStone)->getPrev() != NULL)
            {
                QVERIFY2((*sourceStone)->getPrev()->asInt() == targetStone->getPrev()->asInt(),
                                QString("Source stone and target stone previous stone are "
                                        "different. Expected %1 but received %2.")
                                        .arg(QString((*sourceStone)->getPrev()->asInt()))
                                        .arg(QString(targetStone->getPrev()->asInt())).toUtf8());
            }
            else
            {
                QVERIFY2(targetStone->getPrev() == NULL,
                                "Expected target stone previous stone to be NULL.");
            }

            if ((*sourceStone)->getNext() != NULL)
            {
                QVERIFY2((*sourceStone)->getNext()->asInt() == targetStone->getNext()->asInt(),
                                QString("Source stone and target stone next stone are "
                                        "different. Expected %1 but received %2.")
                                        .arg(QString((*sourceStone)->getNext()->asInt()))
                                        .arg(QString(targetStone->getNext()->asInt())).toUtf8());
            }
            else
            {
                QVERIFY2(targetStone->getNext() == NULL,
                                "Expected target stone previous stone to be NULL.");
            }
        }
    }
}

QTEST_MAIN(NetworkTest)
