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
        // Under windows, settings are stored in %appdata%/RummyCube/settings.conf
        // Under linux, its ~/.RummyCube/settings.conf
        QString settingsFile(QStandardPaths::displayName(QStandardPaths::AppDataLocation) + "/settings.conf");

        // TODO Reimplement
        /*if (!wxDirExists(wxPathOnly(settingsFile)))
        {
            wxMkdir(wxPathOnly(settingsFile));
        }*/

        INSTANCE = new Settings(settingsFile);
    }

    return INSTANCE;
}

void Settings::cleanup()
{
    delete INSTANCE;
    INSTANCE = NULL;
}
