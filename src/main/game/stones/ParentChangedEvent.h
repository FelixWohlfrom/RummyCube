/*
 * ParentChangedEvent.h
 *
 *  Created on: 02.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef PARENTCHANGEDEVENT_H_
#define PARENTCHANGEDEVENT_H_

#include "Gamestone.h"
#include <wx/event.h>

// Declaration for wxwidgets
DECLARE_EVENT_TYPE(wxPARENT_CHANGED, wxNewEventType())

/**
 * This class defines the event that is fired if the parent of a gamestone
 * has changed.
 */
class ParentChangedEvent: public wxCommandEvent
{
	public:
		/**
		 * Default constructor
		 *
		 * @param commandType The command type of the event
		 * @param parent The new parent
		 * @param sender The sender stone
		 * @param stoneMoved If the stone has been moved during parent change or not
		 * @param id The id of the event (default 0)
		 */
		ParentChangedEvent(wxEventType commandType, int parent, Gamestone* sender, bool stoneMoved, int id = 0);

		/**
		 * Copy constructor
		 *
		 * @param orig The original event
		 */
		ParentChangedEvent(const ParentChangedEvent &orig);

		/**
		 * Destructor, empty
		 */
		virtual ~ParentChangedEvent();

		/**
		 * Clones the event
		 *
		 * @return A clone of the event
		 */
		wxEvent* Clone() const;

		/**
		 * Returns the new parent of the stone
		 *
		 * @return The parent
		 */
		int GetParent() const;

		/**
		 * Returns the sender gamestone
		 *
		 * @return The sender
		 */
		Gamestone* GetSender() const;

		/**
		 * Returns if the stone has been moved (true) or clicked (false) for parent change
		 *
		 * @return Returns true, if the stone has been moved
		 */
		bool GetStoneMoved() const;

		/**
		 * Returns the status of the veto.
		 *
		 * @return If the event has been vetoed
		 */
		bool GetVeto() const;

		/**
		 * Veto the event (so the parent won't be changed in sender)
		 *
		 * @param veto If the event should be vetoed or not
		 */
		void Veto(bool veto = true);

	private:
		/**
		 * If the event has been vetoed.
		 */
		bool vetoed;

		/**
		 * The new parent to which the stone has been set
		 */
		int parent;

		/**
		 * The sender gamestone of the event
		 */
		Gamestone* sender;

		/**
		 * If the stone has been moved during parent change or not
		 */
		bool moved;
};

// For wxwidgets
typedef void (wxEvtHandler::*ParentChangedEventFunction)(ParentChangedEvent &);

#define EVT_PARENT_CHANGED(fn)                                        	\
	DECLARE_EVENT_TABLE_ENTRY( wxPARENT_CHANGED, wxID_ANY, wxID_ANY,  	\
	(wxObjectEventFunction)(wxEventFunction)(wxCommandEventFunction) 	\
			wxStaticCastEvent(ParentChangedEventFunction, &fn), (wxObject*) NULL ),

#endif /* PARENTCHANGEDEVENT_H_ */
