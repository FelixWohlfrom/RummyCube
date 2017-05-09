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
 * Animation.h
 *
 *  Created on: 24.03.2017
 *      Author: Felix Wohlfrom
 */

#ifndef SRC_MAIN_GUI_ANIMATIONS_ANIMATION_H_
#define SRC_MAIN_GUI_ANIMATIONS_ANIMATION_H_

#include <QTimer>

/**
 * The base class for animations. It triggers the {@link animate()} method of the subclasses
 * each time a given interval runs out.
 */
class Animation : public QObject
{
    Q_OBJECT

    public:
        /**
         * Creates a new animation. Will start the animation once everything is initialized.
         *
         * @param parent A parent object
         * @param interval The interval in milliseconds after which the {@link animate()} method
         *                 is called
         */
        Animation(QObject* parent, int interval);

        /**
         * Destructor, cleans up the timer.
         */
        ~Animation();

        /**
         * Starts the animation.
         */
        void start();

        /**
         * Stops the animation.
         */
        void stop();

    protected slots:
        /**
         * The abstract method that will be called in the subclasses to play the animation.
         */
        virtual void animate() = 0;

    private:
        /**
         * The timer object.
         */
        QTimer* timer;
};

#endif /* SRC_MAIN_GUI_ANIMATIONS_ANIMATION_H_ */
