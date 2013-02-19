/*
 * CountdownTimer.cpp
 *
 *  Created on: 26.02.2012
 *      Author: Felix Wohlfrom
 */

#include "CountdownTimer.h"

CountdownTimer::CountdownTimer(int seconds) :
	wxStopWatch(), countdownTime(seconds), running(false)
{
	this->Pause();
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
		int timeLeft = countdownTime - (int)(wxStopWatch::Time() / 1000);
		if (timeLeft > 0)
		{
			return timeLeft;
		}
		else
		{
			return 0;
		}
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
		wxStopWatch::Start(t0 * 1000);
	}
}

void CountdownTimer::pause()
{
	if (running)
	{
		running = false;
		wxStopWatch::Pause();
	}
}

void CountdownTimer::resume()
{
	if (!running && countdownTime > -1)
	{
		running = true;
		wxStopWatch::Resume();
	}
}
