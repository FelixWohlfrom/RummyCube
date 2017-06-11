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
 * GameLoadingTest.cpp
 *
 *  Created on: 27.05.2017
 *      Author: Felix Wohlfrom
 */

#include "GameLoadingTest.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "../../main/game/Settings.h"

#define SAVE_GAME_PATH QString("../resources/savegames/")

void GameLoadingTest::init()
{
    // First create a dummy window
    dummyWindow = new QWidget();

    // Then create the game that is used for testing
    game = new RummyCube();
    game->getStoneManager().createStones(dummyWindow);
}

void GameLoadingTest::cleanup()
{
    delete game;
    delete dummyWindow;
    Settings::cleanup();
}

void GameLoadingTest::verifyLoadingFailure_data()
{
    QTest::addColumn<QString>("loadGame");
    QTest::addColumn<QString>("failureRegEx");

    /**
     * This testcase verifies that loading a game fails if no 'settings' tag is found.
     */
    QTest::newRow("noSettingsTag")
        << SAVE_GAME_PATH + "Test_NoSettingsTag.rcs" << "Expected.*settings.*";

    /**
     * This testcase verifies that loading a game fails if no 'oldSettings' tag is found.
     */
    QTest::newRow("noOldSettingsTag")
        << SAVE_GAME_PATH + "Test_NoOldSettingsTag.rcs" << "Expected.*oldSettings.*";

    /**
     * This testcase verifies that loading a game fails if no 'stone' tag is found.
     */
    QTest::newRow("noStoneTag")
        << SAVE_GAME_PATH + "Test_NoStoneTag.rcs" << "Expected.*stone.*";

    /**
     * This testcase verifies that loading a game fails if no 'otherStones' tag is found.
     */
    QTest::newRow("noOtherStonesTag")
        << SAVE_GAME_PATH + "Test_NoOtherStonesTag.rcs" << "Expected.*otherStones.*";

    /**
     * This testcase verifies that loading a game fails if no 'joker' tag is found.
     */
    QTest::newRow("noJokerTag")
        << SAVE_GAME_PATH + "Test_NoJokerTag.rcs" << "Expected.*joker.*";

    /**
     * This testcase verifies that loading a game fails if no 'stone' tag is found.
     */
    QTest::newRow("noStoneWithinJokerTag")
        << SAVE_GAME_PATH + "Test_NoStoneTagWithinJoker.rcs" << "Expected.*stone.*";

    /**
     * This testcase verifies that loading a game fails if no 'player' tag is found.
     */
    QTest::newRow("noPlayerTag")
        << SAVE_GAME_PATH + "Test_NoPlayerTag.rcs" << "Expected.*player.*";

    /**
     * This testcase verifies that loading a game fails if no 'stone' tag is found in
     * a 'player' tag.
     */
    QTest::newRow("noStoneTagWithinPlayer")
        << SAVE_GAME_PATH + "Test_NoStoneTagWithinPlayer.rcs" << "Expected.*stone.*";
}

void GameLoadingTest::verifyLoadingFailure()
{
    QFETCH(QString, loadGame);
    QFETCH(QString, failureRegEx);

    QFile loadFile(loadGame);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        QFAIL(QString("Could not open file %1 for loading. %2")
                        .arg(loadGame).arg(loadFile.errorString()).toUtf8());
    }
    else
    {
        QXmlStreamReader saveGameReader(&loadFile);
        saveGameReader >> game;

        QVERIFY2(saveGameReader.hasError(), "Expected error while loading file, but got none");

        QRegularExpression expression(failureRegEx);
        QRegularExpressionMatch matcher = expression.match(saveGameReader.errorString());
        QVERIFY2(matcher.hasMatch(),
                        QString("Error message did not match expected message. Found: %1")
                            .arg(saveGameReader.errorString()).toUtf8());
    }
}

QTEST_MAIN(GameLoadingTest)
