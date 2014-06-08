/*
 * DummyApp.cpp
 *
 *  Created on: 27.12.2013
 *      Author: Felix Wohlfrom
 */

#include "DummyApp.h"

#include <wx/frame.h>
#include "../../../src/game/Settings.h"

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

bool DummyApp::OnInit()
{
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

	testWindow = new wxFrame(NULL, -1, wxString(_("")), wxDefaultPosition, wxDefaultSize);
	return true;
}

int DummyApp::OnExit()
{
	Settings::cleanup();
	delete testWindow;

	return wxApp::OnExit();
}

wxWindow* DummyApp::getTestWindow()
{
	return testWindow;
}
