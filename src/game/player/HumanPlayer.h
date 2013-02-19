/*
 * HumanPlayer.h
 *
 *  Created on: 10.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef HUMANPLAYER_H_
#define HUMANPLAYER_H_

#include "Player.h"
#include "../stones/Gamestone.h"

class HumanPlayer: public Player
{
	public:
		/**
		 * Creates a new human player
		 *
 		 * @param stoneManager The stone manager on which the player can play
 		 * @param initialStonesLeftToTake The initial stones that the player can take
		 * @param sumInFirstRound The sum that is needed for the player to play out first time
		 * @param stonesInOneRow If the sum needs to be in one row or can be splitted in multiple rows
		 */
		HumanPlayer(StoneManager& stoneManager, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow);

		/**
		 * Dummy to avoid warnings
		 */
		virtual ~HumanPlayer();

		/**
		 * Test the stones on gameboard
		 *
		 * @return true If all stones passed the tests
		 */
		bool testStonesOnGameboard();

		/**
		 * This method is used in network game to notify other players that the round of the current player has beend finished
		 */
		virtual void roundFinished();

	private:
		/**
		 * Resets the stone invalidation
		 */
		void resetStoneInvalidation();

		/**
		 * Invalidates all own stones on the game board
		 */
		void invalidateOwnStonesOnBoard();

		/**
		 * Invalidates the row of given stone
		 *
		 * @param stone The stone of which the row should be invalidated
		 */
		void invalidateRow(Gamestone& stone);
};

#endif /* HUMANPLAYER_H_ */
