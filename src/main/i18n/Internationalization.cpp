/*
 * Internationalization.cpp
 *
 *  Created on: 15.10.2016
 *      Author: Felix Wohlfrom
 */


#include "Internationalization.h"

#include "../game/Settings.h"

#include <QDir>

Internationalization::Internationalization(QCoreApplication* app) :
    app(app), languagePath(), installedLangs()
{
    // Load saved language file
    Language* currLanguage = NULL;
    Settings* settings = Settings::getInstance();
    QString savedLanguage = settings->value("General/Language", "").toString();

    // Initialize path to locales
    languagePath = app->applicationDirPath().append("/i18n");

    // Initialize currently installed languages
    // First add English, which is not in a mo file
    Language newLang = {"English", QLocale("en")};
    installedLangs.insert(newLang);
    defaultLang = newLang; // If English is not really the default language,
                           // it will be overwritten in the loop

    // Now add the languages found in qm files
    QDir dir(languagePath);
    QStringList fileNames = dir.entryList(QStringList("RummyCube*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        // get locale extracted by filename
        QString localeString;
        localeString = fileNames[i]; // "RummyCube_de.qm"
        localeString.truncate(localeString.lastIndexOf('.')); // "RummyCube_de"
        localeString.remove(0, localeString.indexOf('_') + 1); // "de"

        QLocale locale(localeString);
        QString lang = QLocale::languageToString(locale.language());
        Language newLang = { lang, locale };
        installedLangs.insert(newLang);

        // Set selected language to loaded language
        if (newLang.locale.name() == savedLanguage)
        {
            currLanguage = &newLang;
        }

        // Update default language
        if (QLocale::system().language() == locale.language())
        {
            defaultLang = newLang;
        }
    }

    setLanguage(currLanguage);
}

std::set<Language>& Internationalization::getInstalledLanguages()
{
    return installedLangs;
}

void Internationalization::setLanguage(Language* language)
{
    // Remove previous loaded translations
    app->removeTranslator(&translator);
    app->removeTranslator(&translatorQt);

    QString languageToSave = "";

    if (language == NULL)
    {
        language = &defaultLang;
    }
    else
    {
        languageToSave = language->locale.name();
    }

    // Try to load given language
    QString lang = language->locale.name();
    if (translator.load(languagePath + "/RummyCube_" + lang))
    {
        translatorQt.load(languagePath + "/qt_" + lang);
        app->installTranslator(&translator);
        app->installTranslator(&translatorQt);
    }
    else
    {
        // Load default language
        languageToSave = "";
    }

    Settings* settings = Settings::getInstance();
    settings->setValue("General/Language", languageToSave);
}
