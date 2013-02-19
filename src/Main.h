/*
 * Main.h
 *
 *  Created on: 22.02.2012
 *      Author: Felix Wohlfrom
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <wx/app.h>

#include "game/RummyCube.h"

/**
 * The main app class
 */
class Main: public wxApp
{
	private:
		/**
		 * This method is called on initialization of the application.
		 * Creates game object and main gui
		 */
		bool OnInit();

		/**
		 * This method is called on closing of the application.
		 * Cleans up internationalization
		 */
		int OnExit();
};

DECLARE_APP(Main)


#endif /* MAIN_H_ */
