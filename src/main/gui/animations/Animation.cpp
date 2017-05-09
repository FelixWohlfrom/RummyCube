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
 * Animation.cpp
 *
 *  Created on: 24.03.2017
 *      Author: Felix Wohlfrom
 */

#include "Animation.h"

Animation::Animation(QObject* parent, int interval)
{
    timer = new QTimer(parent);
    timer->setInterval(interval);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    this->start();
}

Animation::~Animation() {
    this->stop();
    delete timer;
}

void Animation::start()
{
    timer->start();
}

void Animation::stop()
{
    timer->stop();
}
