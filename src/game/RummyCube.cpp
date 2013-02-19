/*
 * RummyCube.cpp
 *
 *  Created on: 22.07.2011
 *      Author: Felix Wohlfrom
 */
#include "RummyCube.h"

#include "Settings.h"

#include <wx/datstrm.h>
#include <wx/wfstream.h>

RummyCube::RummyCube() :
	opponentPlayers(), stoneManager()
{
	// Load settings
	Settings* settings = Settings::getInstance();
	int initialStonesToTake;
	settings->Read(_T("General/StonesAtBeginning"), &initialStonesToTake, 14);
	int sumInFirstRound;
	settings->Read(_T("General/SumAtBeginning"), &sumInFirstRound, 27);
	bool stonesInOneRow;
	settings->Read(_T("General/StonesInOneRow"), &stonesInOneRow, true);
	int aiPlayerCount;
	settings->Read(_T("General/AIPlayers"), &aiPlayerCount, 1);
	bool timerEnabled;
	settings->Read(_T("General/TimeLimit"), &timerEnabled, false);

	// Initialize the timer
	bool timeLimitEnabled;
	settings->Read(_T("General/TimeLimit"), &timeLimitEnabled);
	int timeLimit;
	settings->Read(_T("General/TimeLimitTime"), &timeLimit);

	if (timeLimitEnabled)
	{
		timer = new CountdownTimer(timeLimit);
	}
	else
	{
		timer = new CountdownTimer(-1);
	}

	// Initalize the human player
	humanPlayer = new HumanPlayer(stoneManager, initialStonesToTake, sumInFirstRound, stonesInOneRow);

	// Initialize the ai players
	while (aiPlayerCount > 0)
	{
		opponentPlayers.push_front(new AIPlayer(aiPlayerCount, stoneManager, initialStonesToTake, sumInFirstRound, stonesInOneRow));
		--aiPlayerCount;
	}
}

RummyCube::~RummyCube()
{
	// clean up the stuff
	wxDELETE(timer);
	wxDELETE(humanPlayer);
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

void RummyCube::createStones(wxPanel* heap)
{
	stoneManager.createStones(heap);
}

std::vector<Gamestone*>& RummyCube::getStones()
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
}

void RummyCube::stopGame()
{
	for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
	{
		(*player)->stopPlaying();
	}
}

wxString RummyCube::loadGame(wxString filename)
{
	wxFileInputStream input(filename);
	wxDataInputStream loadGame(input);

	if (!loadGame.IsOk())
	{
		return _("File could not be opened");
	}

	// Check if we have a rummy cube savegame version 2
	wxUint16 buf;
	loadGame >> buf;
	if (buf != 7)
	{
		return _("The file that should be loaded is no RummyCube savegame");
	}

	// Check for savegame version
	int version;
	loadGame >> version;
	if (version > 2)
	{
		return _("Wrong savegame version");
	}

	// Initialize the timer values
	int timerMaxTime, timerTimeLeft;
	loadGame >> timerMaxTime >> timerTimeLeft;
	wxDELETE(timer);
	timer = new CountdownTimer(timerMaxTime);

	// Delete old players and load new ones
	for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
	{
		delete (*player);
	}
	opponentPlayers.clear();

	int aiPlayerCount;
	loadGame >> aiPlayerCount;
	loadGame >> humanPlayer;

	while (aiPlayerCount > 0)
	{
		AIPlayer* player = new AIPlayer(aiPlayerCount, stoneManager);
		loadGame >> player;
		opponentPlayers.push_front(player);
		--aiPlayerCount;
	}

	// Load the stones without players
	int value;
	Gamestone* stone;
	while (!input.Eof())
	{
		loadGame >> value;
		if (value != 0)
		{
			stone = stoneManager.getStoneFromInt(value);
			loadGame >> stone;
		}
	}

	// Finally start the timer
	timer->start(timerMaxTime - timerTimeLeft);
	return _T("");
}

bool RummyCube::saveGame(wxString filename)
{
	wxFileOutputStream output(filename);
	wxDataOutputStream saveGame(output);

	if (!saveGame.IsOk())
	{
		return false;
	}

	// First a little mark which shows which version of savegames we have (version 2).
	// We store the 7 (mark for rummycube savegame) as 16 bit value to avoid loading in version 1 of the game
	saveGame << (wxUint16)7 << 2;

	// Store the current game settings
	// Initial stones to take and sum in first round is not needed to save
	// They are saved for each player on its own
	saveGame << (wxInt32)timer->getTime() << (wxInt32)timer->timeLeft() << (wxUint32)opponentPlayers.size();

	// Store the players
	saveGame << humanPlayer;
	for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
	{
		saveGame << (*player);
	}

	// Store the stones without players
	std::vector<Gamestone*> stones = stoneManager.getStones();
	for (std::vector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
	{
		if ((*stone)->getPlayer() == NULL)
		{
			saveGame << (*stone);
		}
	}

	return true;
}
