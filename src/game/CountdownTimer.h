/*
 * CountdownTimer.h
 *
 *  Created on: 26.02.2012
 *      Author: Felix Wohlfrom
 */

#ifndef COUNTDOWNTIMER_H_
#define COUNTDOWNTIMER_H_

#include <wx/stopwatch.h>

class CountdownTimer: private wxStopWatch
{
	public:
		/**
		 * Constructor. Creates a new countdown timer.
		 *
		 * @param seconds The initial time to count down in seconds. -1 to disable timer
		 */
		CountdownTimer(int seconds);

		/**
		 * Returns the time the countdown timer can count down
		 *
		 * @return The maximum time set in ctor and setTime method in seconds. -1 if timer is disabled
		 */
		int getTime();

		/**
		 * Returns the time which still is left in the counter
		 *
		 * @return The time that is left in seconds
		 */
		int timeLeft() const;

		/**
		 * Returns if the timer is running or not
		 */
		bool isRunning() const;

		/**
		 * Starts the timer with the time set in setTime
		 *
		 * @param t0 The time in seconds from which the timer should start
		 */
		void start(int t0 = 0);

		/**
		 * Pauses the countdown timer
		 *
		 */
		void pause();

		/**
		 * Resumes the countdown timer
		 */
		void resume();

	private:
		/**
		 * The time that is left for the countdown timer in seconds
		 */
		int countdownTime;

		/**
		 * If the countdown timer is running or not
		 */
		bool running;
};

#endif /* COUNTDOWNTIMER_H_ */
