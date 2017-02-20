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
 * RummyCube.h
 *
 *  Created on: 22.07.2011
 *      Author: Felix Wohlfrom
 */

#ifndef RUMMYCUBE_H_
#define RUMMYCUBE_H_

#include <deque>

#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include "CountdownTimer.h"

#include "player/AIPlayer.h"
#include "player/HumanPlayer.h"
#include "stones/StoneManager.h"

/**
 * The basic game model for singleplayer games. Also base class for multiplayer games or tutorials.
 */
class RummyCube: QObject
{
    public:
        /**
         * Constructor. Creates a new single player game.
         */
        RummyCube();

        /**
         * Destructor. Cleans up all stuff created during game.
         */
        virtual ~RummyCube();

        /**
         * Returns the human player.
         *
         * @return a pointer to the human player
         */
        HumanPlayer* getHumanPlayer() const;

        /**
         * Returns the list with the opponent players.
         *
         * @return a pointer to the opponent player list
         */
        std::deque<OpponentPlayer*> getOpponentPlayers() const;

        /**
         * Returns the timer which is used for time limits.
         *
         * @return a pointer to the used timer
         */
        CountdownTimer* getTimer() const;

        /**
         * Returns the stone manager that manages the stones of this game.
         *
         * @return a reference to the stone manager
         */
        StoneManager& getStoneManager();

        /**
         * Test the stones on gameboard.
         *
         * @return true If all stones passed the tests
         */
        bool testStonesOnGameboard();

        /**
         * This method is called when the round of the human player has been finished. It will
         * notify other players and set all stones on the gameboard as first time played out.
         */
        void roundFinished();

        /**
         * Stops the current game.
         */
        void stopGame();

        /**
         * Loads the game values from given stream.
         *
         * @param input The stream from which should be loaded
         * @param game The game that should be loaded
         */
        friend QXmlStreamReader &operator>>(QXmlStreamReader &input, RummyCube* game);


        /**
         * Saves a game to given stream.
         *
         * @param output The stream on which should be stored
         * @param game The game that should be stored
         */
        friend QXmlStreamWriter &operator<<(QXmlStreamWriter &output, RummyCube* game);

    protected:
        /**
         * The countdown timer that limits the time for the player(s).
         */
        CountdownTimer* timer;

        /**
         * The human player shown in the window.
         */
        HumanPlayer* humanPlayer;

        /**
         * The opponent players.
         */
        std::deque<OpponentPlayer*> opponentPlayers;

        /**
         * The stone manager which manages the stones of current game.
         */
        StoneManager stoneManager;
};

#endif /* RUMMYCUBE_H_ */
