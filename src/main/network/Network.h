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
 * Network.h
 *
 *  Created on: 16.03.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include "../game/stones/StoneManager.h"

#include <QString>
#include <QTcpServer>
#include <QTcpSocket>

#define NETWORK_GAME_PORT 33333

/**
 * This class contains several classes that are useful for data transmission
 * over the network.
 */
class Network
{
    public:
        /**
         * Creates a new server socket.
         *
         * @param port The port which the server should listen to
         * @return The created server socket
         */
        static QTcpServer* createServerSocket(int port);

        /**
         * Creates a new client socket and connects it to given host and port.
         *
         * @param host The host to which the client should connect to
         * @param port The port to which the client should connect to
         * @return The created client socket
         */
        static QTcpSocket* createClientSocket(QString host, int port);

        /**
         * Writes the given string to given socket.
         *
         * @param sock The socket to write to
         * @param toWrite The string that should be written
         */
        static void write(QTcpSocket& sock, QString toWrite);

        /**
         * Reads a message from given socket.
         *
         * @param sock The socket from which should be read
         * @return The string read from socket
         */
        static QString read(QTcpSocket& sock);

        /**
         * Reads the board status from given socket.
         *
         * @param sock The socket from which the board status should be read
         * @param stonemanager The stone manager in which the read board status should be stored
         */
        static void readBoardStatus(QTcpSocket& sock, StoneManager& stonemanager);

        /**
         * Writes the board status to given socket.
         * Just writes the stones which are on heap and board.
         *
         * @param sock The socket to which the board status should be written.
         * @param stonemanager The stone manager from which the board status should be written
         */
        static void writeBoardStatus(QTcpSocket& sock, StoneManager& stonemanager);

        /**
         * Returns the network address(es) of the current pc.
         * loopback adresses are not included.
         *
         * @return A string containing the network address(es) of the current pc, separated by semicolon
         */
        static QString getNetworkAddresses();
};

#endif /* NETWORK_H_ */
