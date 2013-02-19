/*
 * Settings.cpp
 *
 *  Created on: 23.08.2011
 *      Author: Felix Wohlfrom
 */

#include "Settings.h"

#include <wx/filefn.h>
#include <wx/stdpaths.h>

// Initialize static variable
Settings* Settings::INSTANCE = NULL;

Settings::Settings(wxString filename) :
	wxFileConfig(_T("Rummy"), wxEmptyString, filename, wxEmptyString, wxCONFIG_USE_LOCAL_FILE) 
{}

Settings* Settings::getInstance()
{
	if (INSTANCE == NULL)
	{
		// Under windows, settings are stored in %appdata%/RummyCube/settings.conf
		// Under linux, its ~/.RummyCube/settings.conf
		wxString settingsFile(wxStandardPaths::Get().GetUserDataDir() + _T("/settings.conf"));

		if (!wxDirExists(wxPathOnly(settingsFile)))
		{
			wxMkdir(wxPathOnly(settingsFile));
		}

		INSTANCE = new Settings(settingsFile);
	}

	return INSTANCE;
}

void Settings::cleanup()
{
	wxDELETE(INSTANCE);
}
