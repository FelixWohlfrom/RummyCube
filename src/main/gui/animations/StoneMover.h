/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2017 Felix Wohlfrom                                    *
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
 * StoneMover.h
 *
 *  Created on: 12.04.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ANIMATIONS_STONEMOVER_H_
#define SRC_MAIN_GUI_ANIMATIONS_STONEMOVER_H_

#include "Animation.h"

#include "../../game/stones/Gamestone.h"
#include "../../game/stones/StoneManager.h"

/**
 * Generic animation will repeatedly move a stone on an existing row.
 * In constructor, the sourceStone and targetStone pointer need to be set.
 */
class StoneMover: public Animation
{
    public:
        /**
         * Creates the animation.
         *
         * @param parent The parent widget.
         */
        StoneMover(QObject* parent);

        /**
         * Will destroy the animation and the gamestones.
         */
        ~StoneMover();

    protected:
        /**
         * Helper method to drop source stone on target stone.
         *
         * @param source The source stone that will be dropped.
         * @param target The stone to drop.
         */
        void dropStone(Gamestone& source, Gamestone& target);

        /**
         * The source stone.
         */
        Gamestone* sourceStone;

        /**
         * The drop target.
         */
        Gamestone* targetStone;

        /**
         * Dummy stone manager.
         */
        StoneManager manager;

        /**
         * A list of gamestones to animate.
         */
        QVector<Gamestone*> gamestones;

    private:
        /**
         * Counter on how many animations still need to be executed until the animation is reset.
         */
        int finalWaitTimeCounter;

        /**
         * Plays the animation.
         */
        void animate();

        /**
         * Resets the animation.
         */
        void reset();
};

#endif /* SRC_MAIN_GUI_ANIMATIONS_STONEMOVER_H_ */
