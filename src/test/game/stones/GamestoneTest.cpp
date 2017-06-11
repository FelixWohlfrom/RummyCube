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
 * GamestoneTest.cpp
 *
 *  Created on: 19.02.2017
 *      Author: Felix Wohlfrom
 */

#include "GamestoneTest.h"

#include <QTest>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "../../../main/game/Settings.h"
#include "../../../main/game/stones/Gamestone.h"
#include "../../../main/game/stones/StoneManager.h"
#include "../../../main/game/stones/ParentChangedEvent.h"

// We need the meta type to use stone parent enum as data generator
Q_DECLARE_METATYPE(Gamestone::StoneParent);

void GamestoneTest::cleanupTestCase()
{
    Settings::cleanup();
}

void GamestoneTest::defaultConstructorTest()
{
    Gamestone* testStone = new Gamestone();

    QVERIFY2(testStone != NULL, "Gamestone not created");
    QCOMPARE(testStone->isInvalid(), false);
    QCOMPARE(testStone->isFirst(), false);
    QCOMPARE(testStone->getColor(), Gamestone::StoneColor::BLACK);
    QCOMPARE(testStone->getNumber(), 0);
    QVERIFY2(testStone->parent() == NULL,
                 "Parent widget set although none expected: " + testStone->getParent());
    QCOMPARE(testStone->getParent(), Gamestone::StoneParent::HEAP);
    QVERIFY2(testStone->getPlayer() == NULL, "Player set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Previous stone set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Next stone set although none expected.");
    QCOMPARE(testStone->isJoker(), false);
    QCOMPARE(testStone->isFirstTimePlayedOut(), true);

    delete testStone;
}

void GamestoneTest::constructorTest()
{
    StoneManager stoneManager;
    QWidget parent;
    Gamestone* testStone = new Gamestone(&parent, &stoneManager, true,
                                         Gamestone::StoneColor::BLUE, 10);

    QVERIFY2(testStone != NULL, "Gamestone not created");
    QCOMPARE(testStone->isInvalid(), false);
    QCOMPARE(testStone->isFirst(), true);
    QCOMPARE(testStone->getColor(), Gamestone::StoneColor::BLUE);
    QCOMPARE(testStone->getNumber(), 10);
    QCOMPARE(testStone->parent(), &parent);
    QCOMPARE(testStone->getParent(), Gamestone::StoneParent::HEAP);
    QVERIFY2(testStone->getPlayer() == NULL, "Player set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Previous stone set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Next stone set although none expected.");
    QCOMPARE(testStone->isJoker(), false);
    QCOMPARE(testStone->isFirstTimePlayedOut(), true);

    delete testStone;
}

void GamestoneTest::testInvalidation()
{
    Gamestone testStone;
    testStone.setInvalid(true);
    QCOMPARE(testStone.isInvalid(), true);
    testStone.setInvalid(false);
    QCOMPARE(testStone.isInvalid(), false);
}

void GamestoneTest::testFromIntUniqueness()
{
    QWidget parent;
    StoneManager stoneManager;
    QString colorNames[] = { "Unknown", "Black", "Blue", "Yellow", "Red" };

    stoneManager.createStones(&parent);

    QVector<Gamestone*>& stones = stoneManager.getStones();
    QVector<Gamestone*>& comparisonStones = stoneManager.getStones();

    for (QVector<Gamestone*>::const_iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        for (QVector<Gamestone*>::const_iterator comparisonStone(comparisonStones.begin());
                comparisonStone != comparisonStones.end();
                ++comparisonStone)
        {
            if (&(*stone) != &(*comparisonStone))
            {
                QVERIFY2((*stone)->asInt() != (*comparisonStone)->asInt(),
                         QString("Duplicate value of 'asInt()' method, check coding.\n"
                             "Stones: '%1 (%2) %3', '%4 (%5) %6'")
                             .arg((*stone)->getNumber())
                             .arg(colorNames[(*stone)->getColor() + 1])
                             .arg((*stone)->isFirst())
                             .arg((*comparisonStone)->getNumber())
                             .arg(colorNames[(*comparisonStone)->getColor() + 1])
                             .arg((*comparisonStone)->isFirst())
                        .toUtf8().constData());
            }
        }
    }
}

void GamestoneTest::testParentSwitching_data()
{
    QTest::addColumn<Gamestone::StoneParent>("stoneParent");
    QTest::addColumn<bool>("invalid");

    QTest::newRow("Holder") << Gamestone::StoneParent::HOLDER << false;
    QTest::newRow("AIHolder") << Gamestone::StoneParent::AIHOLDER << true;
    QTest::newRow("Heap") << Gamestone::StoneParent::HEAP << true;
    QTest::newRow("Board") << Gamestone::StoneParent::BOARD << true;
}

void GamestoneTest::testParentSwitching()
{
    QFETCH(Gamestone::StoneParent, stoneParent);
    QFETCH(bool, invalid);

    Gamestone testStone;

    // We can't use QSignalSpy here since the parameter is a reference.
    // Instead we use lambda annotation here to handle the event.
    std::vector<ParentChangedEvent> events;
    QObject::connect(&testStone, &Gamestone::parentChanged,
                     [&](ParentChangedEvent& event) { events.emplace_back(event); });

    testStone.setInvalid(true);
    testStone.setParent(stoneParent);
    QCOMPARE(testStone.getParent(), stoneParent);
    QCOMPARE(testStone.isInvalid(), invalid);

    // Verify that correct amount of events is emitted
    QCOMPARE((ulong)events.size(), 1ul);

    ParentChangedEvent event = events.at(0);
    QCOMPARE(event.getParent(), (int)stoneParent);
    QCOMPARE(event.getSender(), &testStone);
    QCOMPARE(event.getStoneMoved(), false);
    QCOMPARE(event.getVeto(), false);
}

void GamestoneTest::testSetFirstTimePlayedOut()
{
    StoneManager stoneManager;
    QWidget parent;
    Gamestone testStone(&parent, &stoneManager, true, Gamestone::StoneColor::BLUE, 10);

    QVERIFY2(testStone.isFirstTimePlayedOut(), "First time played out should be set");

    testStone.finishedFirstTimePlayedOut();
    QVERIFY2(!testStone.isFirstTimePlayedOut(), "First time played out should not be set anymore");
}

void GamestoneTest::testStoneStoringAndRestoring()
{
    StoneManager stoneManager;
    QWidget parent;
    Gamestone testStone(&parent, &stoneManager, true, Gamestone::StoneColor::BLUE, 10);

    // TODO Set prev and next stone

    testStone.move(10, 100);
    testStone.setParent(Gamestone::StoneParent::BOARD);

    testStone.storeStone();

    testStone.move(20, 200);
    testStone.setParent(Gamestone::StoneParent::HEAP);

    testStone.restoreStone();

    QCOMPARE(testStone.pos().x(), 10);
    QCOMPARE(testStone.pos().y(), 100);
    QCOMPARE(testStone.getParent(), Gamestone::StoneParent::BOARD);

    testStone.storeStone(Gamestone::StoneParent::HOLDER);

    testStone.move(20, 200);
    testStone.setParent(Gamestone::StoneParent::HEAP);

    testStone.restoreStone();

    QCOMPARE(testStone.pos().x(), 10);
    QCOMPARE(testStone.pos().y(), 100);
    QCOMPARE(testStone.getParent(), Gamestone::StoneParent::HOLDER);
}

void GamestoneTest::testStoneStoringAndRestoringFromStream()
{
    StoneManager stoneManager;
    QWidget parent;
    Gamestone testStone(&parent, &stoneManager, true, Gamestone::StoneColor::BLUE, 10);

    // TODO Set prev and next stone

    testStone.move(10, 100);
    testStone.setParent(Gamestone::StoneParent::BOARD);

    testStone.storeStone();

    testStone.move(20, 200);
    testStone.setParent(Gamestone::StoneParent::HEAP);

    // Store data in string to restore it later
    QString testString;
    QXmlStreamWriter streamWriter(&testString);
    streamWriter << &testStone;

    // Update stone data
    testStone.move(30, 300);
    testStone.setParent(Gamestone::StoneParent::HOLDER);
    testStone.storeStone();

    // Restore stone data
    QXmlStreamReader streamReader(testString);
    streamReader.readNextStartElement();
    QCOMPARE(streamReader.name().toString(), QString("stone"));
    streamReader >> &testStone;

    QCOMPARE(testStone.pos().x(), 20);
    QCOMPARE(testStone.pos().y(), 200);
    QCOMPARE(testStone.getParent(), Gamestone::StoneParent::HEAP);

    testStone.restoreStone();

    QCOMPARE(testStone.pos().x(), 10);
    QCOMPARE(testStone.pos().y(), 100);
    QCOMPARE(testStone.getParent(), Gamestone::StoneParent::BOARD);

    if (streamWriter.hasError())
    {
        QFAIL(qUtf8Printable(streamWriter.device()->errorString()));
    }
    if (streamReader.hasError())
    {
        QFAIL(qUtf8Printable(streamReader.errorString()));
    }
}

QTEST_MAIN(GamestoneTest)
