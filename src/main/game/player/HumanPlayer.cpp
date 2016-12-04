/*
 * HumanPlayer.cpp
 *
 *  Created on: 10.08.2011
 *      Author: Felix Wohlfrom
 */

#include "HumanPlayer.h"

#include "../Settings.h"
#include "../stones/Gamestone.h"
#include "../stones/StoneManager.h"

HumanPlayer::HumanPlayer(StoneManager& stoneManager, int initialStonesLeftToTake,
        int sumInFirstRound, bool stonesInOneRow) :
    Player("", stoneManager, initialStonesLeftToTake, sumInFirstRound, stonesInOneRow)
{}

HumanPlayer::~HumanPlayer() {}

bool HumanPlayer::testStonesOnGameboard()
{
    resetStoneInvalidation();

    QVector<Gamestone*> stones = stoneManager.getStones();

    // If first round, check if required sum is reached
    if (this->firstPlayOut)
    {
        // Stores the current maximum or sum of all rows, depending of stonesInOneRow setting
        int stoneSum(0);
        bool playedOut(false);
        for (QVector<Gamestone*>::iterator stone(stones.begin());
                stone != stones.end();
                ++stone)
        {
            if ((*stone)->getParent() == Gamestone::BOARD)
            {
                if ((*stone)->isFirstInRow())
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

                if ((*stone)->getPlayer() == this)
                {
                    playedOut = true;
                }
            }
        }

        if (playedOut && stoneSum < sumInFirstRound)
        {
            invalidateOwnStonesOnBoard();
            return false;
        }
    }

    // Stores if all rows contain at least three stones
    bool allValid(true);
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        // Check if at least three stones are in each row
        if ((*stone)->getParent() == Gamestone::BOARD && (*stone)->isFirstInRow()
                && ((*stone)->countStonesInRow() < 3))
        {
            allValid = false;
            invalidateRow(*(*stone));
        }
    }

    return allValid;
}

void HumanPlayer::roundFinished()
{
    QVector<Gamestone*> stones = stoneManager.getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        (*stone)->storeStone();
    }
};

void HumanPlayer::resetStoneInvalidation()
{
    QVector<Gamestone*> stones = stoneManager.getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
            stone != stones.end();
            ++stone)
    {
        (*stone)->setInvalid(false);
        (*stone)->repaint();
    }
}

void HumanPlayer::invalidateOwnStonesOnBoard()
{
    for (QVector<Gamestone*>::iterator stone(ownStones.begin());
            stone != ownStones.end();
            ++stone)
    {
        if ((*stone)->getParent() == Gamestone::BOARD)
        {
            this->invalidateRow(*(*stone));
        }
    }
}

void HumanPlayer::invalidateRow(Gamestone& stone)
{
    stone.setInvalid(true);
    stone.repaint();

    // Invalidate next stones
    Gamestone* next(stone.getNext());
    while (next != NULL)
    {
        next->setInvalid(true);
        next->repaint();
        next = next->getNext();
    }

    // Invalidate previous stones
    Gamestone* prev(stone.getPrev());
    while (prev != NULL)
    {
        prev->setInvalid(true);
        prev->repaint();
        prev = prev->getPrev();
    }
}
