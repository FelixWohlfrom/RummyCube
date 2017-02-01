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
 * HumanPlayer.h
 *
 *  Created on: 10.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

#include "Player.h"
#include "../stones/Gamestone.h"

/**
 * This class represents the human player that is playing the game.
 */
class HumanPlayer: public Player
{
    public:
        /**
         * Creates a new human player.
         *
         * @param stoneManager The stone manager on which the player can play
         * @param initialStonesLeftToTake The initial stones that the player can take
         * @param sumInFirstRound The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         */
        HumanPlayer(StoneManager& stoneManager, int initialStonesLeftToTake,
                int sumInFirstRound, bool stonesInOneRow);

        /**
         * Dummy to avoid warnings.
         */
        virtual ~HumanPlayer();

        /**
         * Test the stones on gameboard.
         *
         * @return true If all stones passed the tests
         */
        bool testStonesOnGameboard();

        /**
         * This method is used in network game to notify other players that the round of the
         * current player has been finished.
         */
        virtual void roundFinished();

    private:
        /**
         * Resets the stone invalidation.
         */
        void resetStoneInvalidation();

        /**
         * Invalidates all own stones on the game board.
         */
        void invalidateOwnStonesOnBoard();

        /**
         * Invalidates the row of given stone.
         *
         * @param stone The stone of which the row should be invalidated
         */
        void invalidateRow(Gamestone& stone);
};

#endif /* HUMANPLAYER_H_ */
