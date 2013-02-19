/*
 * NetworkGameClosedEvent.cpp
 *
 *  Created on: 18.07.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkGameClosedEvent.h"

// For wxwidgets
DEFINE_EVENT_TYPE(wxNETWORK_GAME_CLOSED)

NetworkGameClosedEvent::NetworkGameClosedEvent(wxEventType commandType, wxString closedBy, int id) :
	wxCommandEvent(commandType, id), closedBy(closedBy)
{};

NetworkGameClosedEvent::NetworkGameClosedEvent(const NetworkGameClosedEvent& orig) :
	wxCommandEvent(orig), closedBy(orig.closedBy)
{};

NetworkGameClosedEvent::~NetworkGameClosedEvent() {};

wxEvent* NetworkGameClosedEvent::Clone() const
{
	return new NetworkGameClosedEvent(*this);
}

wxString NetworkGameClosedEvent::getClosedBy() const
{
	return closedBy;
}
