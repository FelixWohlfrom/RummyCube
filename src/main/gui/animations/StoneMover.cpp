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
 * StoneMover.cpp
 *
 *  Created on: 12.04.2017
 *      Author: Felix Wohlfrom
 */

#include "StoneMover.h"

#define POSITION_TOLERANCE 10

#define MOVEMENT_SPEED 5

#define ANIMATION_TIME 100

#define FINAL_WAIT_TIME 1000

#include <QDropEvent>
#include <QMimeData>
#include <QThread>

StoneMover::StoneMover(QObject* parent) :
    Animation(parent, ANIMATION_TIME), sourceStone(NULL), targetStone(NULL), finalWaitTimeCounter(0)
{
    manager.moveAllStonesInRow = false;
}

StoneMover::~StoneMover()
{
    while (!gamestones.empty())
    {
        gamestones.back()->hide();
        delete gamestones.back();
        gamestones.pop_back();
    }
}

void StoneMover::dropStone(Gamestone& source, Gamestone& target)
{
    QMimeData *mimeData = new QMimeData;
    mimeData->setData(GAMESTONE_MIMETYPE, NULL);

    QDropEvent* event = new QDropEvent(
            source.pos(), Qt::MoveAction | Qt::CopyAction,
            mimeData, Qt::LeftButton, Qt::NoModifier);

    manager.draggedStone = &source;
    target.dropEvent(event);

    delete event;
    delete mimeData;
}

void StoneMover::animate()
{
    if (sourceStone == NULL || targetStone == NULL)
    {
        return;
    }

    if (finalWaitTimeCounter > 0)
    {
        finalWaitTimeCounter--;

        if (finalWaitTimeCounter == 0)
        {
            reset();
        }

        return;
    }

    int diffX = sourceStone->pos().x() - targetStone->pos().x();
    int diffY = sourceStone->pos().y() - targetStone->pos().y();

    int newX = sourceStone->pos().x();
    if (abs(diffX) > POSITION_TOLERANCE)
    {
        if (diffX > 0)
        {
            newX -= MOVEMENT_SPEED;
        }
        else
        {
            newX += MOVEMENT_SPEED;
        }
    }

    int newY = sourceStone->pos().y();
    if (abs(diffY) > POSITION_TOLERANCE)
    {
        if (diffY > 0)
        {
            newY -= MOVEMENT_SPEED;
        }
        else
        {
            newY += MOVEMENT_SPEED;
        }
    }

    sourceStone->move(newX, newY);
    sourceStone->moveAddedStones();

    if (abs(diffX) <= POSITION_TOLERANCE
            && abs(diffY) <= POSITION_TOLERANCE)
    {
        dropStone(*sourceStone, *targetStone);
        finalWaitTimeCounter = FINAL_WAIT_TIME / ANIMATION_TIME;
    }
}

void StoneMover::reset()
{
    for (QVector<Gamestone*>::const_iterator it(gamestones.begin());
                it != gamestones.end();
                ++it)
    {
        (*it)->restoreStone();
    }
}
