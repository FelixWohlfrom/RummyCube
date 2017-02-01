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
 * AIPlayer.h
 *
 *  Created on: 13.09.2011
 *      Author: Felix Wohlfrom
 */

#ifndef AIPLAYER_H_
#define AIPLAYER_H_

#include "OpponentPlayer.h"

/**
 * This class represents an opponent player that is played by the computer.
 */
class AIPlayer: public OpponentPlayer
{
    public:
        /**
         * Creates a new ai player.
         *
         * @param number The player number
         * @param stoneManager The stone manager on which the player can play
         * @param initialStonesLeftToTake The initial stones that the player can take
         * @param sumInFirstRound The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         */
        AIPlayer(int number, StoneManager& stoneManager, int initialStonesLeftToTake = 0,
                int sumInFirstRound = 0, bool stonesInOneRow = false);

        /**
         * Lets the player play.
         */
        void play();

    private:
        /**
         * Takes the stones the player can take from heap.
         */
        void takeStones();

        /**
         * Clears all existing rows on player stones.
         */
        void clearRows();

        /**
         * Build rows and play them out just using own stones.
         *
         * @param sameColor If true rows have to be in same color, otherwise rows have to be with
         *                  same number
         */
        void playOutRowsUsingJustOwnStones(bool sameColor);

        /**
         * Plays out own rows using also stones taken from rows that are already played out.
         */
        void playOutStonesUsingStonesFromGameboard();

        /**
         * Counts the stones before given stone.
         *
         * @param stone The stone of which the stones before should be counted
         * @return The count of stones before given stone
         */
        int countStonesBefore(Gamestone& stone);

        /**
         * Counts the stones after given stone.
         *
         * @param stone The stone of which the stones after should be counted
         * @return The count of stones after given stone
         */
        int countStonesAfter(Gamestone& stone);

        /**
         * Plays out all rows that can be played out.
         */
        void playOutRows();

        /**
         * Appends the stones to existing rows on gameboard.
         */
        void appendToExistingRows();
};

#endif /* AIPLAYER_H_ */
