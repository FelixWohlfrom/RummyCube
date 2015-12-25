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

void GamestoneDragNDropTest::singleStone_SingleStone_SameNumber_DifferentColor()
{
	// Initialize the two stones to verify drag'n'drop
	Gamestone dragSource(NULL, &manager, true, Gamestone::BLACK, 1);
	Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

	testDragAndDrop(dragSource, dragTarget, true);
}

void GamestoneDragNDropTest::singleStone_SingleStone_SameNumber_SameColor()
{
	// Initialize the two stones to verify drag'n'drop
	Gamestone dragSource(NULL, &manager, true, Gamestone::BLACK, 1);
	Gamestone dragTarget(NULL, &manager, false, Gamestone::BLACK, 1);

	testDragAndDrop(dragSource, dragTarget, false);
}

void GamestoneDragNDropTest::singleStone_MultiStone_SameNumber_DifferentColor()
{
	// Initialize the two stones to verify drag'n'drop
	Gamestone dragSource(NULL, &manager, true, Gamestone::BLACK, 1);
	Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

	// We have a third stone appended to the target
	Gamestone thirdStone(NULL, &manager, true, Gamestone::YELLOW, 1);
	dragTarget.setNext(&thirdStone);

	// Drag over both stones and check if accepting is working correctly
	testDragAndDrop(dragSource, dragTarget, true);
	testDragAndDrop(dragSource, thirdStone, true);
}

void GamestoneDragNDropTest::singleStone_SingleStone_SameNumber_ColorAlreadyExists()
{
	// Initialize the two stones to verify drag'n'drop
	Gamestone dragSource(NULL, &manager, true, Gamestone::BLACK, 1);
	Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

	// We have a third stone appended to the target
	Gamestone thirdStone(NULL, &manager, false, Gamestone::BLACK, 1);
	dragTarget.setNext(&thirdStone);

	testDragAndDrop(dragSource, dragTarget, false);
}

void GamestoneDragNDropTest::multiStone_singleStone_SameNumber_DifferentColor()
{
	// Initialize the two stones to verify drag'n'drop
	Gamestone dragSource(NULL, &manager, true, Gamestone::BLACK, 1);
	Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

	// We have a third stone appended to the target
	Gamestone thirdStone(NULL, &manager, true, Gamestone::YELLOW, 1);
	dragSource.setNext(&thirdStone);

	// Drag both stones and check if accepting is working correctly
	testDragAndDrop(dragSource, dragTarget, true);
	testDragAndDrop(thirdStone, dragTarget, true);
}

void GamestoneDragNDropTest::multiStone_singleStone_SameNumber_ColorAlreadyExists()
{
	// Initialize the two stones to verify drag'n'drop
	Gamestone dragSource(NULL, &manager, true, Gamestone::BLACK, 1);
	Gamestone dragTarget(NULL, &manager, true, Gamestone::BLUE, 1);

	// We have a third stone appended to the target
	Gamestone thirdStone(NULL, &manager, false, Gamestone::BLUE, 1);
	dragSource.setNext(&thirdStone);

	// Drag both stones and check if accepting is working correctly
	testDragAndDrop(dragSource, dragTarget, false);
	testDragAndDrop(thirdStone, dragTarget, false);
}

void GamestoneDragNDropTest::testDragAndDrop(Gamestone& source, Gamestone& target,
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

QTEST_MAIN(GamestoneDragNDropTest)
