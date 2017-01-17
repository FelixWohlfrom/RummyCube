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
