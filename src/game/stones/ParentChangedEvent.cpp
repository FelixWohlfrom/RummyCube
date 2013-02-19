/*
 * ParentChangedEvent.cpp
 *
 *  Created on: 02.08.2011
 *      Author: Felix Wohlfrom
 */

#include "ParentChangedEvent.h"

// For wxwidgets
DEFINE_EVENT_TYPE(wxPARENT_CHANGED)

ParentChangedEvent::ParentChangedEvent(wxEventType commandType, int parent, Gamestone* sender, bool stoneMoved, int id) :
	wxCommandEvent(commandType, id), vetoed(false), parent(parent), sender(sender), moved(stoneMoved)
{};

ParentChangedEvent::ParentChangedEvent(const ParentChangedEvent& orig) :
	wxCommandEvent(orig), vetoed(orig.vetoed), parent(orig.parent), sender(orig.sender), moved(orig.moved)
{};

ParentChangedEvent::~ParentChangedEvent() {};

wxEvent* ParentChangedEvent::Clone() const
{
	return new ParentChangedEvent(*this);
}

int ParentChangedEvent::GetParent() const
{
	return parent;
}

Gamestone* ParentChangedEvent::GetSender() const
{
	return sender;
}

bool ParentChangedEvent::GetStoneMoved() const
{
	return moved;
}

bool ParentChangedEvent::GetVeto() const
{
	return vetoed;
}

void ParentChangedEvent::Veto(bool veto)
{
	this->vetoed = veto;
}
