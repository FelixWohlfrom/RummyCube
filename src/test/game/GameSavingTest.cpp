/*
 * GameSavingTest.cpp
 *
 *  Created on: 11.06.2016
 *      Author: Felix Wohlfrom
 */

#include "GameSavingTest.h"

#include "../../main/game/RummyCube.h"
#include "../../main/game/Settings.h"

#include <QFile>
#include <QSaveFile>

#define SAVE_GAME "saveGame.rcs"

void GameSavingTest::testGameLoadingAndSaving(QString loadGame)
{
    // First create a dummy window
    QWidget* dummyWindow = new QWidget();

    // Then create the game that is first loaded and then saved again
    RummyCube* game = new RummyCube();
    game->createStones(dummyWindow);

    QFile loadFile(loadGame);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        delete game;
        delete dummyWindow;
        Settings::cleanup();
        QFAIL(QString("Could not open file %1 for loading. %2")
                .arg(loadGame).arg(loadFile.errorString()).toUtf8());
    }
    else
    {
        QXmlStreamReader saveGameReader(&loadFile);
        saveGameReader >> game;

        // Loading failure, show error message and return
        if (saveGameReader.hasError())
        {
            delete game;
            delete dummyWindow;
            Settings::cleanup();
            QFAIL(QString("Error during file loading. %1")
                    .arg(saveGameReader.errorString()).toUtf8());
        }
    }

    QFileInfo filename(SAVE_GAME);

    QSaveFile saveFile(filename.absoluteFilePath());
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        delete game;
        delete dummyWindow;
        Settings::cleanup();
        QFAIL(QString("Could not open savegame file %1 to write. %2")
                .arg(SAVE_GAME).arg(saveFile.errorString()).toUtf8());
    }
    else
    {
        QXmlStreamWriter saveGameWriter(&saveFile);
        saveGameWriter << game;

        if (!saveFile.commit())
        {
            delete game;
            delete dummyWindow;
            Settings::cleanup();
            QFAIL(QString("Could not write savegame. %1").arg(saveFile.errorString()).toUtf8());
        }
    }

    compareFiles(loadGame, SAVE_GAME);

    delete game;
    delete dummyWindow;
    Settings::cleanup();
}

void GameSavingTest::compareFiles(QString orig, QString target)
{
    QFile origFile(orig);
    QFile targetFile(target);

    if (!origFile.open(QIODevice::ReadOnly))
    {
        QFAIL(QString("Could not open file %1 for loading. %2")
                .arg(orig).arg(origFile.errorString()).toUtf8());
    }

    if (!targetFile.open(QIODevice::ReadOnly))
    {
        QFAIL(QString("Could not open file %1 for loading. %2")
                .arg(target).arg(targetFile.errorString()).toUtf8());
    }

    QTextStream origStream(&origFile);
    QTextStream targetStream(&targetFile);
    QString missingLines;
    while (!origStream.atEnd())
    {
        if (targetStream.atEnd())
        {
            missingLines += origStream.readLine() + "\n";
        }
        else
        {
            QString origLine = origStream.readLine();
            QString targetLine = targetStream.readLine();

            QCOMPARE(targetLine, origLine);
        }
    }

    if (!missingLines.isEmpty())
    {
        QFAIL(QString("Files don't have same length, target file is shorter"
               " than original file. Missing lines in target file:\n%1")
               .arg(missingLines).toUtf8());
    }

    QString additionalLines;
    while (!targetStream.atEnd())
    {
        additionalLines += targetStream.readLine() + "\n";
    }
    if (!additionalLines.isEmpty())
    {
        QFAIL(QString("Files don't have same length, original file is shorter "
               "than target file. Additional lines in target file:\n%1")
               .arg(additionalLines).toUtf8());
    }

    origFile.close();
    targetFile.close();
}

void GameSavingTest::savegameBeforeFirstRound()
{
    testGameLoadingAndSaving(
            "../resources/savegames/Test_Before_First_Round.rcs");
}

void GameSavingTest::savegameNoConnectedStones()
{
    testGameLoadingAndSaving(
            "../resources/savegames/Test_First_Round_No_Connected_Stones.rcs");
}

void GameSavingTest::savegameConnectedStones()
{
    testGameLoadingAndSaving(
            "../resources/savegames/Test_First_Round_Stones_Connected.rcs");
}

QTEST_MAIN(GameSavingTest)
