/*
 * ParentChangedEvent.cpp
 *
 *  Created on: 02.08.2011
 *      Author: Felix Wohlfrom
 */

#include "ParentChangedEvent.h"

ParentChangedEvent::ParentChangedEvent(Gamestone::StoneParent parent, Gamestone* sender,
        bool stoneMoved) :
    vetoed(false), parent(parent), sender(sender), moved(stoneMoved)
{}

int ParentChangedEvent::getParent() const
{
    return parent;
}

Gamestone* ParentChangedEvent::getSender() const
{
    return sender;
}

bool ParentChangedEvent::getStoneMoved() const
{
    return moved;
}

bool ParentChangedEvent::getVeto() const
{
    return vetoed;
}

void ParentChangedEvent::veto(bool veto)
{
    this->vetoed = veto;
}
