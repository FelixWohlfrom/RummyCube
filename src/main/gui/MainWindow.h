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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../game/RummyCube.h"
#include "../i18n/Internationalization.h"
#include "../game/stones/ParentChangedEvent.h"

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QContextMenuEvent>

namespace Ui {
    class MainWindow;
}

/**
 * The main game window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window. Optional
         */
        explicit MainWindow(QWidget *parent = 0);

        /**
         * Destructor. Stores settings.
         */
        ~MainWindow();

    protected:
        /**
         * Overwrites change event of QMainWindow. Handles language changes.
         *
         * @param event The change event.
         */
        void changeEvent(QEvent* event);

        /**
         * Event filter to allow usage of dragEnterEvent and dropEvent
         * for both gameboard and holder.
         *
         * @param obj The target object that sent the event
         * @param event The sent event
         *
         * @return If the event was filtered
         */
        bool eventFilter(QObject* obj, QEvent* event);

        /**
         * Listener for event when stone is dragged over an element on main
         * window.
         *
         * @param target The target over which the element is dragged
         * @param event The sent event
         */
        void dragEnterEvent(QObject* target, QDragEnterEvent* event);

        /**
         * Listener for event when stone is dropped on an element on main
         * window.
         *
         * @param target The target object that sent the event
         * @param event The sent event
         */
        void dropEvent(QObject* target, QDropEvent* event);

    private:
        /**
         * The main window ui.
         */
        Ui::MainWindow* ui;

        /**
         * Internationalization instance.
         */
        Internationalization* i18n;

        /**
         * If the user should be aksed if he wants to quit or the window
         * should be forcefully quited (e.g. because he won the game and
         * was already asked if he wants to continue a new game).
         */
        bool forceClose;

        /**
         * The game state. Needed for asynchronous menu event handling.
         */
        enum GameState
        {
            GAME_IDLE,
            GAME_OPPONENT_PLAYING,
            GAME_CLOSING,
            GAME_RESTART_GAME,
            GAME_JOIN_NETWORK_GAME
        };

        /**
         * The current state of the game.
         */
        GameState gameState;

        /**
         * The game model.
         */
        RummyCube* game;

        /**
         * The model of the following game.
         * Needed if currently the ai is playing and needs to be stopped first.
         */
        RummyCube* nextGame;

        /**
         * Stops all opponent players that are currently playing.
         * Just called before closing or starting a new game.
         */
        void stopOpponentPlayers();

        /**
         * Initializes the stones.
         */
        void initStones();

        /**
         * Updates the info label.
         */
        void setInfoLabelText();

        /**
         * Updates the timer label.
         */
        void setTimerLabelText();

        /**
         * Sets the gamestone on the holder depending
         * on stone color and number.
         *
         * @param gamestone The gamestone that has to be set.
         */
        void moveToHolder(Gamestone& gamestone);

        /**
         * Moves the gamestone to a free position on the board.
         *
         * @param gamestone The gamestone that is moved to the board.
         */
        void moveToBoard(Gamestone& gamestone);

        /**
         * Checks, if a stone is on game board on the position that the row
         * will take.
         *
         * @param x The horizontal position of the top left point of the row
         * @param y The vertical position of the top left point of the row
         * @param rowLength The length of the row in pixels
         *
         * @return If a stone is blocking the position
         */
        bool stoneUnderRow(int x, int y, int rowLength);

        /**
         * Updates minimum size of gameboard and holder to allow scrolling
         */
        void updateScrollElementMinSize();

        /**
         * This event is called when the main window gets the focus.
         *
         * @param event The show event
         */
        void showEvent(QShowEvent* event);

        /**
         * Called on window resizing.
         *
         * @param event The resize event
         */
        void resizeEvent(QResizeEvent* event);

        /**
         * Shows the context menu.
         *
         * @param event The context menu event
         */
        void contextMenuEvent(QContextMenuEvent* event);

        /**
         * Util to display an error message.
         */
        void showError(QString message);

    private slots:
        /**
         * Reinitializes the players, game settings and stuff.
         */
        void restartGame();

        /**
         * Called when a gamestone changed the parent.
         *
         * @param event The parent changed event
         */
        void gamestoneParentChanged(ParentChangedEvent& event);

        /**
         * Takes random stones from heap to board.
         * The stone count is the count that the human player has left to take.
         */
        void takeAllStones();

        /**
         * Finishes the current round played by human player and lets the other
         * players play.
         */
        void startNextRound();

        /**
         * The handler for the "new game" menu entry.
         */
        void newGameEvent();

        /**
         * The handler for the "load game" menu entry.
         */
        void loadGameEvent();

        /**
         * The handler for the "save game" menu entry.
         */
        void saveGameEvent();

        /**
         * The handler for the "settings" menu entry.
         */
        void settingsEvent();

        /**
         * The handler for the "move all stones" menu entry.
         *
         * @param checked If the menu entry is checked or not
         */
        void moveAllStonesEvent(bool checked);

        /**
         * The handler for a click on a language.
         *
         * @param action The selected language action.
         */
        void languageChangedEvent(QAction* action);

        /**
         * The handler for the "create new network game" menu entry.
         */
        void createNetworkGameEvent();

        /**
         * The handler for the "join network game" menu entry.
         */
        void joinNetworkGameEvent();

        /**
         * The handler for the "test stones on gameboard" menu entry.
         */
        void testStonesOnGameboardEvent();

        /**
         * The handler for the "about rummycube" menu entry.
         */
        void aboutEvent();

        /**
         * Time limit handler.
         */
        void timeLimitEvent();

        /**
         * Handles window closing (e.g. asks user if he really wants to close).
         *
         * @param event The close event that allows to intercept closing
         */
        void closeEvent(QCloseEvent *event);

        /**
         * Handles errors during network connections.
         *
         * @param errorMessage The error message
         */
        void connectionError(QString errorMessage);

        /**
         * Event handler if a network opponent disconnected.
         */
        void opponentDisconnected();
};

#endif // MAINWINDOW_H
