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
 * RummyCube.cpp
 *
 *  Created on: 22.07.2011
 *      Author: Felix Wohlfrom
 */
#include "RummyCube.h"

#include "Settings.h"
#include "stones/Joker.h"

#include <QFile>

RummyCube::RummyCube() : opponentPlayers(), stoneManager()
{
    // Load settings
    Settings* settings = Settings::getInstance();
    int initialStonesToTake = settings->value("General/StonesAtBeginning", 14).toInt();
    int sumInFirstRound = settings->value("General/SumAtBeginning", 27).toInt();
    bool stonesInOneRow = settings->value("General/StonesInOneRow", true).toBool();
    int aiPlayerCount = settings->value("General/AIPlayers", 1).toInt();

    // Initialize the timer
    bool timeLimitEnabled = settings->value("General/TimeLimit", false).toBool();
    int timeLimit = settings->value("General/TimeLimitTime", -1).toInt();

    if (timeLimitEnabled)
    {
        timer = new CountdownTimer(timeLimit);
    }
    else
    {
        timer = new CountdownTimer( -1);
    }

    // Initalize the human player
    humanPlayer = new HumanPlayer(stoneManager, initialStonesToTake,
            sumInFirstRound, stonesInOneRow);

    // Initialize the ai players
    while (aiPlayerCount > 0)
    {
        opponentPlayers.push_front(new AIPlayer(aiPlayerCount, stoneManager,
                initialStonesToTake, sumInFirstRound, stonesInOneRow));
        --aiPlayerCount;
    }
}

RummyCube::~RummyCube()
{
    // clean up the stuff
    delete timer;
    delete humanPlayer;
    while (!opponentPlayers.empty())
    {
        delete opponentPlayers.back();
        opponentPlayers.pop_back();
    }
}

HumanPlayer* RummyCube::getHumanPlayer() const
{
    return humanPlayer;
}

std::deque<OpponentPlayer*> RummyCube::getOpponentPlayers() const
{
    return opponentPlayers;
}

CountdownTimer* RummyCube::getTimer() const
{
    return timer;
}

void RummyCube::createStones(QWidget* heap)
{
    stoneManager.createStones(heap);
}

QVector<Gamestone*>& RummyCube::getStones()
{
    return stoneManager.getStones();
}

int RummyCube::getStoneCountOnHeap() const
{
    return stoneManager.getStoneCountOnHeap();
}

bool RummyCube::moveAllStonesInRow() const
{
    return stoneManager.moveAllStonesInRow;
}

void RummyCube::moveAllStonesInRow(bool moveAllStonesInRow)
{
    stoneManager.moveAllStonesInRow = moveAllStonesInRow;
}

bool RummyCube::testStonesOnGameboard()
{
    return humanPlayer->testStonesOnGameboard();
}

void RummyCube::roundFinished()
{
    humanPlayer->roundFinished();

    // Set all stones on the gameboard as already played out
    QVector<Gamestone*> stones = stoneManager.getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        if ((*stone)->getParent() == Gamestone::StoneParent::BOARD)
        {
            (*stone)->finishedFirstTimePlayedOut();
        }
    }
}

void RummyCube::stopGame()
{
    for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin());
            player != opponentPlayers.end();
            ++player)
    {
        (*player)->stopPlaying();
    }
}

QXmlStreamReader &operator>>(QXmlStreamReader &input, RummyCube* game)
{
    if (!input.readNextStartElement() || input.name() != "RummyCube")
    {
        input.raiseError(QObject::tr("Not a valid RummyCube Savegame."));
        return input;
    }

    // Initialize the timer values
    int maxTime = input.attributes().value("time").toInt();
    int timeLeft = input.attributes().value("timeLeft").toInt();
    int aiPlayerCount = input.attributes().value("opponents").toInt();
    delete game->timer;
    game->timer = new CountdownTimer(maxTime);

    // Load human player
    input >> game->humanPlayer;

    // Delete old players and load new ones
    for (std::deque<OpponentPlayer*>::iterator player(game->opponentPlayers.begin());
            player != game->opponentPlayers.end();
            ++player)
    {
        delete (*player);
    }
    game->opponentPlayers.clear();

    for (int i = 1; i <= aiPlayerCount; i++)
    {
        AIPlayer* player = new AIPlayer(i, game->stoneManager);
        input >> player;
        game->opponentPlayers.push_back(player);
    }

    // Load the stones without players
    // next start element is already read in previous while loop
    if (!input.readNextStartElement() || input.name() != "otherStones")
    {
        if (!input.hasError())
        {
            input.raiseError(QObject::tr("Expected 'otherStones' Tag, "
                    "instead found '%1'.").arg(input.name().toString()));
        }
        return input;
    }

    while (input.readNextStartElement())
    {
        if (input.name() == "stone")
        {
            int stoneValue = input.attributes().value("val").toInt();
            if (stoneValue != 0)
            {
                Gamestone* stone = game->stoneManager.getStoneFromInt(stoneValue);
                input >> stone;
            }
        }
        else if (input.name() == "joker")
        {
            int stoneValue = input.attributes().value("val").toInt();
            if (stoneValue != 0)
            {
                Gamestone* stone = game->stoneManager.getStoneFromInt(stoneValue);
                input >> (Joker*)stone;
            }
        } else {
            input.raiseError(QObject::tr("Expected 'stone' Tag or 'joker' Tag, "
                                "instead found '%1'.").arg(input.name().toString()));
            break;
        }
    }

    // Finally start the timer
    game->timer->start(maxTime- timeLeft);

    return input;
}

QXmlStreamWriter &operator<<(QXmlStreamWriter &output, RummyCube* game)
{
    output.setAutoFormatting(true);

    output.writeStartDocument();
    output.writeDTD("<!DOCTYPE Rummy>");
    output.writeStartElement("RummyCube");

    // Store the current game settings
    // Initial stones to take and sum in first round is not needed to save
    // They are saved for each player on its own
    output.writeAttribute("time", QString::number(game->timer->getTime()));
    output.writeAttribute("timeLeft", QString::number(game->timer->timeLeft()));

    // Store the players
    output.writeAttribute("opponents",
            QString::number(game->opponentPlayers.size()));
    output << game->humanPlayer;
    for (std::deque<OpponentPlayer*>::iterator
            player(game->opponentPlayers.begin());
            player != game->opponentPlayers.end();
            ++player)
    {
        output << (*player);
    }

    // Store the stones without players
    output.writeStartElement("otherStones");
    QVector<Gamestone*> stones = game->stoneManager.getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        if ((*stone)->getPlayer() == NULL)
        {
            if ((*stone)->isJoker()) {
                output << (Joker*)(*stone);
            }
            else
            {
                output << (*stone);
            }
        }
    }
    output.writeEndElement();
    output.writeEndDocument();

    return output;
}
