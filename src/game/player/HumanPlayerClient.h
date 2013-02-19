/*
 * HumanPlayerClient.h
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef HUMANPLAYERCLIENT_H_
#define HUMANPLAYERCLIENT_H_

#include "HumanPlayer.h"

#include <wx/socket.h>

class HumanPlayerClient: public HumanPlayer
{
	public:
		/**
		 * Creates a new human player on server on network game
		 *
		 * @param stoneManager The stone manager on which the player can play
		 * @param sock The socket which the player uses for communication
		 * @param initialStonesLeftToTake The initial stones that the player can take
		 * @param sumInFirstRound The sum that is needed for the player to play out first time
		 * @param stonesInOneRow If the sum needs to be in one row or can be splitted in multiple rows
		 */
		HumanPlayerClient(StoneManager& stoneManager, wxSocketBase& sock, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow);

		/**
		 * Dummy to avoid warnings
		 */
		virtual ~HumanPlayerClient();

		/**
		 * Notifies other players that round is finished
		 */
		void roundFinished();

	private:
		/**
		 * The socket on which the network player works
		 */
		wxSocketBase& sock;
};

#endif /* HUMANPLAYERCLIENT_H_ */
