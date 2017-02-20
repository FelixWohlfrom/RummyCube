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
 * GameSavingTest.h
 *
 *  Created on: 11.06.2016
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_GAMESAVINGTEST_H_
#define GAME_GAMESAVINGTEST_H_

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
 * These testcases verify that loading and saving a game to an xml file works correctly.
 * It loads different saved games, saves them again and verifies that they are saved correctly.
 */
class GameSavingTest: public QObject
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
         * Compares the two given files. Will fail if the two files are different.
         *
         * @param orig The path to the original file.
         * @param target The path to the target file.
         */
        void compareFiles(QString orig, QString target);

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
         * Generates a list of presaved game files to verify.
         */
        void verifyPresavedGame_data();

        /**
         * @test
         * Tests if loading loadGame and afterwards saving saveGame is successful and both files
         * are identical.
         */
        void verifyPresavedGame();

        /**
         * @test
         * Verifies that all stones of a game can be saved and loaded properly and that all values
         * are properly (re)stored.
         */
        void verifyStoneSaveAndLoading();
};

/**
 * A helper structure to store the data of a gamestone to be verified after loading.
 */
struct GamestoneData
{
    /**
     * The x value of the stone position.
     */
    int posX;
    /**
    * The y value of the stone position.
    */
    int posY;
    /**
    * The stone parent.
    */
    Gamestone::StoneParent parent;
    /**
     * The previous stone of the current stone.
     */
    Gamestone* prev;
    /**
    * The next stone of the current stone.
    */
    Gamestone* next;
};

#endif /* GAME_GAMESAVINGTEST_H_ */
