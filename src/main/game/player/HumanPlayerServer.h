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
 * HumanPlayerServer.h
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef HUMANPLAYERSERVER_H_
#define HUMANPLAYERSERVER_H_

#include "HumanPlayer.h"

#include <list>

#include <QTcpSocket>

/**
 * Represents a human player on server side during network game.
 */
class HumanPlayerServer: public HumanPlayer
{
    public:
        /**
         * Creates a new human player on server on network game.
         *
         * @param stoneManager The stone manager on which the player can play
         * @param stonesAtBeginning The initial stones that the player can take
         * @param sumAtBeginning The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         * @param otherPlayerSocks The sockets which are connected to the other players
         *                         (for broadcasting)
         */
        HumanPlayerServer(StoneManager& stoneManager, int stonesAtBeginning, int sumAtBeginning,
                bool stonesInOneRow, std::list<QTcpSocket*> otherPlayerSocks);

        /**
         * Dummy to avoid warnings.
         */
        ~HumanPlayerServer();

        /**
         * Notifies other players that round is finished.
         */
        void roundFinished();

    private:
        /**
         * The sockets which are connected with other players
         * (for broadcasting).
         */
        std::list<QTcpSocket*> otherPlayerSocks;
};

#endif /* HUMANPLAYERSERVER_H_ */
