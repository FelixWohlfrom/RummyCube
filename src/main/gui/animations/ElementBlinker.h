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
 * ElementBlinker.h
 *
 *  Created on: 25.03.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ANIMATIONS_ELEMENTBLINKER_H_
#define SRC_MAIN_GUI_ANIMATIONS_ELEMENTBLINKER_H_

#include "Animation.h"

#include <QWidget>

/**
 * This animation will blink the element in given constructor.
 */
class ElementBlinker : public Animation
{
    public:
        /**
         * Creates a new blinker element.
         *
         * @param element The element to blink.
         */
        ElementBlinker(QWidget* element);

        /**
         * Will clean up the blinker.
         */
        ~ElementBlinker();

    private:
        /**
         * The element to blink.
         */
        QWidget* element;

        /**
         * If the element was originally enabled or not.
         */
        bool originalEnabled;

        /**
         * Will blink the element.
         */
        void animate();
};

#endif /* SRC_MAIN_GUI_ANIMATIONS_ELEMENTBLINKER_H_ */
