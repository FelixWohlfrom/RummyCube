/*
 * OpponentPlayer.h
 *
 *  Created on: 08.03.2012
 *      Author: felix
 */

#ifndef OPPONENTPLAYER_H_
#define OPPONENTPLAYER_H_

#include "Player.h"

/**
 * This class is used to unify common methods for ai player and opponent network player.
 */
class OpponentPlayer: public Player
{
    public:
        /**
         * Creates a new opponent player.
         *
         * @param name The player name
         * @param stoneManager The stone manager on which the player can play
         * @param initialStonesLeftToTake The initial stones that the player can take
         * @param sumInFirstRound The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         */
        OpponentPlayer(QString name, StoneManager& stoneManager, int initialStonesLeftToTake,
                int sumInFirstRound, bool stonesInOneRow);

        /**
         * Virtual destructor to avoid warnings.
         */
        virtual ~OpponentPlayer();

        /**
         * Lets the player play.
         */
        virtual void play() = 0;

        /**
         * Stops the playing of the player until the player is destroyed.
         */
        void stopPlaying();

    protected:
        /**
         * Stores if the player should stop playing.
         */
        bool stopPlayerPlaying;
};

#endif /* OPPONENTPLAYER_H_ */
