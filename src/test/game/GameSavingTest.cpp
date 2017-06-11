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
 * GameSavingTest.cpp
 *
 *  Created on: 11.06.2016
 *      Author: Felix Wohlfrom
 */

#include "GameSavingTest.h"

#include <QFile>
#include <QSaveFile>

#include "../../main/game/Settings.h"

#define SAVE_GAME "saveGame.rcs"
#define SAVE_GAME_PATH QString("../resources/savegames/")

void GameSavingTest::init()
{
    // First create a dummy window
    dummyWindow = new QWidget();

    // Then create the game that is used for testing
    game = new RummyCube();
    game->getStoneManager().createStones(dummyWindow);

    // Move the stones to an initial position
    QVector<Gamestone*> stones = game->getStoneManager().getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
           stone != stones.end();
           ++stone)
    {
        (*stone)->move((*stone)->asInt(), (*stone)->asInt());

        // Spread the stones over the different stone parents
        switch ((*stone)->getColor())
        {
            case Gamestone::StoneColor::BLUE:
                (*stone)->setParent(Gamestone::StoneParent::HOLDER);
                break;
            case Gamestone::StoneColor::YELLOW:
                (*stone)->setParent(Gamestone::StoneParent::AIHOLDER);
                break;
            case Gamestone::StoneColor::RED:
                (*stone)->setParent(Gamestone::StoneParent::BOARD);
                break;
            default:
                (*stone)->setParent(Gamestone::StoneParent::HEAP);
        }
    }

    // Store the stones to make sure "old*" values are set
    for (QVector<Gamestone*>::iterator stone(stones.begin());
           stone != stones.end();
           ++stone)
    {
        (*stone)->storeStone();
    }
}

void GameSavingTest::cleanup()
{
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

void GameSavingTest::verifyPresavedGame_data()
{
    QTest::addColumn<QString>("loadGame");

    /**
     * This testcase verifies that a game that was saved before any player has played out
     * can be loaded and saved again successfully.
     */
    QTest::newRow("savegameBeforeFirstRound")
        << SAVE_GAME_PATH + "Test_Before_First_Round.rcs";

    /*
     * This testcase verifies a game that was saved after first round and the player didn't
     * connect any stones yet can be saved again successfully.
     */
    QTest::newRow("savegameNoConnectedStones")
        << SAVE_GAME_PATH + "Test_First_Round_No_Connected_Stones.rcs";

    /**
     * This testcase verifies a game that was saved after first round and the player connected
     * some stones can be saved again successfully.
     */
    QTest::newRow("savegameConnectedStones")
        << SAVE_GAME_PATH + "Test_First_Round_Stones_Connected.rcs";
}

void GameSavingTest::verifyPresavedGame()
{
    QFETCH(QString, loadGame);

    QFile loadFile(loadGame);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
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
            QFAIL(QString("Error during file loading. %1")
                    .arg(saveGameReader.errorString()).toUtf8());
        }
    }

    QFileInfo filename(SAVE_GAME);

    QSaveFile saveFile(filename.absoluteFilePath());
    if (!saveFile.open(QIODevice::WriteOnly))
    {
        QFAIL(QString("Could not open savegame file %1 to write. %2")
                .arg(SAVE_GAME).arg(saveFile.errorString()).toUtf8());
    }
    else
    {
        QXmlStreamWriter saveGameWriter(&saveFile);
        saveGameWriter << game;

        if (!saveFile.commit())
        {
            QFAIL(QString("Could not write savegame. %1").arg(saveFile.errorString()).toUtf8());
        }
    }

    compareFiles(loadGame, SAVE_GAME);
}

void GameSavingTest::verifyStoneSaveAndLoading()
{
    // TODO Combine several stones to create rows
    // TODO create variants with joker and without joker

    // First the stone information for later validation
    QVector<GamestoneData> gamestoneData;
    QVector<Gamestone*> stones = game->getStoneManager().getStones();
    for (QVector<Gamestone*>::iterator stone(stones.begin());
           stone != stones.end();
           ++stone)
    {
        GamestoneData data = {
            (*stone)->pos().x(),
            (*stone)->pos().y(),
            (*stone)->getParent(),
            (*stone)->getPrev(),
            (*stone)->getNext()
        };
        gamestoneData.push_back(data);
    }

    // Store game in string
    QString xmlString;
    QXmlStreamWriter writer(&xmlString);
    writer << game;

    // Move all stones to (0,0), change parent and store them again
    for (QVector<Gamestone*>::iterator stone(stones.begin());
           stone != stones.end();
           ++stone)
    {
        (*stone)->move(0, 0);

        if ((*stone)->getColor() == Gamestone::StoneColor::BLACK)
        {
            (*stone)->setParent(Gamestone::StoneParent::AIHOLDER);
        }

        // TODO Reset prev and next stones

        (*stone)->storeStone();
    }


    // Load game and verify that all stones are at correct position
    QXmlStreamReader reader(xmlString);
    reader >> game;

    for (int i = 0; i < stones.size(); i++)
    {
        Gamestone* stone = stones.at(i);
        GamestoneData data = gamestoneData.at(i);

        QCOMPARE(stone->pos().x(), data.posX);
        QCOMPARE(stone->pos().y(), data.posY);
        QCOMPARE(stone->getParent(), data.parent);
        QCOMPARE(stone->getPrev(), data.prev);
        QCOMPARE(stone->getNext(), data.next);
    }

    // Restore all stones and verify they have still the same data
    for (QVector<Gamestone*>::iterator stone(stones.begin());
           stone != stones.end();
           ++stone)
    {
        (*stone)->restoreStone();
    }

    for (int i = 0; i < stones.size(); i++)
    {
        Gamestone* stone = stones.at(i);
        GamestoneData data = gamestoneData.at(i);

        QCOMPARE(stone->pos().x(), data.posX);
        QCOMPARE(stone->pos().y(), data.posY);
        QCOMPARE(stone->getParent(), data.parent);
        QCOMPARE(stone->getPrev(), data.prev);
        QCOMPARE(stone->getNext(), data.next);
    }
}

QTEST_MAIN(GameSavingTest)
