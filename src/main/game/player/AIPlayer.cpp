/*
 * AIPlayer.cpp
 *
 *  Created on: 13.09.2011
 *      Author: Felix Wohlfrom
 */

#include "AIPlayer.h"

#include "../Settings.h"
#include "../stones/Gamestone.h"
#include "../stones/StoneManager.h"

#include <QThread>
#include <QElapsedTimer>
#include <QCoreApplication>

#ifdef _DEBUG
    #include <iostream>
#endif

AIPlayer::AIPlayer(int number, StoneManager& stoneManager, int initialStonesLeftToTake,
        int sumInFirstRound, bool stonesInOneRow) :
    OpponentPlayer(QString("%1").arg(number), stoneManager, initialStonesLeftToTake,
            sumInFirstRound, stonesInOneRow)
{}

void AIPlayer::play()
{
    #ifdef _DEBUG
        std::cout << "=======================| AI " << this->getPlayerName().toStdString();
        std::cout << " playing |========================" << std::endl;
    #endif
    if (stopPlayerPlaying) return;

    // A timer to make sure the information that ai plays is shown
    QElapsedTimer timer;
    timer.start();

    this->takeStones();
    if (stopPlayerPlaying) return;

    // First try to play out new rows
    this->playOutRowsUsingJustOwnStones(true); // ... with same color
    if (stopPlayerPlaying) return;
    this->playOutRowsUsingJustOwnStones(false); // ... with same number
    if (stopPlayerPlaying) return;

    // If already played out complete row, try to append own stones
    // to existing rows
    if (!this->firstPlayOut)
    {
        this->playOutStonesUsingStonesFromGameboard();
        if (stopPlayerPlaying) return;
        this->appendToExistingRows();
        if (stopPlayerPlaying) return;
    }

    #ifndef _DEBUG
        while (timer.elapsed() < 2000)
        {
            QThread::msleep(100);
            QCoreApplication::processEvents();
            if (stopPlayerPlaying) return;
        }
    #endif

    this->newRound();

    #ifdef _DEBUG
        printStones();
    #endif

    QCoreApplication::processEvents();
}

void AIPlayer::takeStones()
{
    #ifdef _DEBUG
        std::cout << "Take stones..." << std::endl;
    #endif

    QVector<Gamestone*> stones = stoneManager.getStones();
    while (stonesLeftToTake > 0 && stoneManager.getStoneCountOnHeap() > 0)
    {
        int index = rand() % stones.size();
        while (stones.at(index)->getParent() != Gamestone::HEAP)
        {
            index = rand() % stones.size();
        }

        this->addStone(*(stones[index]));
        stones.at(index)->setParent(Gamestone::AIHOLDER);
        stones.at(index)->finishedFirstTimePlayedOut();
    }

    #ifdef _DEBUG
        printStones();
    #endif

    QCoreApplication::processEvents();
}

void AIPlayer::clearRows()
{
    for (QVector<Gamestone*>::iterator stone(ownStones.begin());
            stone != ownStones.end();
            ++stone)
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
    for (QVector<Gamestone*>::iterator stone(ownStones.begin());
            stone != ownStones.end();
            ++stone)
    {
        if ((*stone)->isLastInRow())
        {
            for (QVector<Gamestone*>::iterator otherStone(ownStones.begin());
                    otherStone != ownStones.end();
                    ++otherStone)
            {
                if ((*stone)->acceptDropping(*(*otherStone))
                        && ((sameColor && (*stone)->isSameColor(*otherStone))
                            || (!sameColor && (*stone)->isSameNumber(*otherStone)))
                    )
                {
                    if (!(*stone)->appendStone(*(*otherStone)))
                    {
                        // This just happens if two single stones with same number should
                        // be combined
                        (*stone)->setNext(*otherStone);
                    }
                }
            }
        }
    }

    // If first time the player is playing out, check if the required sum is reached
    if (this->firstPlayOut)
    {
        // Stores the current maximum or sum of all rows, depending of stonesInOneRow setting
        int stoneSum(0);

        for (QVector<Gamestone*>::iterator stone(ownStones.begin());
                stone != ownStones.end();
                ++stone)
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

    QCoreApplication::processEvents();
}

void AIPlayer::playOutStonesUsingStonesFromGameboard()
{
    #ifdef _DEBUG
        std::cout << "=================================" << std::endl;
        std::cout << "Play out stones from gameboard..." << std::endl;
    #endif

    QVector<Gamestone*> stones = stoneManager.getStones();

    // For each stone, try if it can be added to a stone if the row is split in the middle
    for (QVector<Gamestone*>::iterator ownStone(ownStones.begin());
            ownStone != ownStones.end();
            ++ownStone)
    {
        #ifdef _DEBUG
            std::cout << "Own stone: " << (*ownStone) << std::endl;
        #endif

        for (QVector<Gamestone*>::iterator stone(stones.begin());
                stone != stones.end();
                ++stone)
        {
            // Just stones on gameboard need to be checked
            if ((*stone)->getParent() == Gamestone::BOARD)
            {
                if ((*stone)->isSameColor(*ownStone) &&
                    (*stone)->isRowWithSameColor() &&
                    (*stone)->isSucc(*ownStone))
                {
                    // We can just add if stones before are at least three and at least one stone
                    // after watched stone (ownStone + stone + one stone after = three stones)
                    if (this->countStonesBefore(**stone) >= 3
                            && this->countStonesAfter(**stone) >= 1)
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
    for (QVector<Gamestone*>::iterator stone(ownStones.begin());
            stone != ownStones.end();
            ++stone)
    {
        if ((*stone)->isFirstInRow() && (*stone)->countStonesInRow() > 2)
        {
            // Set new parent, position is set in MainWindow.cpp
            (*stone)->setParent(Gamestone::BOARD);
            (*stone)->moveAddedStones();

            QCoreApplication::processEvents();
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

    QVector<Gamestone*> stones = stoneManager.getStones();

    bool hasLayedOut;
    do
    {
        hasLayedOut = false;

        // For each stone on board which is first or last ...
        for (QVector<Gamestone*>::iterator stone(stones.begin());
                stone != stones.end();
                ++stone)
        {
            if ((*stone)->getParent() == Gamestone::BOARD &&
                    ((*stone)->isFirstInRow() || (*stone)->isLastInRow()))
            {
                // ... check if player can append own stone
                for (QVector<Gamestone*>::iterator ownStone(ownStones.begin());
                        ownStone != ownStones.end();
                        ++ownStone)
                {
                    if ((*stone)->acceptDropping(*(*ownStone)))
                    {
                        (*stone)->appendStone(*(*ownStone));
                        (*stone)->moveAddedStones();
                        hasLayedOut = true;

                        QCoreApplication::processEvents();
                    }
                }
            }
        }

    // Repeat while own stones have been laid out
    } while (hasLayedOut);
}
