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
 * JokerDragNDropTest.cpp
 *
 *  Created on: 18.12.2015
 *      Author: Felix Wohlfrom
 */

#include "JokerDragNDropTest.h"

#include <sstream>

#include <QMimeData>
#include <QDragEnterEvent>

#include "../../../main/game/Settings.h"

void JokerDragNDropTest::initTestCase()
{
    manager = new StoneManager();
}

void JokerDragNDropTest::cleanupTestCase()
{
    delete manager;
    Settings::cleanup();
}

void JokerDragNDropTest::singleJoker_SingleStone()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    // Dragging joker over normal stone
    testDragAndDrop(dragSource, dragTarget, true);
    // Dragging normal stone over joker
    testDragAndDrop(dragTarget, dragSource, true);
}

void JokerDragNDropTest::singleJoker_SingleJoker()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Dragging joker over another one
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragTarget, dragSource, true);
}

void JokerDragNDropTest::singleJoker_multipleStones()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, manager, true, Gamestone::YELLOW, 1);
    dragTarget.setNext(&thirdStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, thirdStone, true);

    // Dragging target stones over source.
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(thirdStone, dragSource, true);
}

void JokerDragNDropTest::singleJoker_rowWithJokerSingleStone()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    dragTarget.setNext(&thirdStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, thirdStone, true);

    // Dragging target stones over source.
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(thirdStone, dragSource, true);
}

void JokerDragNDropTest::singleJoker_rowWithJoker_sameNumberDifferentColor()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // We have two stones appended to the target
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLUE, 1);
    dragTarget.setNext(&thirdStone);
    thirdStone.setNext(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, thirdStone, false);
    testDragAndDrop(dragSource, fourthStone, true);

    // Dragging target stones over source. The middle stone can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(fourthStone, dragSource, true);

    // Now we add another stone, so no more joker can be added
    Gamestone anotherOne(NULL, manager, false, Gamestone::YELLOW, 1);
    fourthStone.setNext(&anotherOne);

    // Dragging target stones over source.
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, thirdStone, false);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(dragSource, anotherOne, false);

    // Dragging target stones over source. The middle stones can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(anotherOne, dragSource, false);
}

void JokerDragNDropTest::singleJoker_rowWithJoker_sameNumberDifferentColor_RotateColors()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // We have two stones appended to the target
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLUE, 1);
    Gamestone fifthStone(NULL, manager, false, Gamestone::YELLOW, 1);
    Gamestone sixthStone(NULL, manager, false, Gamestone::RED, 1);

    // Create a row and test dragging. It should fail all times.
    dragTarget.setNext(&thirdStone);
    thirdStone.setNext(&fourthStone);
    fourthStone.setNext(&fifthStone);
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(fifthStone, dragSource, false);
    testDragAndDrop(dragSource, fifthStone, false);

    // A second time...
    thirdStone.setNext(NULL);
    dragTarget.setNext(&fourthStone);
    fourthStone.setNext(&fifthStone);
    fifthStone.setNext(&sixthStone);
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(sixthStone, dragSource, false);
    testDragAndDrop(dragSource, sixthStone, false);

    // A third time...
    fourthStone.setNext(NULL);
    dragTarget.setNext(&fifthStone);
    fifthStone.setNext(&sixthStone);
    sixthStone.setNext(&thirdStone);
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(thirdStone, dragSource, false);
    testDragAndDrop(dragSource, thirdStone, false);

    // ... and a final time.
    fifthStone.setNext(NULL);
    dragTarget.setNext(&sixthStone);
    sixthStone.setNext(&thirdStone);
    thirdStone.setNext(&fourthStone);
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(fourthStone, dragSource, false);
    testDragAndDrop(dragSource, fourthStone, false);
}

void JokerDragNDropTest::singleJoker_rowWithJoker_sameColorDifferentNumbers()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Verify, that appending to the first stone doesn't work if the first
    // stone is a 1
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 2);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLACK, 3);
    dragTarget.setNext(&thirdStone);
    thirdStone.setNext(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, thirdStone, false);
    testDragAndDrop(dragSource, fourthStone, true);

    // Dragging target stones over source. The middle stone can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, true);

    // Now we have a row of joker -> black 3 -> black 4, so dragging a
    // joker over it should be ok
    Gamestone fifthStone(NULL, manager, false, Gamestone::BLACK, 4);
    dragTarget.setNext(&fourthStone);
    fourthStone.setNext(&fifthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(dragSource, fifthStone, true);

    // Dragging target stones over source. The middle stone can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(fifthStone, dragSource, true);

    // Verify that appending does work if it is not a 1 and appending to the
    // end does not work if it is a 13
    Gamestone sixthStone(NULL, manager, false, Gamestone::BLACK, 12);
    Gamestone seventhStone(NULL, manager, false, Gamestone::BLACK, 13);
    dragTarget.setNext(&sixthStone);
    sixthStone.setNext(&seventhStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, sixthStone, false);
    testDragAndDrop(dragSource, seventhStone, false);

    // Dragging target stones over source. The middle stone can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(seventhStone, dragSource, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameNumberDifferentColor_JokerIsOne()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Verify, that appending to the first stone doesn't work if the first
    // stone is a 1
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLUE, 1);
    dragSource.setNext(&thirdStone);
    dragTarget.setNext(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, true);
    testDragAndDrop(thirdStone, dragTarget, true);
    testDragAndDrop(thirdStone, fourthStone, false);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, true);
    testDragAndDrop(dragTarget, thirdStone, true);
    testDragAndDrop(fourthStone, thirdStone, false);

    // Now we add another stone, so dragging should not be accepted anymore
    Gamestone anotherOne(NULL, manager, false, Gamestone::YELLOW, 1);
    fourthStone.setNext(&anotherOne);

    // Drag all stones from source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(dragSource, anotherOne, false);
    testDragAndDrop(thirdStone, dragTarget, false);
    testDragAndDrop(thirdStone, fourthStone, false);
    testDragAndDrop(thirdStone, anotherOne, false);

    // Drag stones from target over source. Middle stone can't be dragged
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, false);
    testDragAndDrop(dragTarget, thirdStone, false);
    testDragAndDrop(anotherOne, thirdStone, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameNumberDifferentColor_JokerIsThirteen()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Verify, that appending to the last stone doesn't work if the last
    // stone is a 13
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 13);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLUE, 13);
    dragSource.setPrev(&thirdStone);
    dragTarget.setPrev(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, true);
    testDragAndDrop(thirdStone, dragTarget, true);
    testDragAndDrop(thirdStone, fourthStone, false);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, true);
    testDragAndDrop(dragTarget, thirdStone, true);
    testDragAndDrop(fourthStone, thirdStone, false);

    // Now we add another stone, so dragging should not be accepted anymore
    Gamestone anotherOne(NULL, manager, false, Gamestone::YELLOW, 13);
    fourthStone.setPrev(&anotherOne);

    // Drag all stones from source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(dragSource, anotherOne, false);
    testDragAndDrop(thirdStone, dragTarget, false);
    testDragAndDrop(thirdStone, fourthStone, false);
    testDragAndDrop(thirdStone, anotherOne, false);

    // Drag stones from target over source. Middle stone can't be dragged
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, false);
    testDragAndDrop(dragTarget, thirdStone, false);
    testDragAndDrop(anotherOne, thirdStone, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverStone1()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Some stones to append to the joker. Joker are now black 1 and black 3
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 2);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLACK, 4);
    dragSource.setNext(&thirdStone);
    dragTarget.setNext(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(thirdStone, dragTarget, true);
    testDragAndDrop(thirdStone, fourthStone, false);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, false);
    testDragAndDrop(dragTarget, thirdStone, true);
    testDragAndDrop(fourthStone, thirdStone, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverStone2()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Some stones to append to the joker. Joker are now black 2 and black 4
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLACK, 3);
    dragSource.setPrev(&thirdStone);
    dragTarget.setPrev(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, true);
    testDragAndDrop(thirdStone, dragTarget, false);
    testDragAndDrop(thirdStone, fourthStone, false);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, true);
    testDragAndDrop(dragTarget, thirdStone, false);
    testDragAndDrop(fourthStone, thirdStone, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverJoker1()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Some stones to append to the joker. Joker are now black 2 and black 3
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLACK, 4);
    dragSource.setPrev(&thirdStone);
    dragTarget.setNext(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(thirdStone, dragTarget, false);
    testDragAndDrop(thirdStone, fourthStone, false);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(fourthStone, dragSource, false);
    testDragAndDrop(dragTarget, thirdStone, false);
    testDragAndDrop(fourthStone, thirdStone, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverJoker2()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Some stones to append to the joker. Joker are now black 3 and black 4
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLACK, 2);
    Gamestone fifthStone(NULL, manager, false, Gamestone::BLACK, 5);
    Gamestone sixthStone(NULL, manager, false, Gamestone::BLACK, 6);
    fourthStone.setPrev(&thirdStone);
    dragSource.setPrev(&fourthStone);
    fifthStone.setNext(&sixthStone);
    dragTarget.setNext(&fifthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, fifthStone, false);
    testDragAndDrop(dragSource, sixthStone, false);
    testDragAndDrop(thirdStone, dragTarget, false);
    testDragAndDrop(thirdStone, fifthStone, false);
    testDragAndDrop(thirdStone, sixthStone, false);
    testDragAndDrop(fourthStone, dragTarget, false);
    testDragAndDrop(fourthStone, fifthStone, false);
    testDragAndDrop(fourthStone, sixthStone, false);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, true);
    testDragAndDrop(fifthStone, dragSource, false);
    testDragAndDrop(sixthStone, dragSource, false);
    testDragAndDrop(dragTarget, thirdStone, false);
    testDragAndDrop(fifthStone, thirdStone, false);
    testDragAndDrop(sixthStone, thirdStone, false);
    testDragAndDrop(dragTarget, fourthStone, false);
    testDragAndDrop(fifthStone, fourthStone, false);
    testDragAndDrop(sixthStone, fourthStone, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers_StoneOverStone()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, manager, true);
    Joker dragTarget(NULL, manager, false);

    // Some stones to append to the joker. Joker are now black 1 and black 4
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 2);
    Gamestone fourthStone(NULL, manager, false, Gamestone::BLACK, 3);
    dragSource.setNext(&thirdStone);
    dragTarget.setPrev(&fourthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, false);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(thirdStone, dragTarget, false);
    testDragAndDrop(thirdStone, fourthStone, true);

    // Dragging target stones over source
    testDragAndDrop(dragTarget, dragSource, false);
    testDragAndDrop(fourthStone, dragSource, false);
    testDragAndDrop(dragTarget, thirdStone, false);
    testDragAndDrop(fourthStone, thirdStone, true);
}

void JokerDragNDropTest::singleStone_rowWithJoker_sameNumberDifferentColor()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLUE, 1);
    Joker dragTarget(NULL, manager, true);

    // Some other stones in the same row with joker
    Gamestone thirdStone(NULL, manager, true, Gamestone::BLUE, 2);
    Gamestone fourthStone(NULL, manager, true, Gamestone::BLACK, 2);

    dragTarget.setNext(&thirdStone);
    thirdStone.setNext(&fourthStone);

    // Verify that dragging the source over the joker fails
    testDragAndDrop(dragSource, dragTarget, false);
}

void JokerDragNDropTest::singleStone_rowWithTwoJoker_sameColorWithDifferentNumbers()
{
    // Initialize the two joker
    Joker firstJoker(NULL, manager, true);
    Joker secondJoker(NULL, manager, false);

    // Two additional stones to use for tests
    Gamestone thirdStone(NULL, manager, true, Gamestone::BLUE, 2);
    Gamestone fourthStone(NULL, manager, true, Gamestone::BLUE, 5);

    // Build a row with two joker and the 5 at the end
    firstJoker.setNext(&secondJoker);
    secondJoker.setNext(&fourthStone);

    // Now try to drag the 2 stone over the first joker, which should be possible
    testDragAndDrop(thirdStone, firstJoker, true);
    testDragAndDrop(firstJoker, thirdStone, true);

    // Build a row with two joker and the 2 at the beginning
    thirdStone.setNext(&firstJoker);
    secondJoker.setNext(NULL);

    // Now try to drag the 5 stone over the second joker, which should be possible
    testDragAndDrop(fourthStone, secondJoker, true);
    testDragAndDrop(secondJoker, fourthStone, true);
}

void JokerDragNDropTest::testDragAndDrop(Gamestone& source, Gamestone& target,
        bool shouldAccept)
{
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(GAMESTONE_MIMETYPE, NULL);

    QDragEnterEvent* event = new QDragEnterEvent(
            target.pos()
                    + 0.5
                            * QPoint(target.size().width(),
                                    target.size().height()),
            Qt::MoveAction | Qt::CopyAction, mimeData, Qt::LeftButton,
            Qt::NoModifier);

    manager->draggedStone = &source;
    target.dragEnterEvent(event);

    // Verify testcase
    std::stringstream failureMessage;
    failureMessage << "Stone " << &source << " dragged over " << &target
                        << " -> should accept " << shouldAccept << ", but was "
                        << event->isAccepted();
    QVERIFY2(event->isAccepted() == shouldAccept, failureMessage.str().c_str());

    delete event;
    delete mimeData;
}

QTEST_MAIN(JokerDragNDropTest)
