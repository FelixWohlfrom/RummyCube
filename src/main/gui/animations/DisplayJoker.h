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
 * DisplayStone.h
 *
 *  Created on: 21.04.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ANIMATIONS_DISPLAYJOKER_H_
#define SRC_MAIN_GUI_ANIMATIONS_DISPLAYJOKER_H_

#include "Animation.h"

#include "../../game/stones/StoneManager.h"

/**
 * This "animation" will just display a single joker stone.
 */
class DisplayJoker: public Animation
{
    public:
        /**
         * The constructor. Will display a joker.
         *
         * @param parent The parent object
         */
        DisplayJoker(QWidget* parent);

        /**
         * Destructor. Will destroy the joker and hide it.
         */
        ~DisplayJoker();

    private:
        /**
         * A dummy stone manager.
         */
        StoneManager manager;

        /**
         * The stone to display.
         */
        Gamestone* stoneToDisplay;

        /**
         * The animation, will do nothing.
         */
        void animate();
};

#endif /* SRC_MAIN_GUI_ANIMATIONS_DISPLAYJOKER_H_ */
