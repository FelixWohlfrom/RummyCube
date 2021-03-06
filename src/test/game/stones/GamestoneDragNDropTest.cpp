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
 * GamestoneDragNDropTest.cpp
 *
 *  Created on: 12.12.2015
 *      Author: Felix Wohlfrom
 */

#include "GamestoneDragNDropTest.h"

#include <sstream>

#include <QMimeData>
#include <QDragEnterEvent>

#include "../../../main/game/Settings.h"

void GamestoneDragNDropTest::initTestCase()
{
    manager = new StoneManager();
}

void GamestoneDragNDropTest::cleanupTestCase()
{
    delete manager;
    Settings::cleanup();
}

void GamestoneDragNDropTest::testDragOverAcceptance(Gamestone& source, Gamestone& target,
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

void GamestoneDragNDropTest::singleStone_SingleStone_SameNumber_DifferentColor()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    testDragOverAcceptance(dragSource, dragTarget, true);
}

void GamestoneDragNDropTest::singleStone_SingleStone_SameNumber_SameColor()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dragTarget(NULL, manager, false, Gamestone::BLACK, 1);

    testDragOverAcceptance(dragSource, dragTarget, false);
}

void GamestoneDragNDropTest::singleStone_MultiStone_SameNumber_DifferentColor()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, manager, true, Gamestone::YELLOW, 1);
    dragTarget.setNext(&thirdStone);

    // Drag over both stones and check if accepting is working correctly
    testDragOverAcceptance(dragSource, dragTarget, true);
    testDragOverAcceptance(dragSource, thirdStone, true);
}

void GamestoneDragNDropTest::singleStone_SingleStone_SameNumber_ColorAlreadyExists()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLACK, 1);
    dragTarget.setNext(&thirdStone);

    testDragOverAcceptance(dragSource, dragTarget, false);
}

void GamestoneDragNDropTest::multiStone_singleStone_SameNumber_DifferentColor()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, manager, true, Gamestone::YELLOW, 1);
    dragSource.setNext(&thirdStone);

    // Drag both stones and check if accepting is working correctly
    testDragOverAcceptance(dragSource, dragTarget, true);
    testDragOverAcceptance(thirdStone, dragTarget, true);
}

void GamestoneDragNDropTest::multiStone_singleStone_SameNumber_ColorAlreadyExists()
{
    // Initialize the two stones to verify drag'n'drop
    Gamestone dragSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dragTarget(NULL, manager, true, Gamestone::BLUE, 1);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, manager, false, Gamestone::BLUE, 1);
    dragSource.setNext(&thirdStone);

    // Drag both stones and check if accepting is working correctly
    testDragOverAcceptance(dragSource, dragTarget, false);
    testDragOverAcceptance(thirdStone, dragTarget, false);
}

void GamestoneDragNDropTest::dropStone_Left_SingleStone()
{
    Gamestone dropSource(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dropTarget(NULL, manager, true, Gamestone::BLACK, 2);

    QVERIFY2(dropTarget.appendStone(dropSource), "Did not append stone although it should");

    QVERIFY2(dropTarget.getPrev() == &dropSource, "Did not drop stone on left side");
    QVERIFY2(dropSource.getNext() == &dropTarget, "Did not update drop source correctly");
}

void GamestoneDragNDropTest::dropStone_Left_MultipleStonesSource()
{
    Gamestone otherStone(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dropSource(NULL, manager, true, Gamestone::BLUE, 1);
    Gamestone dropTarget(NULL, manager, true, Gamestone::YELLOW, 1);

    dropSource.setPrev(&otherStone);

    QVERIFY2(dropTarget.appendStone(dropSource), "Did not append stone although it should");

    QVERIFY2(dropTarget.getPrev() == &dropSource, "Did not drop stone on left side");
    QVERIFY2(dropSource.getNext() == &dropTarget, "Did not update drop source correctly");
}

void GamestoneDragNDropTest::dropStone_Left_MultipleStonesTarget()
{
    Gamestone otherStone(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dropSource(NULL, manager, true, Gamestone::BLUE, 1);
    Gamestone dropTarget(NULL, manager, true, Gamestone::YELLOW, 1);

    dropTarget.setNext(&otherStone);

    QVERIFY2(dropTarget.appendStone(dropSource), "Did not append stone although it should");

    QVERIFY2(dropTarget.getPrev() == &dropSource, "Did not drop stone on left side");
    QVERIFY2(dropSource.getNext() == &dropTarget, "Did not update drop source correctly");
}

void GamestoneDragNDropTest::dropStone_Right_SingleStone()
{
    Gamestone dropSource(NULL, manager, true, Gamestone::BLACK, 2);
    Gamestone dropTarget(NULL, manager, true, Gamestone::BLACK, 1);

    dropTarget.appendStone(dropSource);

    QVERIFY2(dropTarget.getNext() == &dropSource, "Did not drop stone on right side");
    QVERIFY2(dropSource.getPrev() == &dropTarget, "Did not update drop source correctly");
}

void GamestoneDragNDropTest::dropStone_Right_MultipleStonesSource()
{
    Gamestone otherStone(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dropSource(NULL, manager, true, Gamestone::BLUE, 1);
    Gamestone dropTarget(NULL, manager, true, Gamestone::YELLOW, 1);

    dropSource.setNext(&otherStone);

    QVERIFY2(dropTarget.appendStone(dropSource), "Did not append stone although it should");

    QVERIFY2(dropTarget.getNext() == &dropSource, "Did not drop stone on left side");
    QVERIFY2(dropSource.getPrev() == &dropTarget, "Did not update drop source correctly");
}

void GamestoneDragNDropTest::dropStone_Right_MultipleStonesTarget()
{
    Gamestone otherStone(NULL, manager, true, Gamestone::BLACK, 1);
    Gamestone dropSource(NULL, manager, true, Gamestone::BLUE, 1);
    Gamestone dropTarget(NULL, manager, true, Gamestone::YELLOW, 1);

    dropTarget.setPrev(&otherStone);

    QVERIFY2(dropTarget.appendStone(dropSource), "Did not append stone although it should");

    QVERIFY2(dropTarget.getNext() == &dropSource, "Did not drop stone on left side");
    QVERIFY2(dropSource.getPrev() == &dropTarget, "Did not update drop source correctly");
}

QTEST_MAIN(GamestoneDragNDropTest)
