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
 * GameLoadingTest.h
 *
 *  Created on: 27.05.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_TEST_GAME_GAMELOADINGTEST_H_
#define SRC_TEST_GAME_GAMELOADINGTEST_H_

#include <QTest>

#include "../../main/game/RummyCube.h"

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
 * These testcases verify that loading broken files results in the correct failure messages.
 */
class GameLoadingTest: public QObject
{
    Q_OBJECT

    private:
        /**
         * A dummy window.
         */
        QWidget* dummyWindow;

        /**
         * The game instance to use for tests.
         */
        RummyCube* game;

        /**
         * Will load the game and verify that the error message matches the given expression.
         *
         * @param filename The file to load.
         * @param failureRegEx The regular expression to check.
         */
        void verifyLoadingFailure(QString filename, QString failureRegEx);

    private slots:
        /**
         * Initializes the testcase. Will create a dummy window and creates a new game.
         */
        void init();

        /**
         * Cleans up the dummy window and game.
         */
        void cleanup();

        /**
         * Generates a list of saved game files that contain the different misconfigured tags.
         */
        void verifyLoadingFailure_data();

        /**
         * @test
         * Tests if loading a savegame containing misconfigured tags is really failing and displays
         * correct failure message.
         */
        void verifyLoadingFailure();
};

#endif /* SRC_TEST_GAME_GAMELOADINGTEST_H_ */
