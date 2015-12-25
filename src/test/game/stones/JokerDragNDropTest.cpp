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

void JokerDragNDropTest::singleJoker_SingleStone()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, &manager, true);
    Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

    // Dragging joker over normal stone
    testDragAndDrop(dragSource, dragTarget, true);
    // Dragging normal stone over joker
    testDragAndDrop(dragTarget, dragSource, true);
}

void JokerDragNDropTest::singleJoker_SingleJoker()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, &manager, true);
    Joker dragTarget(NULL, &manager, false);

    // Dragging joker over another one
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragTarget, dragSource, true);
}

void JokerDragNDropTest::singleJoker_multipleStones()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, &manager, true);
    Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, &manager, true, Gamestone::YELLOW, 1);
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
    Joker dragSource(NULL, &manager, true);
    Joker dragTarget(NULL, &manager, false);

    // We have a third stone appended to the target
    Gamestone thirdStone(NULL, &manager, false, Gamestone::BLACK, 1);
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
    Joker dragSource(NULL, &manager, true);
    Joker dragTarget(NULL, &manager, false);

    // We have two stones appended to the target
    Gamestone thirdStone(NULL, &manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, &manager, false, Gamestone::BLUE, 1);
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
    Gamestone anotherOne(NULL, &manager, false, Gamestone::YELLOW, 1);
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

void JokerDragNDropTest::singleJoker_rowWithJoker_sameColorDifferentNumbers()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, &manager, true);
    Joker dragTarget(NULL, &manager, false);

    // Verify, that appending to the first stone doesn't work if the first
    // stone is a 1
    Gamestone thirdStone(NULL, &manager, false, Gamestone::BLACK, 2);
    Gamestone fourthStone(NULL, &manager, false, Gamestone::BLACK, 3);
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

    // Verify, that appending to the first stone does work if it is not a
    // 1 and appending to the end works if it is no 13
    Gamestone fifthStone(NULL, &manager, false, Gamestone::BLACK, 4);
    dragTarget.setNext(&fourthStone);
    fourthStone.setNext(&fifthStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, fourthStone, false);
    testDragAndDrop(dragSource, fifthStone, true);

    // Dragging target stones over source. The middle stone can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, false); // TODO This should be true
    testDragAndDrop(fifthStone, dragSource, true);

    // Verify that appending does work if it is not a 1 and appending to the
    // end does not work if it is a 13
    Gamestone sixthStone(NULL, &manager, false, Gamestone::BLACK, 12);
    Gamestone seventhStone(NULL, &manager, false, Gamestone::BLACK, 13);
    dragTarget.setNext(&sixthStone);
    sixthStone.setNext(&seventhStone);

    // Dragging source over target
    testDragAndDrop(dragSource, dragTarget, true);
    testDragAndDrop(dragSource, sixthStone, false);
    testDragAndDrop(dragSource, seventhStone, false);

    // Dragging target stones over source. The middle stone can not be
    // dragged independently
    testDragAndDrop(dragTarget, dragSource, false); // TODO This should be true
    testDragAndDrop(seventhStone, dragSource, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameNumberDifferentColor()
{
    // Initialize the two stones to verify drag'n'drop
    Joker dragSource(NULL, &manager, true);
    Joker dragTarget(NULL, &manager, false);

    // Verify, that appending to the first stone doesn't work if the first
    // stone is a 1
    Gamestone thirdStone(NULL, &manager, false, Gamestone::BLACK, 1);
    Gamestone fourthStone(NULL, &manager, false, Gamestone::BLUE, 1);
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
    Gamestone anotherOne(NULL, &manager, false, Gamestone::YELLOW, 1);
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

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers()
{
    // TODO Implement
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

    manager.draggedStone = &source;
    target.dragEnterEvent(event);

    // Verify testcase
    std::stringstream failureMessage;
    failureMessage << "Stone " << &source << " dragged over " << &target
                        << " -> should accept " << shouldAccept << ", but was "
                        << event->isAccepted();
    QVERIFY2(event->isAccepted() == shouldAccept, failureMessage.str().c_str());

    delete event;
}

QTEST_MAIN(JokerDragNDropTest)
