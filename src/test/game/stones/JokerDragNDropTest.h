/*
 * GamestoneDragNDropTest.h
 *
 *  Created on: 12.12.2015
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_STONES_JOKERDRAGNDROPTEST_H_
#define GAME_STONES_JOKERDRAGNDROPTEST_H_

#include <QTest>
//#include "../../Autotest.h"

#include "../../../main/game/stones/StoneManager.h"
#include "../../../main/game/stones/Joker.h"

/**
 * These test veriy that dragging a stone over another is working correctly.
 * This means that the checks if a stone can be dragged over another
 * is working correctly.
 */
class JokerDragNDropTest: public QObject
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
		 * This testcase verifies, that dragging a stone over a joker and vice
		 * versa works.
		 */
		void singleJoker_SingleStone();

		/**
		 * This testcase verifies, that a joker can be dragged over another joker.
		 */
		void singleJoker_SingleJoker();

		/**
		 * Tests dragging of a single joker over a row with multiple stones and
		 * vice versa works correctly.
		 */
		void singleJoker_multipleStones();

		/**
		 * Tests dragging of a single joker over a row containing a joker works
		 * is accepted correctly.
		 */
		void singleJoker_rowWithJokerSingleStone();

		/**
		 * Tests dragging a single joker over a row containing a joker with 
		 * multiple stones, all with different colors is accepted correctly.
		 */
		void singleJoker_rowWithJoker_sameNumberDifferentColor();

	    /**
		 * Tests dragging a single joker over a row containing a joker with 
		 * multiple stones, all with same colors and ascending numbers is 
		 * accepted correctly.
		 */
		void singleJoker_rowWithJoker_sameColorDifferentNumbers();

		/**
		 * Tests dragging a row containing multiple stones containing a joker
		 * over a row containing a joker with multiple stones, all with different
		 * colors is accepted correctly.
		 */
		void rowWithJoker_rowWithJoker_sameNumberDifferentColor();

		/**
		 * Tests dragging a row containing multiple stones containing a joker
		 * over a row containing a joker with multiple stones, with same colors
		 * and ascendingnumbers is accepted correctly.
		 */
		void rowWithJoker_rowWithJoker_sameColorDifferentNumbers();
};

#endif /* GAME_STONES_JOKERDRAGNDROPTEST_H_ */
