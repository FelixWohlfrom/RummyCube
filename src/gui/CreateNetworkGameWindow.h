/*
 * CreateNetworkGameWindow.h
 *
 *  Created on: 10.03.2012
 *      Author: Felix Wohlfrom
 */

#ifndef CREATENETWORKGAMEWINDOW_H_
#define CREATENETWORKGAMEWINDOW_H_

#include <map>

#include "../game/RummyCubeNetwork.h"

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/slider.h>
#include <wx/socket.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>

/**
 * The window to create a new network game
 */
class CreateNetworkGameWindow : public wxDialog
{
	public:
		/**
		 * Constructor. Initializes the window elements
		 *
		 * @param parent The parent window
		 */
		CreateNetworkGameWindow(wxWindow* parent);

		/**
		 * Destructor. Dummy to avoid warnings
		 */
		virtual ~CreateNetworkGameWindow();

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
			ID_SPINCTRLCHANGE = 1,
			ID_STONESINONEROW,
			ID_TIMELIMITENABLED,
			ID_TIMELIMITVALUE,
			ID_SERVER,
			ID_SOCKET
		};

		/**
		 * The network game
		 */
		RummyCubeNetwork* game;

		/**
		 * The server object
		 */
		wxSocketServer* server;

		/**
		 * The client sockets and the associated player names
		 * Key is the socket, value is the name of the player
		 */
		std::map<wxSocketBase*, wxString> connectedPlayer;

		/**
		 * Stores if remote player started game
		 */
		bool canStartGameRemote;

		/**
		 * Stores if current player started game
		 */
		bool canStartGameLocal;

		/**
		 * The main sizer of the window
		 */
		wxBoxSizer* mainSizer;

		/**
		 * Selector for stones to take in first round
		 */
		wxSpinCtrl* stonesInFirstRound;

		/**
		 * Selector for sum that has to be played out first time by the players
		 */
		wxSpinCtrl* sumInFirstRound;

		/**
		 * If the stone sum has to be in one row
		 */
		wxCheckBox* stonesInOneRow;

		/**
		 * If time limit should be enabled or disabled
		 */
		wxCheckBox* timeLimitEnabled;

		/**
		 * Time limit label
		 */
		wxStaticText* timeLimitLabel;

		/**
		 * Time limit slider
		 */
		wxSlider* timeLimitValue;

		/**
		 * Name of the connected player
		 */
		wxStaticText* connectedPlayerLabel;

		/**
		 * The start game button
		 */
		wxButton* startGameButton;

		/**
		 * Sends the current game settings to connected clients
		 */
		void sendSettings();

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
		 * Cleans up the used sockets
		 */
		void close();

		/**
		 * Parses the data received from server socket
		 *
		 * @param sock The socket from which the data was received
		 */
		void parseData(wxSocketBase& sock);

		/**
		 * Event handler for spin ctrl changes
		 */
		void OnSpinCtrlChange(wxSpinEvent& WXUNUSED(e));

		/**
		 * Event handler for stones in one row
		 */
		void OnStonesInOneRowClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event for the time limit enabled checkbox
		 *
		 * @param e The event
		 */
		void OnTimeLimitEnabledClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for start game button
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
		 * Event handler for the time limit slider
		 *
		 * @param e The event
		 */
		void OnTimeLimitSlide(wxScrollEvent& WXUNUSED(e));

		/**
		 * Event handler for server events
		 *
		 * @param e The event
		 */
		void OnServerEvent(wxSocketEvent& e);

		/**
		 * Event handler for socket events
		 *
		 * @param e The event
		 */
		void OnSocketEvent(wxSocketEvent& e);

		/**
		 * Updates the time limit label
		 */
		void updateTimeLimitLabel();

		/**
		 * wxWidgets Event table
		 */
		DECLARE_EVENT_TABLE()
};

#endif /* CREATENETWORKGAMEWINDOW_H_ */
