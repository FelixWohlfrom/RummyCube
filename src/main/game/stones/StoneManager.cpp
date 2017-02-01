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
 * StoneManager.cpp
 *
 *  Created on: 26.07.2011
 *      Author: Felix Wohlfrom
 */

#include "StoneManager.h"

#include "Joker.h"
#include "../Settings.h"

#include <QCoreApplication>

StoneManager::StoneManager() :
    draggedStone(NULL)
{
    // Load settings
    Settings* settings = Settings::getInstance();
    moveAllStonesInRow = settings->value("General/MoveAllStones", true).toBool();
}

StoneManager::~StoneManager()
{
    // Store settings
    Settings* settings = Settings::getInstance();
    settings->setValue("General/MoveAllStones", moveAllStonesInRow);

    // Clean up the stones
    while (!gamestones.empty())
    {
        delete gamestones.back();
        gamestones.pop_back();
        QCoreApplication::processEvents();
    }
}

void StoneManager::createStones(QWidget* heap)
{
    // Create a bunch of stones
    for (int i(0); i < 2; ++i)
    {
        for (int j(1); j <= 13; ++j)
        {
            gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::BLACK, j));
            gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::BLUE, j));
            gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::RED, j));
            gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::YELLOW, j));
        }
    }

    // Create the jockers
    gamestones.push_back(new Joker(heap, this, true));
    gamestones.push_back(new Joker(heap, this, false));
}

QVector<Gamestone*>& StoneManager::getStones()
{
    return gamestones;
}

int StoneManager::getStoneCountOnHeap() const
{
    int count(0);
    for (QVector<Gamestone*>::const_iterator it(gamestones.begin());
            it != gamestones.end();
            ++it)
    {
        if ((*it)->getParent() == Gamestone::HEAP)
        {
            ++count;
        }
    }

    return count;
}

Gamestone* StoneManager::getStoneFromInt(int value) const
{
    // The masks to extract the needed values
    #define FIRSTMASK 1
    #define JOKERMASK 2
    #define COLORMASK 124

    bool isFirst = (value & FIRSTMASK) != 0;
    bool isJoker = (value & JOKERMASK) != 0;
    Gamestone::StoneColor color = (Gamestone::StoneColor)(((value & COLORMASK) >> 3) - 1);
    int number = (value >> 8) - 1;

    for (QVector<Gamestone*>::const_iterator it(gamestones.begin());
            it != gamestones.end();
            ++it)
    {
        // If we need to return a joker, color and number are not needed
        if (isJoker)
        {
            if ((*it)->isJoker() && (*it)->isFirst() == isFirst)
            {
                return *it;
            }
        }
        // Check for color and number
        else
        {
            if (!(*it)->isJoker() && (*it)->isFirst() == isFirst
                    && (*it)->getColor() == color && (*it)->getNumber() == number)
            {
                return *it;
            }
        }
    }

    return NULL;
}
