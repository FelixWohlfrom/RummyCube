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
 * SameColorIncrementingNumber.h
 *
 *  Created on: 12.04.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ANIMATIONS_SAMECOLORINCREMENTINGNUMBER_H_
#define SRC_MAIN_GUI_ANIMATIONS_SAMECOLORINCREMENTINGNUMBER_H_

#include "StoneMover.h"

/**
 * This animation will combine a row with same color and incrementing number.
 */
class SameColorIncrementingNumber: public StoneMover
{
    public:
        /**
         * Creates the animation.
         *
         * @param The parent widget of the stones.
         */
        SameColorIncrementingNumber(QWidget* parent);
};

#endif /* SRC_MAIN_GUI_ANIMATIONS_SAMECOLORINCREMENTINGNUMBER_H_ */
