/*
 * JoinNetworkGameWindow.h
 *
 *  Created on: 31.03.2012
 *      Author: Felix Wohlfrom
 */

#ifndef JOINNETWORKGAMEWINDOW_H_
#define JOINNETWORKGAMEWINDOW_H_

#include "../game/RummyCubeNetwork.h"
#include "../network/NetworkGameSettings.h"

#include <list>

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/socket.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

/**
 * This class represents the window that is shown to connect to a network game
 */
class JoinNetworkGameWindow : public wxDialog
{
	public:
		/**
		 * Constructor. Initializes the window elements
		 *
		 * @param parent The parent window
		 */
		JoinNetworkGameWindow(wxWindow* parent);

		/**
		 * Destructor. Dummy to avoid warnings
		 */
		virtual ~JoinNetworkGameWindow();

		/**
		 * Returns the created game object
		 */
		RummyCubeNetwork* getGame();

	private:
		/**
		 * IDs for wxwidgets
		 */
		enum
		{
			ID_CONNECTTOSERVER = 1,
			ID_SOCKET
		};

		/**
		 * The network game
		 */
		RummyCubeNetwork* game;

		/**
		 * The socket which is used to communicate with the server
		 */
		wxSocketClient* sock;

		/**
		 * If we are connected to an server or not
		 */
		bool connected;

		/**
		 * Stores if remote player started game
		 */
		bool canStartGameRemote;

		/**
		 * Stores if current player started game
		 */
		bool canStartGameLocal;

		/**
		 * The name of the connected player
		 */
		std::list<wxString> connectedPlayer;

		/**
		 * The main sizer of the window
		 */
		wxBoxSizer* mainSizer;

		/**
		 * Hostname/ip address of the server
		 */
		 wxTextCtrl* hostname;

		 /**
		  * The nickname of the player
		  */
		 wxTextCtrl* nickname;

		 /**
		  * The button to connect to the server
		  */
		wxButton* connectButton;

		/**
		 * Label for the stones that can be taken in first round
		 */
		wxStaticText* stonesInFirstRound;

		/**
		 * Label for the sum that has to be reached on first playout
		 */
		wxStaticText* sumInFirstRound;

		/**
		 * If the stone sum has to be in one row
		 */
		wxStaticText* stonesInOneRow;

		/**
		 * Time limit of the game
		 */
		wxStaticText* timeLimit;

		/**
		 * The start game button
		 */
		wxButton* startGameButton;

		/**
		 * The settings of the network game
		 */
		NetworkGameSettings settings;

		/**
		 * Shows settings
		 */
		void showSettingsPanel();

		/**
		 * Shows the waiting panel
		 */
		void showWaitingPanel();

		/**
		 * Updates the settings on gui
		 */
		void updateGui();

		/**
		 * Starts the game
		 */
		void startGame();

		/**
		 * Disconnects from server
		 */
		void disconnect();

		/**
		 * Parses the data received from socket
		 */
		void parseData();

		/**
		 * Event handler for connect button
		 *
		 * @param e The event
		 */
		void OnConnectClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for ok button
		 *
		 * @param e The event
		 */
		void OnOkClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for the cancel button
		 *
		 * param e The event
		 */
		void OnCancelClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for window closing
		 *
		 * @param e The event
		 */
		void OnClose(wxCloseEvent& WXUNUSED(e));

		/**
		 * Event handler for socket events
		 *
		 * @param e The event
		 */
		void OnSocketEvent(wxSocketEvent& e);

		/**
		 * wxWidgets Event table
		 */
		DECLARE_EVENT_TABLE()
};


#endif /* JOINNETWORKGAMEWINDOW_H_ */
