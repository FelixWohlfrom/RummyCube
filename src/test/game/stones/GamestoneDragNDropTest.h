/*
 * GamestoneDragNDropTest.h
 *
 *  Created on: 12.12.2015
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_STONES_GAMESTONEDRAGNDROPTEST_H_
#define GAME_STONES_GAMESTONEDRAGNDROPTEST_H_

#include <QTest>

#include "../../../main/game/stones/StoneManager.h"
#include "../../../main/game/stones/Gamestone.h"

/**
 * These test veriy that dragging a stone over another is working correctly.
 * This means that the checks if a stone can be dragged over another
 * is working correctly.
 */
class GamestoneDragNDropTest: public QObject
{
	Q_OBJECT

	private:
		/**
		 * The stone manager for the gamestones
		 */
		StoneManager manager;

		/**
		 * Helper function that executes the drag'n'drop test. It creates the
		 * drag'n'drop event, executes it and verifies the result.
		 */
		void testDragAndDrop(Gamestone& source, Gamestone& target,
				bool shouldAccept);

	private slots:
		/**
		 * This testcase verifies, that a single stone can be dragged
		 * over another stone with same number but different color.
		 * Should be accepted.
		 */
		void singleStone_SingleStone_SameNumber_DifferentColor();

		/**
		 * This testcase verifies, that a single stone can not be dragged
		 * over another stone with same number and same color.
		 * Should not be accepted.
		 */
		void singleStone_SingleStone_SameNumber_SameColor();

		/**
		 * Tests dragging of a single stone over two other stones with different
		 * colors. Should be accepted.
		 */
		void singleStone_MultiStone_SameNumber_DifferentColor();

		/**
		 * Tests dragging of a single stone over two other stones with the source
		 * color already existing in target colors. Should not be accepted.
		 */
		void singleStone_SingleStone_SameNumber_ColorAlreadyExists();

		/**
		 * Tests dragging of two stones over a single stone with all three different
		 * colors. Should be accepted.
		 */
		void multiStone_singleStone_SameNumber_DifferentColor();

		/**
		 * Tests dragging of two stones over a single stone with target color already
		 * existing in source colors. Should not be accepted.
		 */
		void multiStone_singleStone_SameNumber_ColorAlreadyExists();
};

#endif /* GAME_STONES_GAMESTONEDRAGNDROPTEST_H_ */
