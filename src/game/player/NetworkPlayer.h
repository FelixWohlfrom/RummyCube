/*
 * NetworkPlayer.h
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKPLAYER_H_
#define NETWORKPLAYER_H_

#include "OpponentPlayer.h"

#include "../../network/Network.h"

#include <wx/socket.h>

class NetworkPlayer: public OpponentPlayer
{
	public:
		/**
		 * Creates a new opponent player for network game on client
		 *
		 * @param name The name of the player
		 * @param stoneManager The stone manager on which the player can play
		 * @param sock The socket on which the network player should work
		 */
		NetworkPlayer(wxString name, StoneManager& stoneManager, wxSocketBase& sock);

		/**
		 * Destructor.
		 */
		virtual ~NetworkPlayer();

		/**
		 * Lets the player play
		 */
		virtual void play() = 0;

		/**
		 * Returns if the player has won.
		 *
		 * @return If the player has won or not
		 */
		bool hasWon();

		/**
		 * Sets the event handler for NetworkCloseEvents to given handler
		 *
		 * @param newHandler The handler to set.
		 */
		void setCloseEventHandler(wxEvtHandler* newHandler);

	protected:
		/**
		 * If the player has won the game or not
		 */
		bool playerHasWon;

		/**
		 * Stores if the player finished his round or not
		 */
		bool roundFinished;

		/**
		 * The socket on which the network player works
		 */
		wxSocketBase& sock;

		/**
		 * The event handler for network close events
		 */
		wxEvtHandler* handler;

		/**
		 * Parses the data received from socket associated with player
		 */
		void parseData();
};

#endif /* NETWORKPLAYER_H_ */
