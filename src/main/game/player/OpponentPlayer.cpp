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
 * OpponentPlayer.cpp
 *
 *  Created on: 08.03.2012
 *      Author: felix
 */

#include "OpponentPlayer.h"

OpponentPlayer::OpponentPlayer(QString name, StoneManager& stoneManager,
        int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow) :
    Player(name, stoneManager, initialStonesLeftToTake, sumInFirstRound, stonesInOneRow),
    stopPlayerPlaying(false)
{}

OpponentPlayer::~OpponentPlayer() {}

void OpponentPlayer::stopPlaying()
{
    stopPlayerPlaying = true;
}
