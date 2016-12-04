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
