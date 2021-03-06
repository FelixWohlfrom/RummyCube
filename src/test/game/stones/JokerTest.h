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
 * JokerTest.h
 *
 *  Created on: 25.02.2017
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_STONES_JOKERTEST_H_
#define GAME_STONES_JOKERTEST_H_

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

/**
 * This class tests that the basic joker functionalities work as expected.
 */
class JokerTest: public QObject
{
    Q_OBJECT

    private slots:
        /**
         * Cleans up the testcases.
         */
        void cleanupTestCase();

        /**
         * @test
         * Tests if the default constructor is working.
         */
        void defaultConstructorTest();

        /**
         * @test
         * Tests if the constructor with parameters works fine.
         */
        void constructorTest();

        /**
         * @test
         * Tests if invalidating works properly.
         */
        void testInvalidation();

        /**
         * Data generator for parent switching tests.
         */
        void testParentSwitching_data();

        /**
         * @test
         * Verifies that setting stone parents works properly and that invalidation of stones
         * is done properly on moving stones back to holder.
         */
        void testParentSwitching();

        // TODO Test Reject parent switching

        /**
         * @test
         * Verifies that storing stone data and restoring the data works properly.
         */
        void testStoneStoringAndRestoring();

        /**
         * @test
         * Verifies that storing and restoring to/from a stream works properly.
         */
        void testStoneStoringAndRestoringFromStream();
};

#endif /* GAME_STONES_JOKERTEST_H_ */
