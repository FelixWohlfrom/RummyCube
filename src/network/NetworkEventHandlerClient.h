/*
 * NetworkEventHandlerClient.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKEVENTHANDLERCLIENT_H_
#define NETWORKEVENTHANDLERCLIENT_H_

#include <list>

#include <wx/event.h>
#include <wx/socket.h>

class NetworkPlayerClient;

/**
 * This class is needed because sockets in wxwidgets are not allows to have more than one event handler at once.
 * It receives all events in network games on the client and forwards it to the player which should receive the event.
 * Implemented as singleton, because if it was included in Network.h, a circular reference would be created through NetworkPlayerClient.h
 */
class NetworkEventHandlerClient: public wxEvtHandler
{
	public:
		/**
		 * Ctor.
		 * Registers the handler for socket events on creation
		 *
		 * @param sock The socket to which this event handler should listen to
		 */
		NetworkEventHandlerClient(wxSocketBase& sock);

		/**
		 * Adds a client to the list to which the events are broadcasted
		 *
		 * @param newReceiver The new receiver which should be added to list
		 */
		void addEventReceiver(NetworkPlayerClient* newReceiver);

		/**
		 * Removes the given client from listeners
		 *
		 * @param toRemove The receiver which should be removed
		 */
		void removeEventReceiver(NetworkPlayerClient* toRemove);

	private:
		/**
		 * The list of network player clients to which the network events can be forwarded
		 */
		std::list<NetworkPlayerClient*> eventReceivers;

		/**
		 * Event handler for socket events.
		 *
		 * @param e The event
		 */
		void OnSocketEvent(wxSocketEvent& e);

		// Needed for wxwidgets
	    DECLARE_EVENT_TABLE()
};

#endif /* NETWORKEVENTHANDLERCLIENT_H_ */
