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
 * GamestoneDragNDropTest.h
 *
 *  Created on: 12.12.2015
 *      Author: Felix Wohlfrom
 */

#ifndef GAME_STONES_GAMESTONEDRAGNDROPTEST_H_
#define GAME_STONES_GAMESTONEDRAGNDROPTEST_H_

#include <QTest>

#include "../../../main/game/stones/StoneManager.h"

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
 * These test veriy that dragging a stone over another is working correctly.
 * This means that the checks if a stone can be dragged over another
 * is working correctly.
 */
class GamestoneDragNDropTest: public QObject
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
         * This testcase verifies, that a single stone can be dragged
         * over another stone with same number but different color.
         * Should be accepted.
         */
        void singleStone_SingleStone_SameNumber_DifferentColor();

        /**
         * @test
         * This testcase verifies, that a single stone can not be dragged
         * over another stone with same number and same color.
         * Should not be accepted.
         */
        void singleStone_SingleStone_SameNumber_SameColor();

        /**
         * @test
         * Tests dragging of a single stone over two other stones with different
         * colors. Should be accepted.
         */
        void singleStone_MultiStone_SameNumber_DifferentColor();

        /**
         * @test
         * Tests dragging of a single stone over two other stones with the source
         * color already existing in target colors. Should not be accepted.
         */
        void singleStone_SingleStone_SameNumber_ColorAlreadyExists();

        /**
         * @test
         * Tests dragging of two stones over a single stone with all three different
         * colors. Should be accepted.
         */
        void multiStone_singleStone_SameNumber_DifferentColor();

        /**
         * @test
         * Tests dragging of two stones over a single stone with target color already
         * existing in source colors. Should not be accepted.
         */
        void multiStone_singleStone_SameNumber_ColorAlreadyExists();
};

#endif /* GAME_STONES_GAMESTONEDRAGNDROPTEST_H_ */
