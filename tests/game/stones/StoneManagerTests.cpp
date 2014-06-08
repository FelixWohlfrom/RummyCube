/*
 * StoneManagerTests.cpp
 *
 *  Created on: 26.12.2013
 *      Author: Felix Wohlfrom
 */

#include "gtest/gtest.h"

#include "../../utils/utils.cpp"
#include "../../../src/game/stones/StoneManager.h"

TEST(StoneManager, checkIfStoneFromIntIsUnique)
{
	DummyApp* testApp = utils::createAndShowTestApp();

	StoneManager testManager;
	wxPanel* testPanel = new wxPanel(testApp->getTestWindow());

	testManager.createStones(testPanel);

	std::vector<Gamestone*> stones = testManager.getStones();

	for (std::vector<Gamestone*>::iterator firstStone(stones.begin()); firstStone != stones.end(); ++firstStone)
	{
		for (std::vector<Gamestone*>::iterator secondStone(stones.begin()); secondStone != stones.end(); ++secondStone)
		{
			if ((*firstStone) != (*secondStone))
			{
				ASSERT_NE((*firstStone)->asInt(), (*secondStone)->asInt());
			}
		}
	}

	// TODO Leads to segfault. Is this really needed?
	// wxEntryCleanup();
}
