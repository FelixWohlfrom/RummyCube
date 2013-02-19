/*
 * NetworkPlayerServer.h
 *
 *  Created on: 14.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKPLAYERSERVER_H_
#define NETWORKPLAYERSERVER_H_

#include "NetworkPlayer.h"

#include <list>

class NetworkPlayerServer: private wxEvtHandler, public NetworkPlayer
{
	public:
		/**
		 * Creates a new opponent player for network game on server
		 *
		 * @param name The name of the player
		 * @param stoneManager The stone manager on which the player can play
		 * @param sock The socket which is connected to the player
		 * @param allPlayerSocks The sockets which are connected to the other players (for broadcasting)
		 */
		NetworkPlayerServer(wxString name, StoneManager& stoneManager, wxSocketBase& sock, std::list<wxSocketBase*> allPlayerSocks);

		/**
		 * Destructor. Cleans up socket of player
		 */
		~NetworkPlayerServer();

		/**
		 * Lets the player play
		 */
		void play();

	private:
		/**
		 * The sockets which are connected with other players (for broadcasting)
		 */
		std::list<wxSocketBase*> otherPlayerSocks;

		/**
		 * Event handler for socket events.
		 *
		 * @param e The event
		 */
		void OnSocketEvent(wxSocketEvent& e);

		// Needed for wxwidgets
	    DECLARE_EVENT_TABLE()
};

#endif /* NETWORKPLAYERSERVER_H_ */
