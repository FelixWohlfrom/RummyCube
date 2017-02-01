/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2016 Felix Wohlfrom                                    *
 *                                                                       *
 *  This program is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
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
 * Implemented as singleton.
 */
class Settings : public QSettings
{
    public:
        /**
         * Returns the file setting object.
         *
         * @return A pointer to the singleton
         */
        static Settings* getInstance();

        /**
         * Cleans up the settings object.
         * Just use on shutdown.
         */
        static void cleanup();

    private:
        /**
         * Constructor.
         *
         * @param filename The name of the settings file
         */
        Settings(QString filename);

        /**
         * The singleton instance.
         */
        static Settings* INSTANCE;
};

#endif /* SETTINGS_H_ */
