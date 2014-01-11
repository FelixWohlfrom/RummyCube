/*
 * Internationalization.cpp
 *
 *  Created on: 05.11.2011
 *      Author: felix
 */


#include "Internationalization.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

#include "game/Settings.h"

// Initialize static variable
Internationalization* Internationalization::INSTANCE = NULL;

Internationalization::Internationalization() :
	locale(NULL), isDefaultLang(false), localePath(), defaultLang(), installedLangs()
{
	// Initialize path to locales
	#if wxCHECK_VERSION(3,0,0)
	    wxStandardPaths stdPaths = wxStandardPaths::Get();
	    localePath = wxPathOnly(stdPaths.GetExecutablePath()) + _T("/i18n");
	#else
	    localePath = wxPathOnly(wxStandardPaths::Get().GetExecutablePath()) + _T("/i18n");
	#endif

	// Get id of default language
	int defaultLangId = wxLocale::GetSystemLanguage();

	// Initialize currently installed languages
	// First add english, which is not in a mo file
	Language newLang = {_T("English"), _T(""), wxLANGUAGE_ENGLISH};
	installedLangs.insert(newLang);
	defaultLang = newLang; // If english is not really the default language, it will be overwritten in the loop

	// Now add the languages found in mo files
	wxDir dir(localePath);
	wxString filename;
	const wxLanguageInfo* langinfo;

	if (wxDir::Exists(dir.GetName()))
	{
		for (bool continueSearch(dir.GetFirst(&filename, _T("*.mo"), wxDIR_FILES)); continueSearch; continueSearch = dir.GetNext(&filename))
		{
			langinfo = wxLocale::FindLanguageInfo(wxFileName(filename).GetName());
			if (langinfo != NULL)
			{
				Language newLang = {langinfo->Description, wxFileName(filename).GetName(), langinfo->Language };
				installedLangs.insert(newLang);

				if (langinfo->Language == defaultLangId)
				{
					defaultLang = newLang;
				}
			}
		}
	}

	// Load saved language file
	Settings* settings = Settings::getInstance();
	long langId;
	settings->Read(_T("General/LanguageId"), &langId, wxLANGUAGE_DEFAULT);
	setLanguage(langId);
}

Internationalization* Internationalization::getInstance()
{
	if (INSTANCE == NULL)
	{
		INSTANCE = new Internationalization();
	}

	return INSTANCE;
}

void Internationalization::cleanup()
{
	if (INSTANCE != NULL)
	{
		wxDELETE(INSTANCE->locale);
	}
	wxDELETE(INSTANCE);
}

long Internationalization::getLanguage()
{
	if (locale != NULL)
	{
		if (isDefaultLang)
		{
			return wxLANGUAGE_DEFAULT;
		}
		else
		{
			return locale->GetLanguage();
		}
	}

	return -1;
}

std::set<Language>& Internationalization::getInstalledLanguages()
{
	return installedLangs;
}

Language Internationalization::getDefaultLanguage()
{
	return defaultLang;
}

void Internationalization::setLanguage(long langId)
{
	// Reset locale
	wxDELETE(locale);
	locale = new wxLocale;

	Settings* settings = Settings::getInstance();

	// Try to load language with given id
	for (std::set<Language>::iterator lang(installedLangs.begin()); lang != installedLangs.end(); ++lang)
	{
		if ((*lang).identifier == langId)
		{
			isDefaultLang = false;

			locale->Init((*lang).identifier);
			locale->AddCatalogLookupPathPrefix(localePath);
			locale->AddCatalog((*lang).file);

			settings->Write(_T("General/LanguageId"), (*lang).identifier);
			return;
		}
	}

	// If no language found, fall back to default language
	isDefaultLang = true;
	locale->Init(defaultLang.identifier);
	locale->AddCatalogLookupPathPrefix(localePath);
	locale->AddCatalog(defaultLang.file);

	settings->Write(_T("General/LanguageId"), (long)wxLANGUAGE_DEFAULT);
}
