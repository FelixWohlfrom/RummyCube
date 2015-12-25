/*
 * StoneManager.cpp
 *
 *  Created on: 26.07.2011
 *      Author: Felix Wohlfrom
 */

#include "StoneManager.h"

#include "Joker.h"
#include "../Settings.h"

StoneManager::StoneManager() :
	draggedStone(NULL)
{
	// Load settings
	Settings* settings = Settings::getInstance();
	moveAllStonesInRow = settings->value("General/MoveAllStones", true).toBool();
}

StoneManager::~StoneManager()
{
	// Store settings
	Settings* settings = Settings::getInstance();
	settings->setValue("General/MoveAllStones", moveAllStonesInRow);

	// Clean up the stones
	while (!gamestones.empty())
	{
		delete gamestones.back();
		gamestones.pop_back();
		// TODO Rework to use signals?
		// wxYield();
	}
}

void StoneManager::createStones(QWidget* heap)
{
	// Create a bunch of stones
	for (int i(0); i < 2; ++i)
	{
		for (int j(1); j <= 13; ++j)
		{
			gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::BLACK, j));
			gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::BLUE, j));
			gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::RED, j));
			gamestones.push_back(new Gamestone(heap, this, i == 0, Gamestone::YELLOW, j));
		}
	}

	// Create the jockers
	gamestones.push_back(new Joker(heap, this, true));
	gamestones.push_back(new Joker(heap, this, false));
}

QVector<Gamestone*>& StoneManager::getStones()
{
	return gamestones;
}

int StoneManager::getStoneCountOnHeap() const
{
	int count(0);
	for (QVector<Gamestone*>::const_iterator it(gamestones.begin()); it != gamestones.end(); ++it)
	{
		if ((*it)->getParent() == Gamestone::HEAP)
		{
			++count;
		}
	}

	return count;
}

Gamestone* StoneManager::getStoneDraggingOver(Gamestone& current) const
{
// TODO Reimplement?
/*	wxPoint mousePos(current.GetParent()->ScreenToClient(wxGetMousePosition()));

	for (std::vector<Gamestone*>::const_iterator it(gamestones.begin()); it != gamestones.end(); ++it)
	{
		if (&current != (*it) && current.GetParent() == (*it)->GetParent() &&
				mousePos.x > (*it)->GetPosition().x && mousePos.x < (*it)->GetPosition().x + (*it)->getWidth() &&
				mousePos.y > (*it)->GetPosition().y && mousePos.y < (*it)->GetPosition().y + (*it)->getHeight())
		{
			// TODO Handle overlapping stones
			return (*it);
		}
	}*/

	return NULL;
}

Gamestone* StoneManager::getStoneFromInt(int value) const
{
	// The masks to extract the needed values
	#define FIRSTMASK 1
	#define JOKERMASK 2
	#define COLORMASK 124

	bool isFirst = (value & FIRSTMASK) != 0;
	bool isJoker = (value & JOKERMASK) != 0;
	Gamestone::StoneColor color = (Gamestone::StoneColor)(((value & COLORMASK) >> 3) - 1);
	int number = (value >> 8) - 1;

	for (QVector<Gamestone*>::const_iterator it(gamestones.begin()); it != gamestones.end(); ++it)
	{
		// If we need to return a joker, color and number are not needed
		if (isJoker)
		{
			if ((*it)->isJoker() && (*it)->isFirst() == isFirst)
			{
				return *it;
			}
		}
		// Check for color and number
		else
		{
			if (!(*it)->isJoker() && (*it)->isFirst() == isFirst &&
					(*it)->getColor() == color && (*it)->getNumber() == number)
			{
				return *it;
			}
		}
	}

	return NULL;
}
