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
