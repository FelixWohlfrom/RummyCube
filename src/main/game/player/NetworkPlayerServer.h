/*
 * NetworkPlayerServer.h
 *
 *  Created on: 14.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKPLAYERSERVER_H_
#define NETWORKPLAYERSERVER_H_

#include "NetworkPlayer.h"

#include <list>

/**
 * Represents an opponent player for network game on the server.
 */
class NetworkPlayerServer: public NetworkPlayer
{
    public:
        /**
         * Creates a new opponent player for network game on server.
         *
         * @param name The name of the player
         * @param stoneManager The stone manager on which the player can play
         * @param sock The socket which is connected to the player
         * @param allPlayerSocks The sockets which are connected to the other
         *                       players (for broadcasting)
         */
        NetworkPlayerServer(QString name, StoneManager& stoneManager,
                QTcpSocket& sock, std::list<QTcpSocket*> allPlayerSocks);

        /**
         * Lets the player play.
         */
        void play();

    private:
        /**
         * The sockets which are connected with other players (for broadcasting).
         */
        std::list<QTcpSocket*> otherPlayerSocks;

    public slots:
        /**
         * Event handler if the opponent (the server) disconnected.
         */
        void opponentDisconnected();
};

#endif /* NETWORKPLAYERSERVER_H_ */
