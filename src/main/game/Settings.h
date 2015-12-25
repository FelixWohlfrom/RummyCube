/*
 * Settings.h
 *
 *  Created on: 23.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QSettings>

/**
 * This class handles all the settings.
 * Implemented as singleton
 */
class Settings : public QSettings
{
	public:
		/**
		 * Returns the file setting object
		 *
		 * @return A pointer to the singleton
		 */
		static Settings* getInstance();

		/**
		 * Cleans up the settings object
		 * Just use on shutdown
		 */
		static void cleanup();

	private:
		/**
		 * Constructor
		 *
		 * @param filename The name of the settings file
		 */
		Settings(QString filename);

		/**
		 * The singleton instance
		 */
		static Settings* INSTANCE;
};

#endif /* SETTINGS_H_ */
