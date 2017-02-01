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
