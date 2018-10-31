/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2018 Felix Wohlfrom                                    *
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
 * AIPlayerTest.cpp
 *
 *  Created on: 30.10.2018
 *      Author: Felix Wohlfrom
 */

#include "AIPlayerTest.h"

#include <QTest>

#include "../../../main/game/player/AIPlayer.h"
#include "../../../main/game/stones/StoneManager.h"

void AIPlayerTest::constructorTest()
{
    StoneManager stoneManager;
    AIPlayer* testPlayer = new AIPlayer(1, stoneManager, 10, 0, false);

    QVERIFY2(testPlayer != NULL, "AIPlayer not created");
    QVERIFY2(!testPlayer->getPlayerName().isEmpty(), "Invalid player name");
    QVERIFY2(testPlayer->getStonesLeftToTake() == 10, "Did not set initial stones left to take");
    QVERIFY2(testPlayer->hasWon() == true, "Player did not win, altough it has no more stones");

    delete testPlayer;
}

void AIPlayerTest::verifyPlaying()
{
    // initialize random seed with fixed value to have more
    // reproducable results
    srand(10);

    StoneManager* stoneManager = new StoneManager();
    QWidget parent;
    stoneManager->createStones(&parent);

    AIPlayer* testPlayer = new AIPlayer(1, *stoneManager, 30, 10, false);

    testPlayer->play();

    while (!testPlayer->hasWon())
    {
        testPlayer->play();
    }

    delete testPlayer;
    delete stoneManager;
}

QTEST_MAIN(AIPlayerTest)
