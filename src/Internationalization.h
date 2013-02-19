/*
 * Internationalization.h
 *
 *  Created on: 05.11.2011
 *      Author: felix
 */

#ifndef INTERNATIONALIZATION_H_
#define INTERNATIONALIZATION_H_

#include <set>

#include <wx/wx.h>
#include <wx/intl.h>

/**
 * This struct is used to keep the name of a language and it's identifier together
 */
struct Language
{
	wxString name;
	wxString file;
	long identifier;

	bool operator<(const Language& other) const { return name < other.name; };
};

/**
 * This class is used to handle all stuff that is related to internationalization.
 * Implemented as singleton.
 */
class Internationalization
{
	public:
		/**
		 * Returns the internationalization object
		 *
		 * @return A pointer to the internationalization singleton
		 */
		static Internationalization* getInstance();

		/**
		 * Cleans up the settings object
		 * Just use on shutdown
		 */
		static void cleanup();

		/**
		 * Returns the language id of the currently selected language
		 *
		 * @return The id of the language
		 */
		long getLanguage();

		 /**
		  * Returns the default language.
		  * Either system language or english, if system language is not in installed languages
		  *
		  * @return The default language
		  */
		Language getDefaultLanguage();

		/**
		 * Returns all languages available in i18n folder + english as language used in code.
		 * "Default" entry is not included!.
		 *
		 * Languages ordered by name
		 *
		 * @return A reference to the language list.
		 */
		std::set<Language>& getInstalledLanguages();

		/**
		 * Sets the language to the one defined by langId
		 *
		 * @param langId The language id to set
		 */
		void setLanguage(long langId);

	private:
		/**
		 * Constructor
		 */
		Internationalization();

		/**
		 * The singleton instance
		 */
		static Internationalization* INSTANCE;

		/**
		 * The locale that is currently used by the program
		 */
		wxLocale* locale;

		/**
		 * If the locale is used via wxDEFAULT_LANGUAGE or not
		 */
		bool isDefaultLang;
		
		/**
		 * The path to the directory containing the locales
		 * Set to %program path%/i18n in constructor
		 */
		wxString localePath;

		/**
		 * The default language (system language or english)
		 */
		Language defaultLang;

		/**
		 * A list with the currently installed languages, sorted by language name
		 */
		std::set<Language> installedLangs;
};

#endif /* INTERNATIONALIZATION_H_ */
