/*
 * GameSavingTest.h
 *
 *  Created on: 11.06.2016
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_GAMESAVINGTEST_H_
#define GAME_GAMESAVINGTEST_H_

#include <QTest>

#include <QString>

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
         * Tests if loading loadGame and afterwards saving saveGame is successful.
         *
         * @param loadGame The path to the savegame file to load
         */
        void testGameLoadingAndSaving(QString loadGame);

        /**
         * Compares the two given files. Will fail if the two files are different.
         *
         * @param orig The path to the original file.
         * @param target The path to the target file.
         */
        void compareFiles(QString orig, QString target);

    private slots:
        /**
         * @test
         * This testcase verifies that a game that was saved before any player has played out
         * can be loaded and saved again successfully.
         */
        void savegameBeforeFirstRound();

        /**
         * @test
         * This testcase verifies a game that was saved after first round and the player didn't
         * connect any stones yet can be saved again successfully.
         */
        void savegameNoConnectedStones();

        /**
         * @test
         * This testcase verifies a game that was saved after first round and the player connected
         * some stones can be saved again successfully.
         */
        void savegameConnectedStones();
};

#endif /* GAME_GAMESAVINGTEST_H_ */
