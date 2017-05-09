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
 * SameNumberDifferentColorJoker.cpp
 *
 *  Created on: 24.04.2017
 *      Author: Felix Wohlfrom
 */

#include "SameNumberDifferentColorJoker.h"

#include "../../game/stones/Joker.h"

SameNumberDifferentColorJoker::SameNumberDifferentColorJoker(QWidget* parent) : StoneMover(parent)
{
    gamestones.push_back(new Gamestone(parent, &manager, true, Gamestone::StoneColor::RED, 6));
    gamestones.push_back(new Gamestone(parent, &manager, true, Gamestone::StoneColor::BLACK, 6));
    gamestones.push_back(new Joker(parent, &manager, true));
    gamestones.push_back(new Gamestone(parent, &manager, true, Gamestone::StoneColor::BLUE, 6));

    for (QVector<Gamestone*>::const_iterator it(gamestones.begin());
                it != gamestones.end();
                ++it)
    {
        (*it)->setParent(Gamestone::StoneParent::HOLDER);
    }

    QSize size = parent->size();
    gamestones.at(2)->move((size.width() / 2) - 50, (size.height() / 2) - 50);
    dropStone(*(gamestones.at(0)), *(gamestones.at(1)));
    dropStone(*(gamestones.at(1)), *(gamestones.at(2)));

    gamestones.at(3)->move((size.width() / 2) + 50, (size.height() / 2) + 50);

    for (QVector<Gamestone*>::const_iterator it(gamestones.begin());
                it != gamestones.end();
                ++it)
    {
        (*it)->storeStone();
    }

    sourceStone = gamestones.at(3);
    targetStone = gamestones.at(2);
}

