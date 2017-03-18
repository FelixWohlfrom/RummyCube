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
 * JokerTest.cpp
 *
 *  Created on: 25.02.2017
 *      Author: Felix Wohlfrom
 */

#include "JokerTest.h"

#include <QTest>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "../../../main/game/Settings.h"
#include "../../../main/game/stones/Joker.h"
#include "../../../main/game/stones/StoneManager.h"
#include "../../../main/game/stones/ParentChangedEvent.h"

// We need the meta type to use stone parent enum as data generator
Q_DECLARE_METATYPE(Gamestone::StoneParent);

void JokerTest::cleanupTestCase()
{
    Settings::cleanup();
}

void JokerTest::defaultConstructorTest()
{
    Joker* testStone = new Joker();

    QVERIFY2(testStone != NULL, "Joker not created");
    QCOMPARE(testStone->isInvalid(), false);
    QCOMPARE(testStone->isFirst(), false);
    QCOMPARE(testStone->getColor(), Gamestone::StoneColor::UNKNOWN);
    QCOMPARE(testStone->getNumber(), -1);
    QVERIFY2(testStone->parent() == NULL,
                 "Parent widget set although none expected: " + testStone->getParent());
    QCOMPARE(testStone->getParent(), Gamestone::StoneParent::HEAP);
    QVERIFY2(testStone->getPlayer() == NULL, "Player set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Previous stone set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Next stone set although none expected.");
    QCOMPARE(testStone->isJoker(), true);
    QCOMPARE(testStone->isFirstTimePlayedOut(), true);

    delete testStone;
}

void JokerTest::constructorTest()
{
    StoneManager stoneManager;
    QWidget parent;
    Joker* testStone = new Joker(&parent, &stoneManager, true);

    QVERIFY2(testStone != NULL, "Gamestone not created");
    QCOMPARE(testStone->isInvalid(), false);
    QCOMPARE(testStone->isFirst(), true);
    QCOMPARE(testStone->getColor(), Gamestone::StoneColor::UNKNOWN);
    QCOMPARE(testStone->getNumber(), -1);
    QCOMPARE(testStone->parent(), &parent);
    QCOMPARE(testStone->getParent(), Gamestone::StoneParent::HEAP);
    QVERIFY2(testStone->getPlayer() == NULL, "Player set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Previous stone set although none expected.");
    QVERIFY2(testStone->getPrev() == NULL, "Next stone set although none expected.");
    QCOMPARE(testStone->isJoker(), true);
    QCOMPARE(testStone->isFirstTimePlayedOut(), true);

    delete testStone;
}

void JokerTest::testInvalidation()
{
    Joker testStone;
    testStone.setInvalid(true);
    QCOMPARE(testStone.isInvalid(), true);
    testStone.setInvalid(false);
    QCOMPARE(testStone.isInvalid(), false);
}

void JokerTest::testParentSwitching_data()
{
    QTest::addColumn<Gamestone::StoneParent>("stoneParent");
    QTest::addColumn<bool>("invalid");

    QTest::newRow("Holder") << Gamestone::StoneParent::HOLDER << false;
    QTest::newRow("AIHolder") << Gamestone::StoneParent::AIHOLDER << true;
    QTest::newRow("Heap") << Gamestone::StoneParent::HEAP << true;
    QTest::newRow("Board") << Gamestone::StoneParent::BOARD << true;
}

void JokerTest::testParentSwitching()
{
    QFETCH(Gamestone::StoneParent, stoneParent);
    QFETCH(bool, invalid);

    Joker testStone;

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

void JokerTest::testStoneStoringAndRestoring()
{
    StoneManager stoneManager;
    QWidget parent;
    Joker testStone(&parent, &stoneManager, true);

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

    ((Gamestone&)testStone).storeStone(Gamestone::StoneParent::HOLDER);

    testStone.move(20, 200);
    testStone.setParent(Gamestone::StoneParent::HEAP);

    testStone.restoreStone();

    QCOMPARE(testStone.pos().x(), 10);
    QCOMPARE(testStone.pos().y(), 100);
    QCOMPARE(testStone.getParent(), Gamestone::StoneParent::HOLDER);
}

void JokerTest::testStoneStoringAndRestoringFromStream()
{
    StoneManager stoneManager;
    QWidget parent;
    Joker testStone(&parent, &stoneManager, true);

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
    QCOMPARE(streamReader.name().toString(), QString("joker"));
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

QTEST_MAIN(JokerTest)
