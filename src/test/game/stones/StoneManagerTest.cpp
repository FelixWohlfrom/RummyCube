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
 * StoneManagerTest.cpp
 *
 *  Created on: 26.02.2017
 *      Author: Felix Wohlfrom
 */

#include "StoneManagerTest.h"

#include <QTest>

#include "../../../main/game/Settings.h"
#include "../../../main/game/stones/StoneManager.h"

void StoneManagerTest::cleanupTestCase()
{
    Settings::cleanup();
}

void StoneManagerTest::testStoneCreation()
{
    QWidget testWidget;
    StoneManager stoneManager;

    stoneManager.createStones(&testWidget);

    QVector<Gamestone*>& stones = stoneManager.getStones();

    QVERIFY2(stones.size() > 0, "No stones where created");
}

QTEST_MAIN(StoneManagerTest)
