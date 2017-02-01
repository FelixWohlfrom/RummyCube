/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2016 Felix Wohlfrom                                    *
 *                                                                       *
 *  This program is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
 * ParentChangedEvent.h
 *
 *  Created on: 02.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef PARENTCHANGEDEVENT_H_
#define PARENTCHANGEDEVENT_H_

#include "Gamestone.h"

/**
 * This class defines the data that is emitted when a gamestone tries to change the parent.
 * It provides a method to veto the event, so that the parent isn't changed.
 */
class ParentChangedEvent
{
    public:
        /**
         * Default constructor.
         *
         * @param parent The new parent
         * @param sender The sender stone
         * @param stoneMoved If the stone has been moved during parent change or not
         */
        ParentChangedEvent(Gamestone::StoneParent parent, Gamestone* sender, bool stoneMoved);

        /**
         * Returns the new parent of the stone.
         *
         * @return The parent
         */
        int getParent() const;

        /**
         * Returns the sender gamestone.
         *
         * @return The sender
         */
        Gamestone* getSender() const;

        /**
         * Returns if the stone has been moved (true) or clicked (false) for parent change.
         *
         * @return Returns true if the stone has been moved
         */
        bool getStoneMoved() const;

        /**
         * Returns the status of the veto.
         *
         * @return If the event has been vetoed
         */
        bool getVeto() const;

        /**
         * Veto the event (so the parent won't be changed in sender).
         *
         * @param veto If the event should be vetoed or not
         */
        void veto(bool veto = true);

    private:
        /**
         * If the event has been vetoed.
         */
        bool vetoed;

        /**
         * The new parent to which the stone has been set.
         */
        int parent;

        /**
         * The sender gamestone of the event.
         */
        Gamestone* sender;

        /**
         * If the stone has been moved during parent change or not.
         */
        bool moved;
};

#endif /* PARENTCHANGEDEVENT_H_ */
