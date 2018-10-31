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
 * StoneManagerTest.h
 *
 *  Created on: 26.02.2017
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_STONES_STONEMANAGERTEST_H_
#define GAME_STONES_STONEMANAGERTEST_H_

#include <QObject>

#ifdef _DEBUG
    #ifdef _WIN32
        #ifdef _MSC_VER
             // Uncomment this if you installed visual memory
             // leak detector from http://vld.codeplex.com/
            #include <vld.h>
        #endif
    #endif
#endif

class StoneManagerTest: public QObject
{
    Q_OBJECT

    private slots:
        /**
         * Cleans up the testcases.
         */
        void cleanupTestCase();

        /**
         * @test
         * Tests if stone creation works properly.
         */
        void testStoneCreation();

        /**
         * @test
         * Tests that the stone count on heap is properly counted
         */
        void testStoneCountOnHeap();
};

#endif /* STONEMANAGERTEST_H_ */
