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
 * Tutorial.h
 *
 *  Created on: 18.03.2017
 *      Author: Felix Wohlfrom
 */

#ifndef TUTORIAL_H_
#define TUTORIAL_H_

#include "animations/Animation.h"

#include <QWidget>
#include <QWizard>
#include <QMutex>

namespace Ui {
    class Tutorial;
}

/**
 * A tutorial wizard. The wizard contains several pages. In some of the pages there are links
 * shown. If the cursor is hovered over the link an animation is played.
 */
class Tutorial : public QWizard
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the widget elements.
         *
         * @param parent The parent window.
         * @param heap The heap widget.
         * @param gameboard The gameboard widget.
         * @param holder The holder widget.
         */
        Tutorial(QWidget* parent, QWidget* heap, QWidget* gameboard, QWidget* holder);

        /**
         * Destructor. Deletes the ui object.
         */
        ~Tutorial();

    private:
        /**
         * The tutorial widget ui.
         */
        Ui::Tutorial* ui;

        /**
         * The heap widget.
         */
        QWidget* heap;

        /**
         * The gameboard widget.
         */
        QWidget* gameboard;

        /**
         * The holder widget.
         */
        QWidget* holder;

        /**
         * The animation to play.
         */
        Animation* currentAnimation;

        /**
         * The mutex to synchronize access to the current animation.
         * Sometimes it happens that an animation is created and at the same time the animation
         * url is left. Then the initially created animation would never be destroyed.
         */
        QMutex currentAnimationSyncher;

        /**
         * Returns the id of the next qwizard page.
         */
        int nextId() const;

    private slots:
        /**
         * Plays an animation if a link in the tutorial is hovered.
         *
         * @param animation the animation to play.
         */
        void playAnimation(QString animation);
};

#endif /* TUTORIAL_H_ */
