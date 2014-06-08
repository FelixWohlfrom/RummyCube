/*
 * RummyCubeTests.cpp
 *
 *  Created on: 26.12.2013
 *      Author: Felix Wohlfrom
 */

#include "gtest/gtest.h"

#include "../utils/utils.cpp"
#include "../../../src/game/stones/StoneManager.h"

TEST(NetworkGame, VerifyNetworkGamePlayingSuccessfull)
{
	// initialize random seed with constant value for reproducibility
	// yes, 5 is really magic, it just appeared out of nowhere
	srand(5);

	// First, create the two windows for the network games
	DummyApp* testApp = utils::createAndShowTestApp();

	// Initialize the stone managers with the gamestones
	StoneManager hostStoneManager;
	wxPanel* testPanel = new wxPanel(testApp->getTestWindow());
	testPanel->SetSize(testPanel->GetParent()->GetSize());
	hostStoneManager.createStones(testPanel);

	StoneManager clientStoneManager;
	testPanel = new wxPanel(testApp->getTestWindow());
	clientStoneManager.createStones(testPanel);

	// Display the stones to make debugging easier
	std::vector<Gamestone*> stones = hostStoneManager.getStones();

	for (std::vector<Gamestone*>::iterator stone(stones.begin());
			stone != stones.end(); ++stone)
	{
		(*stone)->Show();
		wxYield();
	}

	// TODO Create new host game

}
