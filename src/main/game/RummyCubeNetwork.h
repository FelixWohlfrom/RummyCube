/*
 * RummyCubeNetwork.h
 *
 *  Created on: 08.01.2013
 *      Author: Felix Wohlfrom
 */

#ifndef RUMMYCUBENETWORK_H_
#define RUMMYCUBENETWORK_H_

#include "RummyCube.h"

#include "../network/NetworkGameSettings.h"

#include <map>
#include <list>

#include <QString>
#include <QTcpSocket>

/**
 * This class represents a network game of rummycube.
 */
class RummyCubeNetwork: public RummyCube
{
    public:
        /**
         * Ctor, for the servers. Create a new network game on a server player.
         *
         * @param settings The settings of the network game
         * @param opponentPlayers A map containing the sockets and the
         *        associated names of the opponent players
         */
        RummyCubeNetwork(NetworkGameSettings& settings, std::map<QTcpSocket*,
                QString> opponentPlayers);

        /**
         * Ctor, for the clients. Create a new network game on a client player.
         *
         * @param settings The settings of the network game
         * @param sock The socket on which the players should listen
         * @param humanPlayer The name of the human player
         * @param opponentPlayerNames The names of the opponent players
         */
        RummyCubeNetwork(NetworkGameSettings& settings, QTcpSocket* sock, QString humanPlayer,
                std::list<QString> opponentPlayerNames);

        /**
         * Dtor, cleans up the stuff.
         */
        virtual ~RummyCubeNetwork();

    private:
        /**
         * The socket which is used on network games on clients.
         */
        QTcpSocket* sock;
};

#endif /* RUMMYCUBENETWORK_H_ */
