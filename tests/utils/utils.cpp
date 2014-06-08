/*
 * utils.cpp
 *
 *  Created on: 01.03.2014
 *      Author: Felix Wohlfrom
 *
 * In this file, you can find several static methods, which are used for common tasks
 * needed for testing the game.
 */

#include "DummyApp.h"

namespace utils
{

/**
 * Creates and displays a dummy window which can be used for further testing.
 * The main window can be controlled from within the test methods and for example
 * be closed or hidden.
 */
DummyApp* createAndShowTestApp()
{
	DummyApp* dummyApp = new DummyApp();
	wxApp::SetInstance(dummyApp);
	int argc = 0;
	char* argv[] = { NULL };
	wxEntryStart(argc, argv);
	dummyApp->CallOnInit();
	dummyApp->getTestWindow()->Show();

	return dummyApp;
}

} // namepspace utils
