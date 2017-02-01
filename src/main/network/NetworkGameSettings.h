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
 * NetworkGameSettings.h
 *
 *  Created on: 07.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKGAMESETTINGS_H_
#define NETWORKGAMESETTINGS_H_

/**
 * This struct is used to keep the network game settings together.
 */

struct NetworkGameSettings
{
    /**
     * The number of stones that can be taken on first round.
     */
    int stonesAtBeginning;

    /**
     * The sum that has to be reached on first play out.
     */
    int sumAtBeginning;

    /**
     * If the sum has to be in one row or can be split in multiple rows.
     */
    bool stonesInOneRow;

    /**
     * The time limit for one round. -1 if disabled.
     */
    int timeLimit;
};


#endif /* NETWORKGAMESETTINGS_H_ */
