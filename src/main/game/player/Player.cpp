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
 * Player.cpp
 *
 *  Created on: 09.08.2011
 *      Author: Felix Wohlfrom
 */

#include "Player.h"

#include "../stones/Joker.h"

#ifdef _DEBUG
    #include <iostream>
#endif

Player::Player(QString name, StoneManager& stoneManager, int initialStonesLeftToTake,
        int sumInFirstRound, bool stonesInOneRow) :
    playerName(name), stonesLeftToTake(initialStonesLeftToTake), sumInFirstRound(sumInFirstRound),
    stonesInOneRow(stonesInOneRow), firstPlayOut(true), stoneManager(stoneManager), ownStones()
{}

Player::~Player() {}

QString Player::getPlayerName() const
{
    return playerName;
}

int Player::getStonesLeftToTake() const
{
    return stonesLeftToTake;
}

void Player::addStone(Gamestone& stone)
{
    --stonesLeftToTake;
    ownStones.push_back(&stone);
    stone.setPlayer(this);
}

bool Player::hasWon()
{
    this->clearOwnStones(); // first clear played out stones
    return (this->ownStones.size() == 0);
}

void Player::clearOwnStones()
{
    QVector<Gamestone*>::iterator stone(ownStones.begin());
    while (stone != ownStones.end())
    {
        if ((*stone)->getParent() == Gamestone::BOARD)
        {
            (*stone)->setPlayer(NULL);
            stone = ownStones.erase(stone);
            this->firstPlayOut = false;
        }
        else
        {
            ++stone;
        }
    }
}

void Player::newRound()
{
    stonesLeftToTake = 1;
    clearOwnStones(); // Also sets the firstPlayOut variable

    QVector<Gamestone*> stones = stoneManager.getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        (*stone)->storeStone();
    }

    #ifdef _DEBUG
        std::cout << "===============================| Player " << this->playerName.toStdString();
        std::cout << ": Finished playing |==============================" << std::endl;
    #endif
}

int Player::calcSumInRow(Gamestone& stone) const
{
    int sum(stone.getNumber()); // Sum contains at least stone number

    // Count next stones
    Gamestone* next = stone.getNext();
    while (next != NULL)
    {
        // Calculate just rows which we played out completely
        if (next->getPlayer() != this)
        {
            return 0;
        }

        sum += next->getNumber();
        next = next->getNext();
    }

    // Count previous stones
    Gamestone* prev = stone.getPrev();
    while (prev != NULL)
    {
        // Calculate just rows which we played out completely
        if (prev->getPlayer() != this)
        {
            return 0;
        }

        sum += prev->getNumber();
        prev = prev->getPrev();
    }

    return sum;
}

QXmlStreamReader &operator>>(QXmlStreamReader &input, Player* player)
{
    if (!input.readNextStartElement() || input.name() != "player")
    {
        if (!input.hasError())
        {
            input.raiseError(QObject::tr("Expected 'player' Tag,"
                    " instead found '%1'.").arg(input.name().toString()));
        }
        return input;
    }

    // Load the settings
    player->stonesLeftToTake = input.attributes().value("stonesLeftToTake").toInt();
    player->sumInFirstRound = input.attributes().value("sumInFirstRound").toInt();
    player->stonesInOneRow = input.attributes().value("stonesInOneRow").toInt() == 1;
    player->firstPlayOut = input.attributes().value("firstPlayOut").toInt() == 1;

    // Load the stones
    player->ownStones.clear();

    int stoneCount = input.attributes().value("stoneCount").toInt();
    if (stoneCount != 0)
    {
        player->stonesLeftToTake += stoneCount; // This is needed because we decrement 
                                                // the stones left to take in addStone method

        while (stoneCount > 0)
        {
            if (!input.readNextStartElement()
                || ((input.name() != "stone") && (input.name() != "joker")))
            {
                if (!input.hasError())
                {
                    input.raiseError(QObject::tr("Expected 'stone' Tag or 'joker' Tag,"
                            " instead found '%1'.").arg(input.name().toString()));
                }
                return input;
            }
            int stoneValue = input.attributes().value("val").toInt();
            if (stoneValue != 0)
            {
                Gamestone* stone = player->stoneManager.getStoneFromInt(stoneValue);
                if (stone->isJoker())
                {
                    input >> (Joker*)stone;
                }
                else
                {
                    input >> stone;
                }
                player->addStone(*stone);
            }

            --stoneCount;
        }
    }
    
    input.skipCurrentElement(); // Finish handling of "player" element

    return input;
}

QXmlStreamWriter &operator<<(QXmlStreamWriter &output, Player* player)
{
    // First store the settings
    output.writeStartElement("player");
    output.writeAttribute("stonesLeftToTake", QString::number(player->stonesLeftToTake));
    output.writeAttribute("sumInFirstRound", QString::number(player->sumInFirstRound));
    output.writeAttribute("stonesInOneRow", QString::number(player->stonesInOneRow));
    output.writeAttribute("firstPlayOut", QString::number(player->firstPlayOut));

    // Store the stones
    output.writeAttribute("stoneCount", QString::number(player->ownStones.length()));
    for (QVector<Gamestone*>::iterator stone(player->ownStones.begin());
            stone != player->ownStones.end();
            ++stone)
    {
        if ((*stone)->isJoker()) {
            output << (Joker*)(*stone);
        }
        else
        {
            output << (*stone);
        }
    }

    output.writeEndElement();

    return output;
}

#ifdef _DEBUG
void Player::printStones()
{
    std::cout << "=====================| Stones of Player " << this->playerName.toStdString();
    std::cout << "|===============================" << std::endl;

    for (QVector<Gamestone*>::iterator stone(ownStones.begin());
            stone != ownStones.end();
            ++stone)
    {
        if ((*stone)->isFirstInRow())
        {
            std::cout << *stone;
            Gamestone* next(*stone);
            while (next->getNext() != NULL)
            {
                next = next->getNext();
                std::cout << " - " << next;
            }

            std::cout << std::endl;
        }
    }

    std::cout << "==========================================================" << std::endl;
}
#endif
