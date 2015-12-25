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

	// Drag joker over stone BLUE - 1 with successor YELLOW - 1
	testDragAndDrop(dragSource, dragTarget, true);
	// Drag joker over stone YELLOW - 1 with predecessor BLUE - 1
	testDragAndDrop(dragSource, thirdStone, true);

	// Drag the stones over the joker
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

	testDragAndDrop(dragSource, dragTarget, true);
	testDragAndDrop(dragSource, thirdStone, true);
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

	testDragAndDrop(dragSource, dragTarget, true);
	testDragAndDrop(dragSource, thirdStone, false);
	testDragAndDrop(dragSource, fourthStone, true);
	testDragAndDrop(dragTarget, dragSource, true);
	testDragAndDrop(thirdStone, dragSource, false);
	testDragAndDrop(fourthStone, dragSource, true);

	// Now we add another stone, so no more joker can be added
	Gamestone anotherOne(NULL, &manager, false, Gamestone::YELLOW, 1);
	fourthStone.setNext(&anotherOne);

	testDragAndDrop(dragSource, dragTarget, false);
	testDragAndDrop(dragSource, anotherOne, false);
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

	testDragAndDrop(dragSource, dragTarget, false);
	testDragAndDrop(dragSource, thirdStone, false);
	testDragAndDrop(dragSource, fourthStone, true);
	testDragAndDrop(dragTarget, dragSource, false);
	testDragAndDrop(thirdStone, dragSource, false);
	testDragAndDrop(fourthStone, dragSource, true);

	// Verify, that appending to the first stone does work if it is not a
	// 1 and appending to the end works if it is no 13
	Gamestone fifthStone(NULL, &manager, false, Gamestone::BLACK, 4);
	dragTarget.setNext(&fourthStone);
	fourthStone.setNext(&fifthStone);

	testDragAndDrop(dragSource, dragTarget, true);
	testDragAndDrop(dragSource, fourthStone, false);
	testDragAndDrop(dragSource, fifthStone, true);
	testDragAndDrop(dragTarget, dragSource, false); // TODO This should be true
	testDragAndDrop(fourthStone, dragSource, false);
	testDragAndDrop(fifthStone, dragSource, true);

	// Verify that appending does work if it is not a 1 and appending to the
	// end does not work if it is a 13
	Gamestone sixthStone(NULL, &manager, false, Gamestone::BLACK, 12);
	Gamestone seventhStone(NULL, &manager, false, Gamestone::BLACK, 13);
	dragTarget.setNext(&sixthStone);
	sixthStone.setNext(&seventhStone);

	testDragAndDrop(dragSource, dragTarget, true);
	testDragAndDrop(dragSource, sixthStone, false);
	testDragAndDrop(dragSource, seventhStone, false);
	testDragAndDrop(dragTarget, dragSource, false); // TODO This should be true
	testDragAndDrop(sixthStone, dragSource, false);
	testDragAndDrop(seventhStone, dragSource, false);
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameNumberDifferentColor()
{
}

void JokerDragNDropTest::rowWithJoker_rowWithJoker_sameColorDifferentNumbers()
{
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
