/*
 * HumanPlayerClient.h
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef HUMANPLAYERCLIENT_H_
#define HUMANPLAYERCLIENT_H_

#include "HumanPlayer.h"

#include <QTcpSocket>

/**
 * Represents a human player on client side during network game.
 */
class HumanPlayerClient: public HumanPlayer
{
    public:
        /**
         * Creates a new human player on server on network game.
         *
         * @param stoneManager The stone manager on which the player can play
         * @param sock The socket which the player uses for communication
         * @param stonesAtBeginning The initial stones that the player can take
         * @param sumAtBeginning The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         */
        HumanPlayerClient(StoneManager& stoneManager, QTcpSocket& sock, int stonesAtBeginning,
                int sumAtBeginning, bool stonesInOneRow);

        /**
         * Dummy to avoid warnings.
         */
        virtual ~HumanPlayerClient();

        /**
        * Initializes the player for the next round.
        */
        void newRound();

        /**
         * Notifies other players that round is finished.
         */
        void roundFinished();

    private:
        /**
         * The socket on which the network player works.
         */
        QTcpSocket& sock;

        /**
         * Stores if the first round has already been played or
         * if the player needs to be reset on new round.
         */
        bool isFirstRound;
};

#endif /* HUMANPLAYERCLIENT_H_ */
