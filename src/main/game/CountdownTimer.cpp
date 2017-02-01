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
 * CountdownTimer.cpp
 *
 *  Created on: 26.02.2012
 *      Author: Felix Wohlfrom
 */

#include "CountdownTimer.h"

#include <iostream>

CountdownTimer::CountdownTimer(int seconds) :
    QTimer(), countdownTime(seconds), currTime(0), running(false)
{
    connect(this, SIGNAL(timeout()),
            this, SLOT(countDown()));
}

int CountdownTimer::getTime()
{
    return countdownTime;
}

int CountdownTimer::timeLeft() const
{
    if (countdownTime == -1)
    {
        return 0;
    }
    else
    {
        return countdownTime - currTime;
    }
}

bool CountdownTimer::isRunning() const
{
    return running;
}

void CountdownTimer::start(int t0)
{
    if (!running && countdownTime > -1)
    {
        running = true;
        currTime = t0;
        QTimer::start(1000);
    }
}

void CountdownTimer::pause()
{
    if (running)
    {
        running = false;
        QTimer::stop();
    }
}

void CountdownTimer::resume()
{
    if (!running && countdownTime > -1)
    {
        running = true;
        QTimer::start(1000);
    }
}

void CountdownTimer::countDown()
{
    if (running)
    {
        currTime++;
    }
}
