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
