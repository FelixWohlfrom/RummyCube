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
 * ParentChangedEvent.cpp
 *
 *  Created on: 02.08.2011
 *      Author: Felix Wohlfrom
 */

#include "ParentChangedEvent.h"

ParentChangedEvent::ParentChangedEvent(Gamestone::StoneParent parent, Gamestone* sender,
        bool stoneMoved) :
    vetoed(false), parent(parent), sender(sender), moved(stoneMoved)
{}

int ParentChangedEvent::getParent() const
{
    return parent;
}

Gamestone* ParentChangedEvent::getSender() const
{
    return sender;
}

bool ParentChangedEvent::getStoneMoved() const
{
    return moved;
}

bool ParentChangedEvent::getVeto() const
{
    return vetoed;
}

void ParentChangedEvent::veto(bool veto)
{
    this->vetoed = veto;
}
