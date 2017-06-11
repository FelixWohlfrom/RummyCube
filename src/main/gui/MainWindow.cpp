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
 * MainWindow.cpp
 *
 *  Created on: 21.07.2011
 *      Author: Felix Wohlfrom
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Tutorial.h"
#include "SettingsWindow.h"
#include "CreateNetworkGameWindow.h"
#include "JoinNetworkGameWindow.h"
#include "AboutWindow.h"

#include "../game/Settings.h"
#include "../game/player/NetworkPlayer.h"

#include <iostream>

#include <QFile>
#include <QDebug>
#include <QSaveFile>
#include <QMimeData>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>

// Margin around existing stones at playout time for ai player in pixel
#define MARGIN 15

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), forceClose(false),
    gameState(GAME_IDLE), nextGame(NULL)
{
    setWindowIcon(QIcon(":images/icon"));

    // Fire up the game
    game = new RummyCube();

    // Initialize settings, i18n and timer
    Settings* settings = Settings::getInstance();
    i18n = new Internationalization(QApplication::instance());
    connect(game->getTimer(), SIGNAL(timeout()),
                        this, SLOT(timeLimitEvent()));

    // Initialize window
    ui->setupUi(this);

    // Generate language menu
    QString savedLanguage = settings->value("General/Language", "").toString();
    QActionGroup* langGroup = new QActionGroup(ui->menu_Language);
    langGroup->setExclusive(true);
    langGroup->addAction(ui->defaultLanguageAction);
    connect(langGroup, SIGNAL(triggered(QAction*)),
                 this, SLOT(languageChangedEvent(QAction*)));
    if (savedLanguage == "")
    {
        ui->defaultLanguageAction->setChecked(true);;
    }
    for (Language lang : i18n->getInstalledLanguages())
    {
        QAction *action = new QAction(lang.name, this);
        action->setCheckable(true);
        action->setData(QVariant::fromValue<Language>(lang));

        ui->menu_Language->addAction(action);
        langGroup->addAction(action);

        if (savedLanguage == lang.locale.name())
        {
            action->setChecked(true);
        }
    }

    // Init main window position and size
    // Can not be done earlier, because SetSize() fires an OnResize event
    // and event handler will run into a null pointer
    QRect screenSize = QApplication::desktop()->screenGeometry();
    int screenWidth = screenSize.width();
    int screenHeight = screenSize.height();
    int height = settings->value("Position/Height",
            screenHeight - (screenHeight / 5)).toInt();
    int width = settings->value("Position/Width",
            screenWidth - (screenWidth / 3)).toInt();
    int top = settings->value("Position/Top", 100).toInt();
    int left = settings->value("Position/Left", 30).toInt();
    this->resize(width, height);
    this->move(left, top);

    ImagePanel* gameboardPanel = new ImagePanel(
                settings->value("General/GameboardPath").toString(),
                QString(":images/gameboard"),
                this);
    gameboardPanel->setSizePolicy(ui->gameboard->sizePolicy());
    // This is needed to make the game board fill all space of the parent
    // scroll area
    gameboardPanel->setMinimumSize(1, 1);
    ui->gameboardScroller->setWidget(gameboardPanel);
    ui->gameboard = gameboardPanel;

    ImagePanel* holderPanel = new ImagePanel(
                settings->value("General/HolderPath").toString(),
                QString(":images/holder"),
                this);
    holderPanel->setSizePolicy(ui->holder->sizePolicy());
    // This is needed to make the holder fill all space of the parent
    // scroll area
    holderPanel->setMinimumSize(1, 1);
    ui->holderScroller->setWidget(holderPanel);
    ui->holder = holderPanel;

    ui->gameboard->installEventFilter(this);
    ui->gameboard->setAcceptDrops(true);
    ui->holder->installEventFilter(this);
    ui->holder->setAcceptDrops(true);

    connect(ui->infoLabel, SIGNAL(clicked()),
                     this, SLOT(takeAllStones()));

    connect(ui->passButton, SIGNAL(clicked()),
                      this, SLOT(startNextRound()));

    this->moveAllStonesEvent(game->getStoneManager().moveAllStonesInRow);
}

MainWindow::~MainWindow()
{
    // Close game
    delete game;

    // Store position
    Settings* settings = Settings::getInstance();
    settings->setValue("Position/Height", this->size().height());
    settings->setValue("Position/Width", this->size().width());
    settings->setValue("Position/Top", this->pos().y());
    settings->setValue("Position/Left", this->pos().x());

    delete i18n;
    delete ui;
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

    // Just init the stones if not already initialized
    if (game->getStoneManager().getStones().size() == 0)
    {
        this->initStones();
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);

    // Make the label text fit the label
    QFontMetrics metrics(ui->infoLabel->font());
    QString elidedText = metrics.elidedText(ui->infoLabel->toolTip(),
            Qt::ElideRight, this->width() / 2);
    ui->infoLabel->setText(elidedText);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu contextMenu(this);
    QAction* moveAllStones = new QAction(tr("Move &All Stones"), this);
    moveAllStones->setShortcut(Qt::Key_A);
    moveAllStones->setStatusTip(tr("Move all stones in the moved row"));
    moveAllStones->setCheckable(true);
    moveAllStones->setChecked(ui->moveAllStonesAction->isChecked());
    connect(moveAllStones, SIGNAL(toggled(bool)),
                     this, SLOT(moveAllStonesEvent(bool)));
    contextMenu.addAction(moveAllStones);

    QAction* testStones = new QAction(tr("&Test Stones On Gameboard"), this);
    testStones->setShortcut(Qt::Key_T);
    testStones->setStatusTip(tr("Tests stones on the gameboard and marks "
                                "the stones gray that violate any rules"));
    connect(testStones, SIGNAL(triggered()),
                  this, SLOT(testStonesOnGameboardEvent()));
    contextMenu.addAction(testStones);

    QAction* pass = new QAction(tr("&Pass (Next Round)"), this);
    pass->setShortcut(Qt::Key_N);
    pass->setStatusTip(tr("Finish the current round and continue"
            " with next round"));
    pass->setEnabled(ui->passAction->isEnabled());
    connect(pass, SIGNAL(triggered()),
            this, SLOT(startNextRound()));
    contextMenu.addAction(pass);

    contextMenu.exec(event->globalPos());
}

void MainWindow::showError(QString message)
{
    QMessageBox saveError(this);
    saveError.setWindowTitle(tr("Error"));
    saveError.setText(message);
    saveError.setStandardButtons(QMessageBox::Ok);
    saveError.setIcon(QMessageBox::Critical);
    saveError.exec();
}

void MainWindow::restartGame()
{
    // Reset menu entries, cursor and settings.
    // If we are joining a network game, settings are done
    // in startNextRound method.
    if (this->gameState != GAME_JOIN_NETWORK_GAME)
    {
        this->setCursor(Qt::ArrowCursor);
        ui->passAction->setEnabled(true);
        ui->passButton->setEnabled(true);
        ui->loadGameAction->setEnabled(true);
        ui->saveGameAction->setEnabled(true);
        Gamestone::canMoveStones = true;
    }

    // Reset game
    delete this->game;
    if (nextGame != NULL)
    {
        game = nextGame;
        nextGame = NULL;
    }
    else
    {
        game = new RummyCube();
    }

    this->initStones();

    if (this->gameState == GAME_JOIN_NETWORK_GAME)
    {
        this->startNextRound();
    }

    this->gameState = GAME_IDLE;

    // Reset size of gameboard and holder
    ui->gameboard->setMinimumSize(1, 1);
    ui->holder->setMinimumSize(1, 1);
}

void MainWindow::initStones()
{
    // Create the stones
    game->getStoneManager().createStones(ui->heap);
    QVector<Gamestone*>& stones = game->getStoneManager().getStones();

    int y(5);
    for (int i(0); i < stones.size(); ++i)
    {
        int x((rand() % (ui->heap->size().width() - 60)) + 20);
        stones.at(i)->move(x, y);
        stones.at(i)->storeStone();

        // This makes the stones appear one after another
        stones.at(i)->show();
        qApp->processEvents();

        // Connect the parent change event with the main window
        connect(stones.at(i), SIGNAL(parentChanged(ParentChangedEvent&)),
                this, SLOT(gamestoneParentChanged(ParentChangedEvent&)));
    }

    // Init information panel
    this->setInfoLabelText();

    // Update pass buttons
    ui->passAction->setEnabled(false);
    ui->passButton->setEnabled(false);
}

void MainWindow::setInfoLabelText()
{
    // If more then one stone can be taken
    if (game->getHumanPlayer()->getStonesLeftToTake() > 1
            && game->getStoneManager().getStoneCountOnHeap() > 0)
    {
        ui->infoLabel->setText(
                tr("Please get a stone (%1 more) or click here to get all"
                        " at once")
                        .arg(game->getHumanPlayer()->getStonesLeftToTake()));
    }
    // If only one stone can be taken
    else if (game->getHumanPlayer()->getStonesLeftToTake() == 1
            && game->getStoneManager().getStoneCountOnHeap() > 0)
    {
        ui->infoLabel->setText(tr("Please get a stone"));
    }
    // If no more stones can be taken
    else
    {
        ui->infoLabel->setText(tr("It's your turn"));
    }
    ui->infoLabel->setToolTip(ui->infoLabel->text());
}

void MainWindow::setTimerLabelText()
{
    if (game != NULL
            && game->getTimer() != NULL
            && game->getTimer()->getTime() > -1)
    {
        // Mark background red if < 10 seconds left
        if (game->getTimer()->timeLeft() <= 10
                // and even time value
                && !(game->getTimer()->timeLeft() & 0x01))
        {
            ui->timerLabel->setStyleSheet("QLabel { color : red; }");
        }
        else
        {
            ui->timerLabel->setStyleSheet("QLabel { color : black; }");
        }

        int min = game->getTimer()->timeLeft() / 60;
        int sec = game->getTimer()->timeLeft() % 60;
        ui->timerLabel->setText(
                tr("%1:%2 minutes").arg(min).arg(sec, 2, 10, QLatin1Char('0')));
        ui->timerLabel->setToolTip(
                tr("%1:%2 minutes").arg(min).arg(sec, 2, 10, QLatin1Char('0')));
    }
    else
    {
        ui->timerLabel->setText("");
        ui->timerLabel->setToolTip("");
    }
}

void MainWindow::takeAllStones()
{
    // If the stones can not be moved, don't take the stones
    if (!Gamestone::canMoveStones) return;

    QVector<Gamestone*> stones = game->getStoneManager().getStones();
    while (game->getHumanPlayer()->getStonesLeftToTake() > 0
            && game->getStoneManager().getStoneCountOnHeap() > 0)
    {
        int index = rand() % stones.size();
        while (stones.at(index)->getParent() != Gamestone::HEAP)
        {
            index = rand() % stones.size();
        }
        stones.at(index)->setParent(Gamestone::HOLDER);
    }
}

void MainWindow::gamestoneParentChanged(ParentChangedEvent& event)
{
    Gamestone& sender(*(event.getSender()));

    switch (event.getParent())
    {
        case Gamestone::AIHOLDER:
            // Move stone to invisible area on heap
            // No break needed, needs to be reparented too on game loading
            sender.move(QPoint(0, 100));
            /* no break */
        case Gamestone::HEAP:
            sender.setParent(ui->heap);
            break;
        case Gamestone::BOARD:
            sender.setParent(ui->gameboard);
            if (!event.getStoneMoved() // Stones are moved if is network game
                // Otherwise ai plays, stones are automatically moved to board
                && sender.getParent() == Gamestone::AIHOLDER
                && sender.isFirstInRow())
            {
                this->moveToBoard(sender);
            }
            break;
        case Gamestone::HOLDER:
            // Just move to holder if there are still stones left to take
            // or if the stone is drag'n'dropped
            if (game->getHumanPlayer()->getStonesLeftToTake() > 0
                    || event.getStoneMoved())
            {
                sender.setParent(ui->holder);
                sender.show();

                if (!event.getStoneMoved())
                {
                    // Start game timer if clicked on stone
                    game->getTimer()->start();

                    game->getHumanPlayer()->addStone(sender);
                    this->moveToHolder(sender);
                    this->setInfoLabelText();
                    if (game->getHumanPlayer()->getStonesLeftToTake() == 0)
                    {
                        ui->passAction->setEnabled(true);
                        ui->passButton->setEnabled(true);
                    }
                    sender.storeStone(Gamestone::HOLDER);
                }
            }
            else
            {
                event.veto();
            }
            break;
    }
}

void MainWindow::moveToHolder(Gamestone& gamestone)
{
    // Get screen size
    QRect screen = QApplication::desktop()->screenGeometry();
    int screenHeight = screen.height();

    int y(10);
    int x(0);

    // If stone is joker, move completely to right
    if (gamestone.isJoker())
    {
        x = 14;
    }
    else
    {
        // Top of stone depends on color
        switch (gamestone.getColor())
        {
            case Gamestone::BLACK:  y = 0;
                                    break;
            case Gamestone::BLUE:   y = 1;
                                    break;
            case Gamestone::YELLOW: y = 2;
                                    break;
            case Gamestone::RED:    y = 3;
                                    break;
            default:                break;    // Avoid warning
        }
        // Calculate top position
        y = y * (gamestone.getHeight() + 10) + screenHeight / 200 + 10;

        // Init left position
        x = gamestone.getNumber();
    }

    // Calculate left position
    // Height, not width to calculate position so a little
    // space between the stones is added
    x = x * gamestone.getHeight() + screenHeight / 100;
    if (gamestone.isFirst())
    {
        x += 5;
    }
    else
    {
        x += 10;
        y += 5;
    }

    // Move the stone
    gamestone.move(QPoint(x, y));

    // Bring stones to front
    for (QVector<Gamestone*>::iterator
            stone(game->getStoneManager().getStones().begin());
            stone != game->getStoneManager().getStones().end();
            ++stone)
    {
        if ((*stone)->getParent() == Gamestone::HOLDER && !(*stone)->isFirst())
        {
            (*stone)->raise();
        }
    }
}

void MainWindow::moveToBoard(Gamestone& stone)
{
    // Calculate new x and y values of position
    int x(50);
    int y(20);
    int rowLength(stone.countStonesInRow() * stone.size().width());
    #ifdef _DEBUG
        std::cout << "Try to play out stone " << &stone << std::endl;
    #endif

    while (this->stoneUnderRow(x, y, rowLength))
    {
        y += 10;

        if (y + stone.pos().y() > ui->gameboard->size().height())
        {
            y = 20;
            x += 20;
        }
    }

    // Set position
    #ifdef _DEBUG
        std::cout << "Moving stone " << &stone;
        std::cout << " to pos (" << x << ", " << y << ")" << std::endl;
    #endif
    stone.move(QPoint(x, y));
}

bool MainWindow::stoneUnderRow(int x, int y, int rowLength)
{
    // Check if any of the stones are under the new position of the row
    QVector<Gamestone*> gamestones = game->getStoneManager().getStones();
    for (QVector<Gamestone*>::iterator stone(gamestones.begin());
            stone != gamestones.end();
            ++stone)
    {
        if ((*stone)->getParent() == Gamestone::BOARD &&
            (
                // Horizontal left end is in the row
                ((*stone)->pos().x() - MARGIN >= x
                    && (*stone)->pos().x() + MARGIN <= x + rowLength)
                // Horizontal right end is in the row
                || ((*stone)->pos().x() + (*stone)->size().width() + MARGIN >= x
                    && (*stone)->pos().x() + (*stone)->size().width() - MARGIN
                        <= x + rowLength)
            ) && (
                // Vertical top end is in the row
                ((*stone)->pos().y() - MARGIN >= y
                    && (*stone)->pos().y() + MARGIN
                        <= y + (*stone)->size().height())
                // Vertical bottom end is in the row
                || ((*stone)->pos().y() + (*stone)->size().height() + MARGIN
                        >= y
                    && (*stone)->pos().y() + (*stone)->size().height() - MARGIN
                        <= y + (*stone)->size().height())
            ))
        {
            #ifdef _DEBUG
                std::cout << " Stone at pos (" << x << ", " << y << "): "
                          << *stone << std::endl;
            #endif
            return true;
        }
    }

    return false;
}

void MainWindow::updateScrollElementMinSize()
{
    int holderMinX = 1;
    int holderMinY = 1;
    int gameboardMinX = 1;
    int gameboardMinY = 1;
    for (QVector<Gamestone*>::iterator
            stone(game->getStoneManager().getStones().begin());
            stone != game->getStoneManager().getStones().end();
            ++stone)
    {
        if ((*stone)->getParent() == Gamestone::HOLDER)
        {
            if ((*stone)->pos().x() + (*stone)->size().width() > holderMinX)
            {
                holderMinX = (*stone)->pos().x()
                        + (*stone)->size().width() + 10;
            }
            if ((*stone)->pos().y() + (*stone)->size().height() > holderMinY)
            {
                holderMinY = (*stone)->pos().y()
                        + (*stone)->size().height() + 10;
            }
        }

        if ((*stone)->getParent() == Gamestone::BOARD)
        {
            if ((*stone)->pos().x() + (*stone)->size().width() > gameboardMinX)
            {
                gameboardMinX = (*stone)->pos().x()
                        + (*stone)->size().width() + 10;
            }
            if ((*stone)->pos().y() + (*stone)->size().height() > gameboardMinY)
            {
                gameboardMinY = (*stone)->pos().y()
                        + (*stone)->size().height() + 10;
            }
        }
    }

    ui->holder->setMinimumSize(holderMinX, holderMinY);
    ui->gameboard->setMinimumSize(gameboardMinX, gameboardMinY);
}

void MainWindow::startNextRound()
{
    // Check if all rules have been observed
    if (!game->testStonesOnGameboard())
    {
        // If not, ask if stones should be reset
        QMessageBox ruleFailed(this);
        ruleFailed.setWindowTitle("RummyCube");
        ruleFailed.setText(tr("At least one stone doesn't lie on the game board"
            " as told in the rules (shown in gray). Do you want to continue and"
            " move ALL stones to the position they had at the beginning"
            " of the round?"));
        ruleFailed.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        ruleFailed.setIcon(QMessageBox::Warning);
        if (ruleFailed.exec() == QMessageBox::No)
        {
            return;
        }
        else
        {
            QVector<Gamestone*> gamestones(game->getStoneManager().getStones());
            for (QVector<Gamestone*>::iterator stone(gamestones.begin());
                    stone != gamestones.end();
                    ++stone)
            {
                (*stone)->restoreStone();
                (*stone)->setInvalid(false);
            }
        }
    }

    this->setCursor(Qt::WaitCursor);
    ui->passAction->setEnabled(false);
    ui->passButton->setEnabled(false);
    ui->loadGameAction->setEnabled(false);
    ui->saveGameAction->setEnabled(false);
    Gamestone::canMoveStones = false;
    game->getTimer()->pause();

    if (gameState != GAME_JOIN_NETWORK_GAME)
    {
        // Notifies the opponents that the round is finished
        game->roundFinished();

        if (game->getHumanPlayer()->hasWon())
        {
            QMessageBox youWon(this);
            youWon.setWindowTitle("RummyCube");
            youWon.setText(tr("You won the game. Congratulations! Do you want"
                    " to start a new singleplayer game? Otherwise the game will"
                    " be closed."));
            youWon.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            youWon.setIcon(QMessageBox::Question);
            if (youWon.exec() == QMessageBox::Yes)
            {
                this->restartGame();
            }
            else
            {
                forceClose = true;
                this->close();
            }
            return;
        }
    }

    gameState = GAME_OPPONENT_PLAYING;
    std::deque<OpponentPlayer*> opponentPlayers = game->getOpponentPlayers();
    for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin());
            player != opponentPlayers.end();
            ++player)
    {
        ui->infoLabel->setText(tr("Player %1: I'm playing")
                .arg((*player)->getPlayerName()));
        ui->infoLabel->setToolTip(tr("Player %1: I'm playing")
                .arg((*player)->getPlayerName()));
        QCoreApplication::processEvents();

        (*player)->play();

        this->updateScrollElementMinSize();

        if ((*player)->hasWon())
        {
            QMessageBox opponentWon(this);
            opponentWon.setWindowTitle("RummyCube");
            opponentWon.setText(tr("Player %1 won the game.\n"
                    "Do you want to try it one more time?"
                    " Otherwise the game will be closed.")
                    .arg((*player)->getPlayerName()));
            opponentWon.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            opponentWon.setIcon(QMessageBox::Question);
            if (opponentWon.exec() == QMessageBox::Yes)
            {
                this->restartGame();
                return;
            }
            else
            {
                forceClose = true;
                this->close();
            }
        }
    }

    // Check gamestate, can be changed during message queue refresh
    // calls in (*player)->play()
    GameState state = gameState;
    gameState = GAME_IDLE;
    switch (state)
    {
        case GAME_CLOSING          :    forceClose = true;
                                        this->close();
                                        return;
        case GAME_RESTART_GAME     :
        case GAME_JOIN_NETWORK_GAME:    this->restartGame();
                                        return;
        default:                        break;
    }

    Gamestone::canMoveStones = true;

    game->getTimer()->start();

    game->getHumanPlayer()->newRound();
    this->setInfoLabelText();

    if (game->getStoneManager().getStoneCountOnHeap() == 0)
    {
        ui->passButton->setEnabled(true);
        ui->passAction->setEnabled(true);
    }
    ui->loadGameAction->setEnabled(true);
    ui->saveGameAction->setEnabled(true);

    this->setCursor(Qt::ArrowCursor);
}

void MainWindow::changeEvent(QEvent* event)
{
    if (event != NULL && event->type() == QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
        setInfoLabelText();
        setTimerLabelText();
    }
    QMainWindow::changeEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::DragEnter:
        {
            QDragEnterEvent* dragEnterEvent = static_cast<QDragEnterEvent*>(event);
            this->dragEnterEvent(obj, dragEnterEvent);
            return true;
        }

        case QEvent::Drop:
        {
            QDropEvent* dropEvent = static_cast<QDropEvent*>(event);
            this->dropEvent(obj, dropEvent);
            return true;
        }

        default:
            // standard event processing
            return QObject::eventFilter(obj, event);
    }
}

void MainWindow::dragEnterEvent(QObject* target, QDragEnterEvent* event)
{
    // We need to check here for ignore action since a gamestone might already have
    // handled the event
    if (event->mimeData()->hasFormat(GAMESTONE_MIMETYPE)
            && event->dropAction() != Qt::IgnoreAction)
    {
        // Verify if can really be dropped back on holder.
        // Need to verify complete row, not just single stone to make sure
        // we can drop the complete row on holder.
        if (target == ui->holder)
        {
            Gamestone* stone = (Gamestone*)(event->source());

            while (stone != NULL)
            {
                if (!stone->isFirstTimePlayedOut())
                {
                    return;
                }
                stone = stone->getNext();
            }

            stone = (Gamestone*)(event->source());
            while (stone != NULL)
            {
                if (!stone->isFirstTimePlayedOut())
                {
                    return;
                }
                stone = stone->getPrev();
            }

            event->acceptProposedAction();
            return;
        }
        else if (target == ui->gameboard)
        {
            event->acceptProposedAction();
            return;
        }
    }
}

void MainWindow::dropEvent(QObject* target, QDropEvent* event)
{
    // We need to check here for ignore action since a gamestone might already have
    // handled the event
    if (event->mimeData()->hasFormat(GAMESTONE_MIMETYPE)
            && event->dropAction() != Qt::IgnoreAction)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();

        QByteArray itemData = event->mimeData()->data(GAMESTONE_MIMETYPE);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPoint offset;
        dataStream >> offset;

        // Calculate center of stone, needed to check for overlapping stones
        Gamestone* source = (Gamestone*)(event->source());
        QPoint stoneCenter = event->pos() - offset
            + QPoint(source->getWidth() / 2, source->getHeight() / 2);

        if (target == ui->gameboard)
        {
            Gamestone* overlappedStone =
                    qobject_cast<Gamestone*>(ui->gameboard->childAt(stoneCenter));

            bool stoneAlreadyMoved = false;
            if (overlappedStone != NULL)
            {
                QDragEnterEvent enterEvent(event->pos(), event->dropAction(), event->mimeData(),
                    event->mouseButtons(), event->keyboardModifiers());
                overlappedStone->dragEnterEvent(&enterEvent);
                if (enterEvent.dropAction() != Qt::IgnoreAction)
                {
                    overlappedStone->dropEvent(event, true);
                    stoneAlreadyMoved = true;
                }
            }

            if (!stoneAlreadyMoved)
            {
                source->setParent(Gamestone::BOARD, true);

                source->move(event->pos() - offset);
                source->moveAddedStones();
            }
        }
        else
        {
            Gamestone* overlappedStone = qobject_cast<Gamestone*>(ui->holder->childAt(stoneCenter));

            bool stoneAlreadyMoved = false;
            if (overlappedStone != NULL)
            {
                QDragEnterEvent enterEvent(event->pos(), event->dropAction(), event->mimeData(),
                    event->mouseButtons(), event->keyboardModifiers());
                overlappedStone->dragEnterEvent(&enterEvent);
                if (enterEvent.dropAction() != Qt::IgnoreAction)
                {
                    overlappedStone->dropEvent(event, true);
                    stoneAlreadyMoved = true;
                }
            }

            if (!stoneAlreadyMoved)
            {
                source->setParent(Gamestone::HOLDER, true);

                source->move(event->pos() - offset);
                source->moveAddedStones();
            }
        }

        // Calculate new minimum height of parent widget
        int minHeight = ((QWidget*)source->parent())->minimumSize().height();
        if (source->pos().y() + source->size().height() > minHeight)
        {
            minHeight = source->pos().y() + source->size().height() + 10;
        }

        // Minimum width is more complex. We first need to count the stones
        // wich are more right of our current stone in the current row to get
        // the proper width.
        int rowSize = source->size().width();
        Gamestone* curr = source;
        while (!curr->isLastInRow())
        {
            rowSize += source->size().width();
            curr = curr->getNext();
        }

        int minWidth = ((QWidget*)source->parent())->minimumSize().width();
        if (source->pos().x() + rowSize > minWidth)
        {
            minWidth = source->pos().x() + rowSize + 10;
        }

        ((QWidget*)source->parent())->setMinimumSize(minWidth, minHeight);
    }
}

void MainWindow::newGameEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    QMessageBox newGame(this);
    newGame.setWindowTitle("RummyCube");
    newGame.setText(tr("Do you really want to close the current game and"
            " start a new one?"));
    newGame.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    newGame.setIcon(QMessageBox::Question);
    if (newGame.exec() == QMessageBox::Yes)
    {
        if (gameState != GAME_IDLE)
        {
            gameState = GAME_RESTART_GAME;
            game->stopGame();
        }
        else
        {
            this->restartGame();
        }
    }

    // Resume timer if was running
    else if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::loadGameEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    QFileDialog dialog(this, tr("Load RummyCube savegame"), "",
            tr("RummyCube Savegame (*.rcs)"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);

    if (dialog.exec() == QDialog::Accepted)
    {
        QFile loadFile(dialog.selectedFiles().at(0));

        if (!loadFile.open(QIODevice::ReadOnly))
        {
            showError(tr("Could not open file for loading. %1")
                    .arg(loadFile.errorString()));
        }
        else
        {
            QXmlStreamReader gameReader(&loadFile);

            // Try to load game, disable the window while loading to avoid
            // interactions
            RummyCube* oldGame = game;
            game = new RummyCube();
            initStones();
            this->setEnabled(false);
            gameReader >> game;
            this->setEnabled(true);

            // Loading failure, show error message and return
            if (gameReader.hasError())
            {
                showError(tr("Error during file loading. %1")
                        .arg(gameReader.errorString()));

                game = oldGame;

                // Resume timer if was running
                if (timeLimitRunning)
                {
                    game->getTimer()->resume();
                }

                return;
            }
            else
            {
                // Clean up backups
                delete oldGame;
            }

            // Loading succeded, do some final updates on gui
            if (game->getHumanPlayer()->getStonesLeftToTake() == 0
                    || game->getStoneManager().getStoneCountOnHeap() == 0)
            {
                ui->passButton->setEnabled(true);
            }
            else
            {
                ui->passButton->setEnabled(false);
            }
            ui->passAction->setEnabled(ui->passButton->isEnabled());
            this->setInfoLabelText();
            this->setTimerLabelText();
            this->updateScrollElementMinSize();

            return;
        }
    }

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::saveGameEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    QFileDialog dialog(this, tr("Save current game"), "",
            tr("RummyCube Savegame (*.rcs)"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("rcs");

    if (dialog.exec() == QDialog::Accepted)
    {
        QFileInfo filename(dialog.selectedFiles().at(0));

        QSaveFile saveFile(filename.absoluteFilePath());
        if (!saveFile.open(QIODevice::WriteOnly))
        {
            showError(tr("Could not open savegame file to write. %1")
                    .arg(saveFile.errorString()));
        }
        else
        {
            QXmlStreamWriter saveGameWriter(&saveFile);
            saveGameWriter << game;

            if (!saveFile.commit())
            {
                showError(tr("Could not write savegame. %1")
                        .arg(saveFile.errorString()));
            }
        }
    }

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::tutorialEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Save game to restore status after tutorials have been finished
    QString saveGame;
    QXmlStreamWriter saveGameWriter(&saveGame);
    saveGameWriter << game;

    QVector<Gamestone*> gamestones(game->getStoneManager().getStones());
    for (QVector<Gamestone*>::iterator stone(gamestones.begin());
            stone != gamestones.end();
            ++stone)
    {
        (*stone)->hide();
    }

    Tutorial(this, this->ui->heap, this->ui->gameboard, this->ui->holder).exec();

    // Restore game
    this->setEnabled(false);
    QXmlStreamReader gameReader(saveGame);
    gameReader >> game;

    this->setEnabled(true);

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::settingsEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    SettingsWindow(this).exec();

    // Change images to new values
    Settings* settings = Settings::getInstance();
    ((ImagePanel*)ui->holder)->setImage(
            settings->value("General/HolderPath").toString());
    ((ImagePanel*)ui->gameboard)->setImage(
            settings->value("General/GameboardPath").toString());

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::moveAllStonesEvent(bool checked)
{
    game->getStoneManager().moveAllStonesInRow = checked;

    // Update main menu item, context menu item is updated on creation
    // of context menu
    ui->moveAllStonesAction->setChecked(checked);
    ui->moveAllStonesLabel->setEnabled(checked);

    if (checked)
    {
        ui->moveAllStonesLabel->setToolTip(tr("Move all stones in a row"));
    }
    else
    {
        ui->moveAllStonesLabel->setToolTip(tr("Move just single stones"));
    }
}

void MainWindow::languageChangedEvent(QAction* action)
{
    if (action != NULL)
    {
        if (action == ui->defaultLanguageAction)
        {
            i18n->setLanguage(NULL);
        }
        else
        {
            Language lang = action->data().value<Language>();
            i18n->setLanguage(&lang);
        }
    }
}

void MainWindow::createNetworkGameEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    CreateNetworkGameWindow createNetworkGameWindow(this);
    if (createNetworkGameWindow.exec())
    {
        this->nextGame = createNetworkGameWindow.getGame();

        std::deque<OpponentPlayer*> opponents =
                this->nextGame->getOpponentPlayers();
        for (std::deque<OpponentPlayer*>::iterator
                player(opponents.begin());
                player != opponents.end();
                ++player)
        {
            NetworkPlayer* networkPlayer = (NetworkPlayer*) *player;
            QTcpSocket& sock(networkPlayer->getSocket());
            connect(&sock, SIGNAL(disconnected()),
                     this, SLOT(opponentDisconnected()));
            connect(networkPlayer, SIGNAL(connectionError),
                             this, SLOT(connectionError));
        }

        if (this->gameState == GAME_IDLE)
        {
            this->restartGame();
        }
        else
        {
            this->gameState = GAME_RESTART_GAME;
            game->stopGame();
        }

        return;
    }

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::joinNetworkGameEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    JoinNetworkGameWindow joinNetworkGame(this);

    if (joinNetworkGame.exec())
    {
        this->nextGame = joinNetworkGame.getGame();

        std::deque<OpponentPlayer*> opponents =
                this->nextGame->getOpponentPlayers();
        for (std::deque<OpponentPlayer*>::iterator
                player(opponents.begin());
                player != opponents.end();
                ++player)
        {
            NetworkPlayer* networkPlayer = (NetworkPlayer*) *player;
            connect(&(networkPlayer->getSocket()), SIGNAL(disconnected()),
                                             this, SLOT(opponentDisconnected()));
            connect(networkPlayer, SIGNAL(connectionError),
                             this, SLOT(connectionError));
        }

        GameState currState = this->gameState;
        this->gameState = GAME_JOIN_NETWORK_GAME;
        if (currState == GAME_IDLE)
        {
            this->restartGame();
        }
        else
        {
            game->stopGame();
        }

        return;
    }

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::testStonesOnGameboardEvent()
{
    game->testStonesOnGameboard();
}

void MainWindow::aboutEvent()
{
    // First stop timer
    bool timeLimitRunning = game->getTimer()->isRunning();
    game->getTimer()->pause();

    // Show dialog
    AboutWindow(this).exec();

    // Resume timer if was running
    if (timeLimitRunning)
    {
        game->getTimer()->resume();
    }
}

void MainWindow::timeLimitEvent()
{
    setTimerLabelText();

    // If no more time is left, force round finish
    if (game != NULL
            && game->getTimer() != NULL
            && game->getTimer()->getTime() > -1
            && game->getTimer()->timeLeft() == 0)
    {
        // If the player still has some stones left to take,
        // take random stones from holder
        this->takeAllStones();

        if (!game->testStonesOnGameboard())
        {
            QMessageBox ruleFailed(this);
            ruleFailed.setWindowTitle("RummyCube");
            ruleFailed.setText(tr("At least one stone doesn't lie on the game"
                    " board as told in the rules (shown in gray). ALL stones"
                    " will be moved to the position they had at the beginning"
                    " of the round!"));
            ruleFailed.setStandardButtons(QMessageBox::Ok);
            ruleFailed.setIcon(QMessageBox::Warning);
            ruleFailed.exec();

            QVector<Gamestone*> gamestones(game->getStoneManager().getStones());
            for (QVector<Gamestone*>::iterator stone(gamestones.begin());
                    stone != gamestones.end();
                    ++stone)
            {
                (*stone)->restoreStone();
                (*stone)->setInvalid(false);
            }
        }

        // Continue next round
        this->startNextRound();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // By default accept closing the game
    event->accept();

    if (forceClose)
    {
        // If opponent is currently playing, we need to finish the
        // startNextRound method. It will then force close the window.
        if (gameState != GAME_IDLE)
        {
            gameState = GAME_CLOSING;
            game->stopGame();
            event->ignore();
        }
    }
    else
    {
        // First stop timer
        bool timeLimitRunning = game->getTimer()->isRunning();
        game->getTimer()->pause();

        // Show dialog
        // Security question on window close
        QMessageBox closeGame(this);
        closeGame.setWindowTitle("RummyCube");
        closeGame.setText(tr("Do you really want to close the game?"));
        closeGame.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        closeGame.setIcon(QMessageBox::Question);
        switch (closeGame.exec())
        {
            case QMessageBox::Yes:
                // If opponent is currently playing, we need to finish the
                // startNextRound method. It will then force close the window.
                if (gameState == GAME_OPPONENT_PLAYING)
                {
                    game->stopGame();
                    event->ignore();
                }

                gameState = GAME_CLOSING;
                return;
            case QMessageBox::No:
                event->ignore();
                break;
        }

        // Resume timer if was running
        if (timeLimitRunning)
        {
            game->getTimer()->resume();
        }
    }
}

void MainWindow::connectionError(QString errorMessage)
{
    // Show dialog
    QMessageBox connectionError(this);
    connectionError.setWindowTitle("RummyCube");
    connectionError.setText(tr("Error during network connection: %1. Do you"
            " want to start a new singleplayer game? Otherwise the game will be"
            " closed.").arg(errorMessage));
    connectionError.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    connectionError.setIcon(QMessageBox::Question);
    if (connectionError.exec() == QMessageBox::Yes)
    {
        if (gameState == GAME_IDLE)
        {
            this->restartGame();
        }
        else
        {
            gameState = GAME_RESTART_GAME;
            game->stopGame();
        }
    }
    else
    {
        forceClose = true;
        this->close();
    }
}

void MainWindow::opponentDisconnected()
{
    // If we close the game we don't care if opponents disconnect
    if (this->gameState == GAME_CLOSING || this->forceClose)
    {
        return;
    }

    // First stop timer
    game->getTimer()->pause();

    nextGame = NULL;

    // Show dialog
    QMessageBox opponentDisconnected(this);
    opponentDisconnected.setWindowTitle("RummyCube");
    opponentDisconnected.setText(tr("Your opponent left the game. Do you want"
            " to start a new one? Otherwise the game will be closed."));
    opponentDisconnected.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    opponentDisconnected.setIcon(QMessageBox::Question);
    if (opponentDisconnected.exec() == QMessageBox::Yes)
    {
        if (gameState == GAME_IDLE)
        {
            this->restartGame();
        }
        else
        {
            gameState = GAME_RESTART_GAME;
            game->stopGame();
        }
    }
    else
    {
        forceClose = true;
        this->close();
    }
}
