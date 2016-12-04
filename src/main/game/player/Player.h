/*
 * Player.h
 *
 *  Created on: 09.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "../stones/StoneManager.h"

/**
 * This class handles the players.
 */
class Player
{
    public:
        /**
         * Creates a new player.
         *
         * @param name The player name
         * @param stoneManager The stone manager on which the player can play
         * @param initialStonesLeftToTake The initial stones that the player can take
         * @param sumInFirstRound The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         */
        Player(QString name, StoneManager& stoneManager, int initialStonesLeftToTake,
                int sumInFirstRound, bool stonesInOneRow);

        /**
         * Virtual destructor to avoid warnings.
         */
        virtual ~Player();

        /**
         * Returns the name of the player as QString.
         *
         * @return The name of the player
         */
        QString getPlayerName() const;

        /**
         * Returns the stones the player has left to take.
         */
        int getStonesLeftToTake() const;

        /**
         * Add a stone to the player
         *
         * @param stone The stone to add
         */
        void addStone(Gamestone& stone);

        /**
         * Checks if the player has won.
         * Clears played out stones.
         *
         * @return If the player has won or not
         */
        virtual bool hasWon();

        /**
         * Clears the stones that are played out.
         * Sets the firstPlayOut variable to false
         */
        void clearOwnStones();

        /**
         * Initializes the player for the next round.
         */
        virtual void newRound();

        /**
         * Loads the player settings and stones from given stream.
         *
         * @param input The stream from which should be loaded
         * @param player The player that should be loaded
         */
        friend QXmlStreamReader &operator>>(QXmlStreamReader &input, Player* player);

        /**
         * Saves the player settings and stones in given stream.
         *
         * @param output The stream on which should be stored
         * @param player The player that should be stored
         */
        friend QXmlStreamWriter &operator<<(QXmlStreamWriter &output, Player* player);

    private:
        /**
         * The player name.
         */
        QString playerName;

    protected:
        /**
         * The stones that the player still can take.
         */
        int stonesLeftToTake;

        /**
         * The sum that is needed for the first time a player plays out to board.
         */
        int sumInFirstRound;

        /**
         * If the sum needs to be in one row or also can be split in multiple rows.
         */
        bool stonesInOneRow;

        /**
         * If is first time the player plays out.
         */
        bool firstPlayOut;

        /**
         * The stone manager which manages the stones of the player.
         */
        StoneManager& stoneManager;

        /**
         * The gamestones managed by this player.
         */
        QVector<Gamestone*> ownStones;

        /**
         * Calculate the sum in a row of given stone. Only stones which are owned
         * by current player are counted.
         *
         * @param stone The stone of which the row sum should be calculated
         * @return The sum in the row
         */
        int calcSumInRow(Gamestone& stone) const;

        #ifdef _DEBUG
            /**
             * Prints out the stones on console for debugging.
             */
            void printStones();
        #endif
};

#endif /* PLAYER_H_ */
