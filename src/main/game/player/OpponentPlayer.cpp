/*
 * OpponentPlayer.cpp
 *
 *  Created on: 08.03.2012
 *      Author: felix
 */

#include "OpponentPlayer.h"

OpponentPlayer::OpponentPlayer(wxString name, StoneManager& stoneManager, int initialStonesLeftToTake, int sumInFirstRound, bool stonesInOneRow) :
	Player(name, stoneManager, initialStonesLeftToTake, sumInFirstRound, stonesInOneRow), stopPlayerPlaying(false)
{};

OpponentPlayer::~OpponentPlayer() {};

void OpponentPlayer::stopPlaying()
{
	stopPlayerPlaying = true;
}
