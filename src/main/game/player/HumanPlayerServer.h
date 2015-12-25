/*
 * HumanPlayerServer.h
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef HUMANPLAYERSERVER_H_
#define HUMANPLAYERSERVER_H_

#include "HumanPlayer.h"

#include <list>

#include <wx/socket.h>

class HumanPlayerServer: public HumanPlayer
{
	public:
		/**
		 * Creates a new human player on server on network game
		 *
		 * @param stoneManager The stone manager on which the player can play
		 * @param initialStonesLeftToTake The initial stones that the player can take
		 * @param sumInFirstRound The sum that is needed for the player to play out first time
		 * @param stonesInOneRow If the sum needs to be in one row or can be splitted in multiple rows
		 * @param otherPlayerSocks The sockets which are connected to the other players (for broadcasting)
		 */
		HumanPlayerServer(StoneManager& stoneManager, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow, std::list<wxSocketBase*> otherPlayerSocks);

		/**
		 * Dummy to avoid warnings
		 */
		~HumanPlayerServer();

		/**
		 * Notifies other players that round is finished
		 */
		void roundFinished();

	private:
		/**
		 * The sockets which are connected with other players (for broadcasting)
		 */
		std::list<wxSocketBase*> otherPlayerSocks;
};

#endif /* HUMANPLAYERSERVER_H_ */
