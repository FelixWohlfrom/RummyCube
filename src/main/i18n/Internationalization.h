/*
 * Internationalization.h
 *
 *  Created on: 05.11.2011
 *      Author: felix
 */

#ifndef INTERNATIONALIZATION_H_
#define INTERNATIONALIZATION_H_

#include <set>

#include <QTranslator>
#include <QApplication>

/**
 * This struct is used to keep the name of a language and it's identifier together.
 */
struct Language
{
    /**
     * The name of the language.
     */
    QString name;

    /**
     * The the locale for the current language.
     */
    QLocale locale;

    /**
     * Returns if the current language name is alphabetical ordered before the
     * given language.
     *
     * @param other The other language that should be compared
     * @return If the current language is alphabetically ordered before the
     *         given language
     */
    bool operator<(const Language& other) const { return name < other.name; };
};

Q_DECLARE_METATYPE(Language)

/**
 * This class is used to handle all stuff that is related to internationalization.
 */
class Internationalization
{
    public:
        /**
         * Constructor.
         *
         * @param app The app that should be localized.
         */
        Internationalization(QCoreApplication* app);

        /**
         * Returns all languages available in i18n folder + English as language used in code.
         * "Default" entry is not included!
         *
         * Languages ordered by name.
         *
         * @return A reference to the language list.
         */
        std::set<Language>& getInstalledLanguages();

        /**
         * Sets the active language to the given language.
         *
         * @param language The language to select.
         */
        void setLanguage(Language* language);

    private:
        /**
         * The app that should be localized.
         */
        QCoreApplication* app;
        
        /**
         * The path to the directory containing the languages.
         * Set to %program path%/i18n in constructor.
         */
        QString languagePath;

        /**
         * The default language (system language or English).
         */
        Language defaultLang;

        /**
         * A list with the currently installed languages, sorted by language name.
         */
        std::set<Language> installedLangs;

        /**
         * The translator object for the application translations.
         */
        QTranslator translator;

        /**
         * The translator object for qt translations.
         */
        QTranslator translatorQt;
};

#endif /* INTERNATIONALIZATION_H_ */
