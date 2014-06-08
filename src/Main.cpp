/*
 * Main.cpp
 *
 *  Created on: 22.02.2012
 *      Author: Felix Wohlfrom
 */

#include "Main.h"

#include "Internationalization.h"

#include "game/Settings.h"
#include "gui/MainWindow.h"

#ifdef _DEBUG
	#ifdef _WIN32
		#include <iostream>

		#ifdef _MSC_VER
			// Uncomment this if you installed visual memory
			// leak detector from http://vld.codeplex.com/
			#include <vld.h>
		#endif
	#endif
#endif

IMPLEMENT_APP(Main)

bool Main::OnInit()
{
	// initialize random seed
	srand(time(NULL));

	// Enable debug console on win32
	#ifdef _DEBUG
		#ifdef _WIN32
			int hConHandle;
			long long lStdHandle;
			FILE *fp;
			// allocate a console for this app
			AllocConsole();

			// redirect unbuffered STDOUT to the console
			lStdHandle = (long long)GetStdHandle(STD_OUTPUT_HANDLE);
			hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
			fp = _fdopen(hConHandle, "w");
			*stdout = *fp;
			setvbuf(stdout, NULL, _IONBF, 0);
		#endif
	#endif

	// Start the main window and show it
	MainWindow* mainWin = new MainWindow();
	return mainWin->Show();
}

int Main::OnExit()
{
	Settings::cleanup();
	Internationalization::cleanup();

	return wxApp::OnExit();
}
