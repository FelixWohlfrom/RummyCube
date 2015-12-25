/*
 * Player.cpp
 *
 *  Created on: 09.08.2011
 *      Author: Felix Wohlfrom
 */

#include "Player.h"

#ifdef _DEBUG
	#include <iostream>
#endif

Player::Player(QString name, StoneManager& stoneManager, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow) :
	playerName(name), stonesLeftToTake(initialStonesLeftToTake), sumInFirstRound(sumInFirstRound), stonesInOneRow(stonesInOneRow),
	firstPlayOut(true), stoneManager(stoneManager), ownStones()
{}

Player::~Player() {};

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
	std::vector<Gamestone*>::iterator stone(ownStones.begin());
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
	for (QVector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
	{
		(*stone)->storeStone();
	}

	#ifdef _DEBUG
		std::cout << "===============================| Player " << this->playerName.toStdString() << ": Finished playing |==============================" << std::endl;
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

#if 0
wxDataInputStream &operator>>(wxDataInputStream &input, Player* player)
{
	// Load the settings
	input >> player->stonesLeftToTake >> player->sumInFirstRound;
	wxUint8 buf;
	input >> buf;
	player->stonesInOneRow = (buf != 0);
	input >> buf;
	player->firstPlayOut = (buf != 0);

	int stoneCount;
	input >> stoneCount;
	player->ownStones.clear();

	player->stonesLeftToTake += stoneCount; // This is needed because we decrement 
											// the stones left to take in addStone method
	while (stoneCount > 0)
	{
		int stoneValue;
		input >> stoneValue;
		Gamestone* stone = player->stoneManager.getStoneFromInt(stoneValue);
		input >> stone;
		player->addStone(*stone);

		--stoneCount;
	}

	return input;
}

wxDataOutputStream &operator<<(wxDataOutputStream &output, Player* player)
{
	// First store the settings
	// Store booleans as wxUint8
	output << player->stonesLeftToTake << player->sumInFirstRound;
	output << (wxUint8)player->stonesInOneRow << (wxUint8)player->firstPlayOut;

	// Store the stones
	output << (wxUint32)player->ownStones.size();
	for (std::vector<Gamestone*>::iterator stone(player->ownStones.begin()); stone != player->ownStones.end(); ++stone)
	{
		output << (*stone);
	}

	return output;
}
#endif

#ifdef _DEBUG
void Player::printStones()
{
	std::cout << "=====================| Stones of Player " << this->playerName.toStdString() << "|===============================" << std::endl;

	for (std::vector<Gamestone*>::iterator stone(ownStones.begin()); stone != ownStones.end(); ++stone)
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
