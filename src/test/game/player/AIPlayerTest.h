/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2018 Felix Wohlfrom                                    *
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
 * AIPlayerTest.h
 *
 *  Created on: 30.10.2018
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_TEST_GAME_PLAYER_AIPLAYERTEST_H_
#define SRC_TEST_GAME_PLAYER_AIPLAYERTEST_H_

#include <QObject>

/**
 * This class verifies that the ai player works fine.
 */
class AIPlayerTest: public QObject
{
     Q_OBJECT

     private slots:
         /**
          * @test
          * Tests if the constructor with parameters works fine.
          */
         void constructorTest();

         /**
          * @test
          * Verifies that the ai player can play against itself.
          */
         void verifyPlaying();
};

#endif /* SRC_TEST_GAME_PLAYER_AIPLAYERTEST_H_ */
