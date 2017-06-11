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
 * Settings.cpp
 *
 *  Created on: 23.08.2011
 *      Author: Felix Wohlfrom
 */

#include "Settings.h"

#include <QStandardPaths>

// Initialize static variable
Settings* Settings::INSTANCE = NULL;

Settings::Settings(QString filename) :
    QSettings(filename, QSettings::IniFormat)
{}

Settings* Settings::getInstance()
{
    if (INSTANCE == NULL)
    {
        QString settingsFile(
                QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
                + "/settings.conf");

        INSTANCE = new Settings(settingsFile);
    }

    return INSTANCE;
}

void Settings::cleanup()
{
    delete INSTANCE;
    INSTANCE = NULL;
}
