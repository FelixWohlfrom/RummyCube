/*
 * AIPlayer.cpp
 *
 *  Created on: 13.09.2011
 *      Author: Felix Wohlfrom
 */

#include "AIPlayer.h"

#include <wx/utils.h>
#include <wx/stopwatch.h>

#include "../Settings.h"
#include "../stones/Gamestone.h"
#include "../stones/StoneManager.h"

#ifdef _DEBUG
	#include <iostream>
#endif

// This method tests, if a stone is just previous or next stone of at most one other stone.
// Ai player sometime runs in this annoying bug.
bool allStonesPrevOrNextWithOne(StoneManager& stoneManager)
{
	std::vector<Gamestone*> stones = stoneManager.getStones();
	for (std::vector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
	{
		int prevCount(0);
		int nextCount(0);

		for (std::vector<Gamestone*>::iterator otherStone(stones.begin()); otherStone != stones.end(); ++otherStone)
		{
			if ((*otherStone)->getPrev() == (*stone))
			{
				++prevCount;
			}

			if ((*otherStone)->getNext() == (*stone))
			{
				++nextCount;
			}
		}

		if (prevCount > 1 || nextCount > 1)
		{
			return false;
		}
	}

	return true;
}

AIPlayer::AIPlayer(int number, StoneManager& stoneManager, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow) :
	OpponentPlayer(wxString::Format(_T("%d"), number), stoneManager, initialStonesLeftToTake, sumInFirstRound, stonesInOneRow)
{}

void AIPlayer::play()
{
	#ifdef _DEBUG
		std::cout << "=======================| AI " << this->getPlayerName().mb_str() << " playing |========================" << std::endl;
	#endif
	if (stopPlayerPlaying) return;

	// A timer to make sure the information that ai plays is shown
	wxStopWatch timer;

	this->takeStones();
	if (stopPlayerPlaying) return;

	// First try to play out new rows
	this->playOutRowsUsingJustOwnStones(true); // ... with same color
	wxASSERT(allStonesPrevOrNextWithOne(stoneManager));
	if (stopPlayerPlaying) return;
	this->playOutRowsUsingJustOwnStones(false); // ... with same number
	wxASSERT(allStonesPrevOrNextWithOne(stoneManager));
	if (stopPlayerPlaying) return;

	// If already played out complete row, try to append own stones to existing rows
	if (!this->firstPlayOut)
	{
		this->playOutStonesUsingStonesFromGameboard();
		wxASSERT(allStonesPrevOrNextWithOne(stoneManager));
		if (stopPlayerPlaying) return;
		this->appendToExistingRows();
		wxASSERT(allStonesPrevOrNextWithOne(stoneManager));
		if (stopPlayerPlaying) return;
	}

	#ifndef _DEBUG
		while (timer.Time() < 4000)
		{
			wxMilliSleep(100);
			wxYield();
			if (stopPlayerPlaying) return;
		}
	#endif

	this->newRound();

	#ifdef _DEBUG
		printStones();
	#endif

	wxYield();
}

void AIPlayer::takeStones()
{
	#ifdef _DEBUG
		std::cout << "Take stones..." << std::endl;
	#endif

	// For testing, just take some special stones.
	/*int indizies[] = {
		4,  // 2 (Black) 1
		43, // 11 (Yellow) 1
		88, // 10 (Black) 0
		6,  // 2 (Red) 1
		76, // 7 (Black) 0
		64, // 4 (Black) 0
		33, // 9 (Blue) 1
		55, // 1 (Yellow) 0
		40, // 11 (Black) 1
		50, // 13 (Red) 1
		90, // 10 (Red) 0
		84, // 9 (Black) 0
		30, // 8 (Red) 1
		22 // 6 (Red) 1
	};
	int i(0);*/

	std::vector<Gamestone*> stones = stoneManager.getStones();
	while (stonesLeftToTake > 0 && stoneManager.getStoneCountOnHeap() > 0)
	{
		int index = rand() % stones.size();
		while (stones.at(index)->getParent() != Gamestone::HEAP)
		{
			index = rand() % stones.size();
		}
		// For testing
		/*int index = indizies[i];
		++i;*/

		this->addStone(*(stones[index]));
		stones.at(index)->setParent(Gamestone::AIHOLDER);
	}

	#ifdef _DEBUG
		printStones();
	#endif

	wxYield();
}

void AIPlayer::clearRows()
{
	for (std::vector<Gamestone*>::iterator stone(ownStones.begin()); stone != ownStones.end(); ++stone)
	{
		(*stone)->setNext(NULL);
		(*stone)->setPrev(NULL);
	}
}

void AIPlayer::playOutRowsUsingJustOwnStones(bool sameColor)
{
	#ifdef _DEBUG
		std::cout << "==================================" << std::endl;
		std::cout << "Build rows just with own stones..." << std::endl;
	#endif

	// Build up rows out of own stones
	for (std::vector<Gamestone*>::iterator stone(ownStones.begin()); stone != ownStones.end(); ++stone)
	{
		if ((*stone)->isLastInRow())
		{
			for (std::vector<Gamestone*>::iterator otherStone(ownStones.begin()); otherStone != ownStones.end(); ++otherStone)
			{
				if ((*stone)->acceptDropping(*(*otherStone)) &&
						((sameColor && (*stone)->isSameColor(*otherStone)) ||
						 (!sameColor && (*stone)->isSameNumber(*otherStone)))
					)
				{
					if (!(*stone)->appendStone(*(*otherStone)))
					{
						// This just happens if two single stones with same number should be combined
						(*stone)->setNext(*otherStone);
					}
				}
			}
		}
	}

	// If first time the player is playing out, check if the required sum is reached
	if (this->firstPlayOut)
	{
		int stoneSum(0); // Stores the current maximum or sum of all rows, depending of stonesInOneRow setting
		for (std::vector<Gamestone*>::iterator stone(ownStones.begin()); stone != ownStones.end(); ++stone)
		{
			if ((*stone)->isFirstInRow() &&
					// Don't calculate stones which don't have enough stones
					(*stone)->countStonesInRow() > 2)
			{
				if (stonesInOneRow)
				{
					stoneSum = std::max(stoneSum, this->calcSumInRow(*(*stone)));
				}
				else
				{
					stoneSum += this->calcSumInRow(*(*stone));
				}
			}
		}

		if (stoneSum > sumInFirstRound)
		{
			this->playOutRows();
			this->firstPlayOut = false;
		}
	}

	// If not first time the player is playing out, always play out rows
	else
	{
		this->playOutRows();
	}

	// Finally clean up the built rows
	this->clearRows();

	wxYield();
}

void AIPlayer::playOutStonesUsingStonesFromGameboard()
{
	#ifdef _DEBUG
		std::cout << "=================================" << std::endl;
		std::cout << "Play out stones from gameboard..." << std::endl;
	#endif

	std::vector<Gamestone*> stones = stoneManager.getStones();

	// For each stone, try if it can be added to a stone if the row is splitted in the middle
	for (std::vector<Gamestone*>::iterator ownStone(ownStones.begin()); ownStone != ownStones.end(); ++ownStone)
	{
		#ifdef _DEBUG
			std::cout << "Own stone: " << (*ownStone) << std::endl;
		#endif

		for (std::vector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
		{
			// Just stones on gameboard need to be checked
			if ((*stone)->getParent() == Gamestone::BOARD)
			{
				if ((*stone)->isSameColor(*ownStone) &&
					(*stone)->isRowWithSameColor() &&
					(*stone)->isSucc(*ownStone))
				{
					// We can just add, if stones before are at least three and at least one stone
					// after watched stone (ownStone + stone + one stone after = three stones)
					if (this->countStonesBefore(**stone) >= 3 && this->countStonesAfter(**stone) >= 1)
					{
						(*stone)->setPrev(*ownStone);
						(*ownStone)->setParent(Gamestone::BOARD);
						(*ownStone)->moveAddedStones();
					}
				}
			}
		}
	}

	// Clear the stones which are played out
	this->clearOwnStones();

	#ifdef _DEBUG
		std::cout << "=================================" << std::endl;
	#endif
}

int AIPlayer::countStonesBefore(Gamestone& stone)
{
	int count(0);

	Gamestone* prevStone(stone.getPrev());
	while (prevStone != NULL)
	{
		++count;
		prevStone = prevStone->getPrev();
	}

	return count;
}

int AIPlayer::countStonesAfter(Gamestone& stone)
{
	int count(0);

	Gamestone* nextStone(stone.getNext());
	while (nextStone != NULL)
	{
		++count;
		nextStone = nextStone->getNext();
	}

	return count;
}

void AIPlayer::playOutRows()
{
	for (std::vector<Gamestone*>::iterator stone(ownStones.begin()); stone != ownStones.end(); ++stone)
	{
		if ((*stone)->isFirstInRow() && (*stone)->countStonesInRow() > 2)
		{
			// Set new parent, position is set in MainWindow.cpp
			(*stone)->setParent(Gamestone::BOARD);
			(*stone)->moveAddedStones();

			wxYield();
		}
	}

	// Clear all stones which are now played out
	clearOwnStones();
}

void AIPlayer::appendToExistingRows()
{
	#ifdef _DEBUG
		std::cout << "===================" << std::endl;
		std::cout << "Appending stones..." << std::endl;
	#endif

	std::vector<Gamestone*> stones = stoneManager.getStones();

	bool hasLayedOut;
	do
	{
		hasLayedOut = false;

		// For each stone on board which is first or last ...
		for (std::vector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
		{
			if ((*stone)->getParent() == Gamestone::BOARD &&
					((*stone)->isFirstInRow() || (*stone)->isLastInRow()))
			{
				// ... check if player can append own stone
				for (std::vector<Gamestone*>::iterator ownStone(ownStones.begin()); ownStone != ownStones.end(); ++ownStone)
				{
					if ((*stone)->acceptDropping(*(*ownStone)))
					{
						(*stone)->appendStone(*(*ownStone));
						(*stone)->moveAddedStones();
						hasLayedOut = true;

						wxYield();
					}
				}
			}
		}

	// Repeat while own stones have been layed out
	} while (hasLayedOut);
}
