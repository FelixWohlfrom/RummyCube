/*
 * GamestoneDragNDropTest.h
 *
 *  Created on: 12.12.2015
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_STONES_JOKERDRAGNDROPTEST_H_
#define GAME_STONES_JOKERDRAGNDROPTEST_H_

#include <QTest>

#include "../../../main/game/stones/StoneManager.h"
#include "../../../main/game/stones/Joker.h"

#ifdef _DEBUG
    #ifdef _WIN32
        #ifdef _MSC_VER
             // Uncomment this if you installed visual memory
             // leak detector from http://vld.codeplex.com/
            #include <vld.h>
        #endif
    #endif
#endif

/**
 * These test verify that dragging a stone over another is working correctly.
 * This means that the checks if a stone can be dragged over another
 * is working correctly.
 */
class JokerDragNDropTest: public QObject
{
    Q_OBJECT

    private:
        /**
         * The stone manager for the gamestones.
         */
        StoneManager* manager;

        /**
         * Helper function that executes the drag'n'drop test. It creates the
         * drag'n'drop event, executes it and verifies the result.
         *
         * @param source The source stone that is dragged
         * @param target The target stone on which the source should be dropped
         * @param shouldAccept If dropping should be accepted or not
         */
        void testDragAndDrop(Gamestone& source, Gamestone& target,
                bool shouldAccept);

    private slots:
        /**
         * Initializes the stone manager
         */
        void initTestCase();

        /**
         * Cleans up the settings object.
         */
        void cleanupTestCase();

        /**
         * @test
         * This testcase verifies that dragging a stone over a joker and vice
         * versa works.
         */
        void singleJoker_SingleStone();

        /**
         * @test
         * This testcase verifies that a joker can be dragged over another joker.
         */
        void singleJoker_SingleJoker();

        /**
         * @test
         * Tests dragging of a single joker over a row with multiple stones and
         * vice versa works correctly.
         */
        void singleJoker_multipleStones();

        /**
         * @test
         * Tests dragging of a single joker over a row containing a joker works
         * is accepted correctly.
         */
        void singleJoker_rowWithJokerSingleStone();

        /**
         * @test
         * Tests dragging a single joker over a row containing a joker with 
         * multiple stones, all with different colors is accepted correctly.
         */
        void singleJoker_rowWithJoker_sameNumberDifferentColor();

        /**
         * @test
         * Tests dragging a single joker over a row containing a joker with 
         * multiple stones, all with same colors and ascending numbers is 
         * accepted correctly.
         */
        void singleJoker_rowWithJoker_sameColorDifferentNumbers();

        /**
         * @test
         * Tests dragging a row containing multiple stones containing a joker
         * over a row containing a joker with multiple stones, all with different
         * colors is accepted correctly.
         */
        void rowWithJoker_rowWithJoker_sameNumberDifferentColor();

        /**
         * @test
         * Tests dragging a row containing multiple stones containing a joker
         * over a row containing a joker with multiple stones, with same colors
         * and ascending numbers is accepted correctly. The following combination is tested:
         * Joker -> Black 2 combined with Joker -> Black 4
         */
        void rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverStone1();

        /**
         * @test
         * Tests dragging a row containing multiple stones containing a joker
         * over a row containing a joker with multiple stones, with same colors
         * and ascending numbers is accepted correctly. The following combination is tested:
         * Black 1 -> Joker combined with Black 3 -> Joker
         */
        void rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverStone2();

        /**
         * @test
         * Tests dragging a row containing multiple stones containing a joker
         * over a row containing a joker with multiple stones, with same colors
         * and ascending numbers is accepted correctly. The following combination is tested:
         * Black 1 -> Joker combined with Joker -> Black 4
         */
        void rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverJoker1();

        /**
         * @test
         * Tests dragging a row containing multiple stones containing a joker
         * over a row containing a joker with multiple stones, with same colors
         * and ascending numbers is accepted correctly. The following combination is tested:
         * Black 1 -> Black 2 -> Joker combined with Joker -> Black 5 -> Black 6
         */
        void rowWithJoker_rowWithJoker_sameColorDifferentNumbers_JokerOverJoker2();

        /**
         * @test
         * Tests dragging a row containing multiple stones containing a joker
         * over a row containing a joker with multiple stones, with same colors
         * and ascending numbers is accepted correctly. The following combination is tested:
         * Joker -> Black 2 combined with Black 3 -> Joker
         */
        void rowWithJoker_rowWithJoker_sameColorDifferentNumbers_StoneOverStone();

        /**
         * @test
         * Tests the following scenario:
         * Two stones with same number, different color. One joker added to left of
         * this row. Now a stone with successing number, but same color of stone
         * right of joker is dragged over joker. This should fail.
         */
        void singleStone_rowWithJoker_sameNumberDifferentColor();
};

#endif /* GAME_STONES_JOKERDRAGNDROPTEST_H_ */
