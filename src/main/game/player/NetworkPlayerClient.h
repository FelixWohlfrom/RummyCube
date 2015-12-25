/*
 * NetworkPlayerClient.h
 *
 *  Created on: 09.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKPLAYERCLIENT_H_
#define NETWORKPLAYERCLIENT_H_

#include "NetworkPlayer.h"

#include "../../network/NetworkEventHandlerClient.h"

/**
 * Represents an opponent player for network game on the clients.
 */
class NetworkPlayerClient: public NetworkPlayer
{
	public:
		/**
		 * Creates a new opponent player for network game on client
		 *
		 * @param name The name of the player
		 * @param stoneManager The stone manager on which the player can play
		 * @param sock The socket which the player uses for communication
		 * @param eventHandler The event handler which is used to notify or get notification of game closing
		 * @param skipAfterCreation If the player play() method should be skipped directly after creation of game
		 */
		NetworkPlayerClient(wxString name, StoneManager& stoneManager, wxSocketBase& sock, NetworkEventHandlerClient& eventHandler, bool skipAfterCreation);

		/**
		 * Destructor. Cleans up the socket
		 */
		~NetworkPlayerClient();

		/**
		 * Lets the player play
		 */
		void play();

		/**
		 * Event handler for socket events.
		 *
		 * @param e The event
		 */
		void OnSocketEvent(wxSocketEvent& e);

	private:
		/**
		 * If the current opponent should be skipped after game creation
		 */
		bool skipAfterCreation;
};

#endif /* NETWORKPLAYERCLIENT_H_ */
