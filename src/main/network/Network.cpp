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
 * Network.cpp
 *
 *  Created on: 16.03.2012
 *      Author: Felix Wohlfrom
 */

#include "Network.h"

#include <set>

#include <QNetworkInterface>

QTcpServer* Network::createServerSocket(int port)
{
    QTcpServer* server = new QTcpServer(NULL);
    server->listen(QHostAddress::Any, port);
    return server;
}

QTcpSocket* Network::createClientSocket(QString host, int port)
{
    QTcpSocket* client = new QTcpSocket();
    client->connectToHost(host, port);
    return client;
}

// Use qint32 to ensure that the number has 4 bytes
static inline QByteArray IntToArray(qint32 source)
{
    // Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

static inline qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}

void Network::write(QTcpSocket& sock, QString toWrite)
{
    int len = toWrite.toUtf8().size();
    #ifdef _DEBUG
        std::cout << "write: len - " << len << std::endl;
    #endif

    // First send the size of the string
    sock.write(IntToArray(len));

    // Now transmit the data as string
    #if _DEBUG
        // std::cout << "send - " << toWrite.toStdString() << std::endl;
    #endif
    sock.write(toWrite.toUtf8());
    sock.flush();
}

QString Network::read(QTcpSocket& sock)
{
    // Read the size of the following data
    int len = ArrayToInt(sock.read(4));
    #ifdef _DEBUG
        std::cout << "read: len - " << len << std::endl;
    #endif
    while (len == 0)
    {
        sock.waitForReadyRead();
        len = ArrayToInt(sock.read(4));
        #ifdef _DEBUG
            std::cout << "read: len - " << len << std::endl;
        #endif
    }

    // Now read the complete string
    QString msg;
    while (len > 0)
    {
        QString recvd = QString::fromUtf8(sock.read(len));
        msg += recvd;
        len = len - recvd.toUtf8().size();
        #ifdef _DEBUG
            std::cout << "still to receive len: " << len << std::endl;
            // std::cout << "recvd - " << msg.toStdString() << std::endl;
        #endif

        if (len > 0)
        {
            sock.waitForReadyRead();
        }
    }
    return msg;
}

void Network::readBoardStatus(QTcpSocket& sock, StoneManager& stonemanager)
{
    // First read the board status as xml string
    QString boardStatus = read(sock);

    // Parse board status from xml stream
    QXmlStreamReader input(boardStatus);

    // Check if stones need to be moved to ai holder
    QVector<Gamestone*> gamestones = stonemanager.getStones();
    std::set<Gamestone*> toAiHolder;
    for (QVector<Gamestone*>::iterator stone(gamestones.begin());
            stone != gamestones.end();
            ++stone)
    {
        toAiHolder.insert(*stone);
    }

    // Read stones from socket
    if (!input.readNextStartElement() || input.name() != "GameStones")
    {
        input.raiseError(QObject::tr("Expected 'GameStones' Tag, "
                "instead found '%1'.").arg(input.name().toString()));
        return;
    }

    while (input.readNextStartElement() && input.name() == "stone")
    {
        int stoneValue = input.attributes().value("val").toInt();
        if (stoneValue != 0)
        {
            Gamestone* stone = stonemanager.getStoneFromInt(stoneValue);
            input >> stone;

            toAiHolder.erase(stone);
        }
    }

    // Move the remaining stones to ai holder, if not own stones
    for (std::set<Gamestone*>::iterator stone(toAiHolder.begin());
            stone != toAiHolder.end();
            ++stone)
    {
        (*stone)->setParent(Gamestone::AIHOLDER);
    }

    // Store stone previous positions
    for (QVector<Gamestone*>::iterator stone(gamestones.begin());
                stone != gamestones.end();
                ++stone)
    {
        (*stone)->storeStone();
    }

    if (input.hasError())
    {
        throw input.error();
    }
}

void Network::writeBoardStatus(QTcpSocket& sock, StoneManager& stonemanager)
{
    write(sock, "boardStatus");

    QString xmlString;
    QXmlStreamWriter output(&xmlString);

    output.writeDTD("<!DOCTYPE Rummycube>");
    output.writeStartElement("GameStones");

    QVector<Gamestone*> stones = stonemanager.getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        if ((*stone)->getParent() == Gamestone::HEAP
                || (*stone)->getParent() == Gamestone::BOARD)
        {
            output << (*stone);
        }
    }

    output.writeEndElement();
    output.writeEndDocument();

    write(sock, xmlString);
}

QString Network::getNetworkAddresses()
{
    QString addresses;

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if (!address.isLoopback())
        {
            if (addresses.length() == 0)
            {
                addresses += address.toString();
            }
            else
            {
                addresses += ";" + address.toString();
            }
        }
    }

    return addresses;
}
