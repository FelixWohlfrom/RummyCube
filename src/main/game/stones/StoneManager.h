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
 * StoneManager.h
 *
 *  Created on: 25.07.2011
 *      Author: Felix Wohlfrom
 */

#ifndef STONEMANAGER_H_
#define STONEMANAGER_H_

#include <QVector>

#include "Gamestone.h"

/**
 * The stone manager handles all gamestone specific stuff.
 * It's used for example for creation/destroying, but also for drag'n'drop.
 */
class StoneManager
{
    public:
        /*
         * C'tor. Inits variables.
         */
        StoneManager();

        /**
         * D'tor. Cleans up the stones.
         */
        ~StoneManager();

        /**
         * Indicates if all stones in a row should be moved at once.
         */
        bool moveAllStonesInRow;

        /**
         * Creates a bunch of stones on given heap.
         *
         * @param heap The heap on which the stones should be created
         */
        void createStones(QWidget* heap);

        /**
         * Returns the stones.
         *
         * @return A reference to the stone vector
         */
        QVector<Gamestone*>& getStones();

        /**
         * Returns the stone count on heap.
         *
         * @return The number of stones which are still on the heap
         */
        int getStoneCountOnHeap() const;

        /**
         * Returns the gamestone represented by the int value.
         * The value needs to be built as described in Gamestone.asInt() method
         *
         * @param value The integer value of the stone which should be returned
         * @return A pointer to the stone which is described in value
         */
        Gamestone* getStoneFromInt(int value) const;

        /**
         * Stores the stone that is currently dragged on the gameboard.
         * If no stone is dragged, this is NULL.
         */
        Gamestone* draggedStone;

    private:
        /**
         * The gamestones managed by current stone manager.
         */
        QVector<Gamestone*> gamestones;
};

#endif /* STONEMANAGER_H_ */
