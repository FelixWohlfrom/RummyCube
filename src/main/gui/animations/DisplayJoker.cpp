/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2017 Felix Wohlfrom                                    *
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
 * DisplayStone.cpp
 *
 *  Created on: 21.04.2017
 *      Author: Felix Wohlfrom
 */

#include "DisplayJoker.h"

#include "../game/stones/Joker.h"

DisplayJoker::DisplayJoker(QWidget* parent):
    Animation(parent, 10)
{
    stoneToDisplay = new Joker(parent, &manager, 1);
    stoneToDisplay->setParent(Gamestone::StoneParent::HOLDER);
    QSize size = parent->size();
    stoneToDisplay->move(size.width() / 2, size.height() / 2);
}

DisplayJoker::~DisplayJoker()
{
	stoneToDisplay->hide();
    delete stoneToDisplay;
}

void DisplayJoker::animate()
{}
