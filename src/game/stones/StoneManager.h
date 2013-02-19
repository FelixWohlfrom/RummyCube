/*
 * StoneManager.h
 *
 *  Created on: 25.07.2011
 *      Author: Felix Wohlfrom
 */

#ifndef STONEMANAGER_H_
#define STONEMANAGER_H_

#include <vector>

#include <wx/panel.h>

#include "Gamestone.h"

/**
 * The stone manager handles all gamestone specific stuff.
 * It's used for example for creation/destroying, but also for drag'n'drop
 */
class StoneManager
{
	public:
		/*
		 * C'tor. Inits variables
		 */
		StoneManager();

		/**
		 * D'tor. Cleans up the stones
		 */
		~StoneManager();

		/**
		 * Indicates if all stones in a row should be moved at once
		 */
		bool moveAllStonesInRow;

		/**
		 * Creates a bunch of stones on given heap
		 *
		 * @param heap The heap on which the stones should be created
		 */
		void createStones(wxPanel* heap);

		/**
		 * Returns the stones
		 *
		 * @return A reference to the stone vector
		 */
		std::vector<Gamestone*>& getStones();

		/**
		 * Returns the stone count on heap
		 *
		 * @return The number of stones which are still on the heap
		 */
		int getStoneCountOnHeap() const;

		/**
		 * Returns the stone which the current stone is dragging over.
		 * Relevant position is the cursor position!
		 *
		 * @param current The stone that is being dragged
		 * @return A reference to the stone that is under the dragged stone. NULL if no stone is at this position
		 */
		Gamestone* getStoneDraggingOver(Gamestone& current) const;

		/**
		 * Returns the gamestone represented by the int value.
		 * The value needs to be built as descriped in Gamestone.asInt() method
		 *
		 * @param value The integer value of the stone which should be returned
		 * @return A pointer to the stone which is descriped in value
		 */
		Gamestone* getStoneFromInt(int value) const;

		/**
		 * Stores the stone that is currently dragged on the gameboard.
		 * If no stone is dragged, this is NULL.
		 * Needed for handling of overlapping elements in wxwidgets
		 */
		Gamestone* draggedStone;

	private:
		/**
		 * The gamestones managed by current stone manager
		 */
		std::vector<Gamestone*> gamestones;
};

#endif /* STONEMANAGER_H_ */
