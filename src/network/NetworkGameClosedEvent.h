/*
 * NetworkGameClosedEvent.h
 *
 *  Created on: 18.07.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKGAMECLOSEDEVENT_H_
#define NETWORKGAMECLOSEDEVENT_H_

#include <wx/event.h>

// Declaration for wxwidgets
DECLARE_EVENT_TYPE(wxNETWORK_GAME_CLOSED, wxNewEventType())

/**
 * This class defines the event that is fired if the main window receives network game input
 */
class NetworkGameClosedEvent: public wxCommandEvent
{
	public:
		/**
		 * Default constructor
		 *
		 * @param commandType The command type of the event
		 * @param closedBy The name of the player which closed the game
		 * @param id The id of the event (default 0)
		 */
		NetworkGameClosedEvent(wxEventType commandType, wxString closedBy, int id = 0);

		/**
		 * Copy constructor
		 *
		 * @param orig The original event
		 */
		NetworkGameClosedEvent(const NetworkGameClosedEvent &orig);

		/**
		 * Destructor, empty
		 */
		virtual ~NetworkGameClosedEvent();

		/**
		 * Clones the event
		 *
		 * @return A clone of the event
		 */
		wxEvent* Clone() const;

		/**
		 * Returns the name of the player that closed the game
		 *
		 * @return The name of the player
		 */
		wxString getClosedBy() const;

	private:
		/**
		 * The name of the player that closed the game
		 */
		wxString closedBy;
};

// For wxwidgets
typedef void (wxEvtHandler::*NetworkGameClosedEventFunction)(NetworkGameClosedEvent&);

#define EVT_NETWORK_GAME_CLOSED(fn)                                        	\
	DECLARE_EVENT_TABLE_ENTRY( wxNETWORK_GAME_CLOSED, wxID_ANY, wxID_ANY,  	\
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) 	    \
			wxStaticCastEvent(NetworkGameClosedEventFunction, &fn), (wxObject*) NULL ),

#endif /* NETWORKGAMECLOSEDEVENT_H_ */
