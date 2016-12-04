/*
 * NetworkPlayerClient.h
 *
 *  Created on: 09.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKPLAYERCLIENT_H_
#define NETWORKPLAYERCLIENT_H_

#include "NetworkPlayer.h"

/**
 * Represents an opponent player for network game on the clients.
 */
class NetworkPlayerClient: public NetworkPlayer
{
    Q_OBJECT

    public:
        /**
         * Creates a new opponent player for network game on client.
         *
         * @param name The name of the player
         * @param stoneManager The stone manager on which the player can play
         * @param sock The socket which the player uses for communication
         * @param stonesAtBeginning The initial stones that the player can take
         * @param sumAtBeginning The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         * @param skipAfterCreation If the player play() method should be skipped directly after
         *                          creation of game
         */
        NetworkPlayerClient(QString name, StoneManager& stoneManager, QTcpSocket& sock,
                int stonesAtBeginning, int sumAtBeginning, bool stonesInOneRow,
                bool skipAfterCreation);

        /**
         * Lets the player play.
         */
        void play();

    private:
        /**
         * If the current opponent should be skipped after game creation.
         */
        bool skipAfterCreation;
};

#endif /* NETWORKPLAYERCLIENT_H_ */
