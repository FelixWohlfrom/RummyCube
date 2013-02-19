/*
 * MainWindow.h
 *
 *  Created on: 21.07.2011
 *      Author: Felix Wohlfrom
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include "ImagePanel.h"
#include "../game/RummyCube.h"
#include "../game/stones/ParentChangedEvent.h"
#include "../network/NetworkGameClosedEvent.h"

#include <wx/menu.h>
#include <wx/timer.h>
#include <wx/frame.h>
#include <wx/panel.h>
#include <wx/socket.h>
#include <wx/stattext.h>

/**
 * The main window of the game
 */
class MainWindow : public wxFrame
{
	public:
		/**
		 * Constructor. Initializes the window elements
		 */
		MainWindow();

		/**
		 * Destructor. Stores settings
		 */
		virtual ~MainWindow();

	private:
		/**
		 * IDs for the main menu items which are not available already
		 * in wxWidgets
		 */
		enum
		{
		    ID_PASS = 100,
		    ID_TUTORIAL,
		    ID_MOVEALLSTONES,
		    ID_SOUNDS,
		    // ID_LANGUAGE, // Just for the sake of completeness, languages are handled using language id
		    ID_CREATENETWORKGAME,
		    ID_JOINNETWORKGAME,
		    ID_TESTSTONES,
		    ID_UPDATE,

			// Timer id
			ID_UPDATETIMERLABEL
		};

		/**
		 * The game state. Needed for asynchronous menu event handling
		 */
		enum GameState
		{
			GAME_IDLE,
			GAME_OPPONENT_PLAYING,
			GAME_CLOSING,
			GAME_RESTARTING,
			GAME_CREATE_NETWORKGAME,
			GAME_JOIN_NETWORKGAME
		};

		/**
		 * The current state of the game
		 */
		GameState gameState;

		/**
		 * The game model
		 */
		RummyCube* game;

		/**
		 * The heap contains the stones on game startup
		 */
		wxPanel* heap;

		/**
		 * The game board on which the stones are played out
		 */
		ImagePanel* board;

		/**
		 * The holder for the player.
		 * Contains the stones that the player can play out
		 */
		ImagePanel* holder;

		/**
		 * The information label
		 */
		wxStaticText* infoLabel;

		/**
		 * The timer label
		 */
		wxStaticText* timerLabel;

		/**
		 * The timerlabel update timer
		 */
		wxTimer timerLabelUpdater;

		/**
		 * The main menu
		 */
		wxMenuBar* mainMenu;

		/**
		 * Generates the main menu
		 */
		void generateMainMenu();

		/**
		 * Shows the context menu
		 *
		 * @param e The context event
		 */
		void ShowContextMenu(wxContextMenuEvent& e);

		/**
		 * This event is called when the main window gets the focus.
		 * Just needed on windows for stone initialization
		 *
		 * @param e The focus event
		 */
		#ifdef _WIN32
			void OnFocus(wxFocusEvent& WXUNUSED(e));
		#endif

		/**
		 * Stops all opponent players that are currently playing.
		 * Just called before closing or starting a new game.
		 */
		void stopOpponentPlayers();

		/**
		 * Destroys the window
		 *
		 * @param e The close event
		 */
		void OnClose(wxCloseEvent& e);

		/**
		 * Refits the main window elements
		 *
		 * @param e The size event
		 */
		void OnResize(wxSizeEvent& e);

		/**
		 * Reinitializes the players, game settings and stuff
		 */
		void restartGame();

		/**
		 * Initializes the stones
		 */
		void initStones();

		/**
		 * Updates the info label
		 */
		void setInfoLabelText();

		/**
		 * Updates the timer label
		 *
		 */
		void setTimerLabelText();

		/**
		 * Takes random stones from heap to board.
		 * The stone count is the count that the human player has left to take.
		 *
		 * @param e The mouse event while clicking
		 */
		void takeAllStones(wxMouseEvent& WXUNUSED(e));

		/**
		 * Called when a gamestone changed the parent
		 *
		 * @param e The parent changed event
		 */
		void OnGamestoneParentChanged(ParentChangedEvent& e);

		/**
		 * Sets the gamestone on the holder depending
		 * on stone color and number
		 *
		 * @param gamestone The gamestone that has to be set
		 */
		void moveToHolder(Gamestone& gamestone);

		/**
		 * Moves the gamestone to a free position on the board
		 */
		void moveToBoard(Gamestone& gamestone);

		/**
		 * Checks, if a stone is on game board on the position that the row will take
		 *
		 * @param x The horizontal position of the top left point of the row
		 * @param y The vertical position of the top left point of the row
		 * @param rowLength The length of the row in pixels
		 */
		bool stoneUnderRow(int x, int y, int rowLength);

		/**
		 * The handler for the "new game" menu entry
		 *
		 * @param e The event
		 */
		void OnNewGame(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "next round" menu entry
		 *
		 * @param e The event
		 */
		void OnNextRound(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "load game" menu entry
		 *
		 * @param e The event
		 */
		void OnLoadGame(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "load game" menu entry
		 *
		 * @param e The event
		 */
		void OnSaveGame(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "tutorial" menu entry
		 *
		 * @param e The event
		 */
		void OnShowTutorial(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "close game" menu entry
		 *
		 * @param e The event
		 */
		void OnExit(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "properties" menu entry
		 *
		 * @param e The event
		 */
		void OnProperties(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "move all stones" menu entry
		 *
		 * @param e The event
		 */
		void OnMoveAllStones(wxCommandEvent& e);

		/**
		 * The handler for the "language" menu entry
		 *
		 * @param e The event
		 */
		void OnChangeLanguage(wxCommandEvent& e);

		/**
		 * The handler for the "create network game" menu entry
		 *
		 * @param e The event
		 */
		void OnCreateNetworkGame(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "join a game" menu entry
		 *
		 * @param e The event
		 */
		void OnJoinNetworkGame(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "test stones on gameboard" menu entry
		 *
		 * @param e The event
		 */
		void OnTestStonesOnGameboard(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the "about rummycube" menu entry
		 *
		 * @param e The event
		 */
		void OnAboutRummyCube(wxCommandEvent& WXUNUSED(e));

		/**
		 * The handler for the time limit updater
		 *
		 * @param e The event
		 */
		void OnTimeLimitTimer(wxTimerEvent& WXUNUSED(e));

		/**
		 * The handler for the events that are fired on network game closing
		 *
		 * @param e The event
		 */
		void OnNetworkGameClosed(NetworkGameClosedEvent& e);

		// Needed for wxwidgets
	    DECLARE_EVENT_TABLE()
};

#endif /* MAINWINDOW_H_ */
