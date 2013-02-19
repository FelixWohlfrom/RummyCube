/*
 * CreateNetworkGameWindow.cpp
 *
 *  Created on: 10.03.2012
 *      Author: Felix Wohlfrom
 *
 */

#include "CreateNetworkGameWindow.h"

#include "../network/Network.h"
#include "../network/NetworkGameSettings.h"
#include "../game/RummyCubeNetwork.h"

#include <list>
#include <deque>

#include <wx/msgdlg.h>
#include <wx/socket.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

// The word wrap size of description in pixels
#define WRAP_SIZE 340

// The event table for wxwidgets
BEGIN_EVENT_TABLE(CreateNetworkGameWindow, wxWindow)
	EVT_SPINCTRL(ID_SPINCTRLCHANGE, CreateNetworkGameWindow::OnSpinCtrlChange)
	EVT_CHECKBOX(ID_STONESINONEROW, CreateNetworkGameWindow::OnStonesInOneRowClick)
	EVT_CHECKBOX(ID_TIMELIMITENABLED, CreateNetworkGameWindow::OnTimeLimitEnabledClick)
	EVT_BUTTON(wxID_OK, CreateNetworkGameWindow::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, CreateNetworkGameWindow::OnCancelClick)
	EVT_COMMAND_SCROLL(ID_TIMELIMITVALUE, CreateNetworkGameWindow::OnTimeLimitSlide)
	EVT_CLOSE(CreateNetworkGameWindow::OnClose)
	EVT_SOCKET(ID_SERVER, CreateNetworkGameWindow::OnServerEvent)
	EVT_SOCKET(ID_SOCKET, CreateNetworkGameWindow::OnSocketEvent)
END_EVENT_TABLE()

CreateNetworkGameWindow::CreateNetworkGameWindow(wxWindow* parent) :
	wxDialog(parent, wxID_ANY, wxString(_("Create Network game"))),
	game(NULL), connectedPlayer(), canStartGameRemote(false), canStartGameLocal(false)
{
	// Main sizer
	mainSizer = new wxBoxSizer(wxVERTICAL);

	// Sizer to keep settings all together
	wxBoxSizer* settingsSizer = new wxBoxSizer(wxVERTICAL);

	// The sizer for the upper half of the network game creation
	wxFlexGridSizer* ipAddressSizer = new wxFlexGridSizer(2);
	ipAddressSizer->AddGrowableCol(1, 1);
	// The ip(s) of the current host
	wxStaticText* infoText = new wxStaticText(this, wxID_ANY, wxString(_("Your IP-Address(es)")) + _T(":"));
	infoText->SetToolTip(_("Here you can see your IP-Address(es), pass them to the player who want to play with you"));
	infoText->Wrap(WRAP_SIZE);
	ipAddressSizer->Add(infoText);
	wxTextCtrl* ipAddresses = new wxTextCtrl(this, wxID_ANY, Network::getNetworkAddresses(), wxDefaultPosition, wxSize(150, -1), wxTE_READONLY);
	ipAddressSizer->Add(ipAddresses, 0, wxALIGN_RIGHT);

	// The sizer for the settings of the network game creation
	wxGridBagSizer* networkGameSettingsSizer = new wxGridBagSizer(15, 15);
	// Number of stones in first round
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Number of stones that can be taken in first round")) + _T(":"));
	infoText->SetToolTip(_("Insert here the number of stones that you can get in the first round"));
	infoText->Wrap(WRAP_SIZE);
	networkGameSettingsSizer->Add(infoText, wxGBPosition(0, 0));
	stonesInFirstRound = new wxSpinCtrl(this, ID_SPINCTRLCHANGE, _T("14"), wxDefaultPosition, wxSize(50, 21), wxSP_ARROW_KEYS, 5, 30, 14);
	stonesInFirstRound->SetToolTip(_("Insert here the number of stones that you can get in the first round"));
	networkGameSettingsSizer->Add(stonesInFirstRound, wxGBPosition(0, 1));
	// Sum of stone numbers on first time playing out
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Sum of stone numbers that you need to play first time out")) + _T(":"));
	infoText->SetToolTip(_("Insert the sum that you need to play out first time"));
	infoText->Wrap(WRAP_SIZE);
	networkGameSettingsSizer->Add(infoText, wxGBPosition(1, 0));
	sumInFirstRound = new wxSpinCtrl(this, ID_SPINCTRLCHANGE, _T("27"), wxDefaultPosition, wxSize(50, 21), wxSP_ARROW_KEYS, 3, 39, 27);
	sumInFirstRound->SetToolTip(_("Insert the sum that you need to play out first time"));
	networkGameSettingsSizer->Add(sumInFirstRound, wxGBPosition(1, 1));
	// Checkbox for stones in one row at start
	stonesInOneRow = new wxCheckBox(this, ID_STONESINONEROW, _("Stones must be in one row"));
	stonesInOneRow->SetToolTip(_("If this option is selected the sum that you need to play out first time has to be in one row"));
	stonesInOneRow->SetValue(true);
	networkGameSettingsSizer->Add(stonesInOneRow, wxGBPosition(2, 0), wxGBSpan(1, 2));

	// The time limit selectors
	wxGridBagSizer* timeLimitSizer = new wxGridBagSizer(15, 15);
	timeLimitEnabled = new wxCheckBox(this, ID_TIMELIMITENABLED, _("Time limit on/off"));
	timeLimitEnabled->SetToolTip(_("Click here to switch the time limit on and off"));
	timeLimitEnabled->SetValue(false);
	timeLimitSizer->Add(timeLimitEnabled, wxGBPosition(0, 0));
	timeLimitLabel = new wxStaticText(this, wxID_ANY, wxString(_T("1:00 ")) + _("minutes"));
	if (!timeLimitEnabled->GetValue())
	{
		timeLimitLabel->Disable();
	}
	timeLimitSizer->Add(timeLimitLabel, wxGBPosition(0, 1), wxDefaultSpan, wxALIGN_RIGHT);
	timeLimitValue = new wxSlider(this, ID_TIMELIMITVALUE, 60, 15, 300, wxDefaultPosition, wxSize(380, 41), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_TOP);
	#if wxCHECK_VERSION(2, 9, 0)
		timeLimitValue->SetTickFreq(10);
	#else
		timeLimitValue->SetTickFreq(10, 0);
	#endif
	if (!timeLimitEnabled->GetValue())
	{
		timeLimitValue->Disable();
	}
	timeLimitSizer->Add(timeLimitValue, wxGBPosition(1, 0), wxGBSpan(1, 2));
	updateTimeLimitLabel();

	// Information about connected player
	wxFlexGridSizer* connectedPlayerSizer = new wxFlexGridSizer(2);
	connectedPlayerSizer->AddGrowableCol(1, 1);
	wxStaticText* connectedPlayerInfo = new wxStaticText(this, wxID_ANY, wxString(_("Connected player")) + _T(":"));
	connectedPlayerInfo->SetToolTip(_("The name of the player which is connected to your game"));
	connectedPlayerInfo->Wrap(WRAP_SIZE);
	connectedPlayerSizer->Add(connectedPlayerInfo);
	connectedPlayerLabel = new wxStaticText(this, wxID_ANY, _T(""));
	connectedPlayerLabel->SetToolTip(_("The name of the player which is connected to your game"));
	connectedPlayerSizer->Add(connectedPlayerLabel);

	// Put all into settings sizer
	settingsSizer->Add(ipAddressSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(networkGameSettingsSizer, 0, wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(timeLimitSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(connectedPlayerSizer, 0, wxEXPAND | wxALL, 15);

	// The buttons
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	startGameButton = new wxButton(this, wxID_OK, _("&Start Game"));
	startGameButton->SetToolTip(_("Click here to start a network game"));
	startGameButton->SetDefault();
	startGameButton->Enable(false);
	buttonSizer->Add(startGameButton, 0, wxALL, 10);
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, _("&Cancel"));
	cancelButton->SetToolTip(_("Cancel the network game creation"));
	buttonSizer->Add(cancelButton, 0, wxALL, 10);

	// The sizer for the information shown if the host did not already start game
	wxBoxSizer* waitPanelSizer = new wxBoxSizer(wxVERTICAL);

	// Stick all together and make just settings visible
	mainSizer->Add(settingsSizer);
	mainSizer->Add(waitPanelSizer, 1, wxEXPAND);
	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
	this->SetSizer(mainSizer);
	showSettingsPanel();

	// Set the size
	this->SetSize(this->GetEffectiveMinSize());
	this->Layout();
	this->Center();

	// Based on settingsSizer size add the information that the host did not start the game with correct size and centered
	wxStaticText* waitForOpponent = new wxStaticText(this, wxID_ANY, _("Waiting for your opponent to start game"));
	waitForOpponent->SetToolTip(_("Waiting for your opponent to start game"));
	waitPanelSizer->Add(waitForOpponent, 0, wxALIGN_CENTER | wxTOP, settingsSizer->GetSize().y / 2 - waitForOpponent->GetSize().y / 2);
	waitForOpponent->Hide();

	// Create the server socket for network game
	server = Network::createServerSocket(3020);

	if (!server->IsOk())
	{
		wxMessageDialog(this, _("Server could not be created. Port 3020 is already in use!"), _("Error"), wxOK | wxICON_ERROR).ShowModal();
		server->Destroy();
		delete server;
		return;
	}

	server->SetEventHandler(*this, ID_SERVER);
	server->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_LOST_FLAG);
	server->Notify(true);
}

CreateNetworkGameWindow::~CreateNetworkGameWindow() {
	//delete server;
};

RummyCubeNetwork* CreateNetworkGameWindow::getGame()
{
	return game;
}

void CreateNetworkGameWindow::sendSettings()
{
	for (std::map<wxSocketBase*, wxString>::iterator elem = connectedPlayer.begin(); elem != connectedPlayer.end(); ++elem)
	{
		Network::write(*elem->first, _T("settings"));
		NetworkGameSettings settings;
		settings.stonesInFirstRound = stonesInFirstRound->GetValue();
		settings.sumInFirstRound = sumInFirstRound->GetValue();
		settings.stonesInOneRow = stonesInOneRow->IsChecked();
		if (timeLimitEnabled->IsChecked())
		{
			settings.timeLimit = timeLimitValue->GetValue();
		}
		else
		{
			settings.timeLimit = -1;
		}
		elem->first->WriteMsg(&settings, sizeof(settings));
	}

	// If we send settings, the remote player first needs to confirm the new settings
	canStartGameRemote = false;
}

void CreateNetworkGameWindow::showSettingsPanel()
{
	mainSizer->Show((size_t)0);
	mainSizer->Hide((size_t)1);
}

void CreateNetworkGameWindow::showWaitingPanel()
{
	mainSizer->Hide((size_t)0);
	mainSizer->Show((size_t)1);
}

void CreateNetworkGameWindow::updateGui()
{
	if (canStartGameLocal)
	{
		showWaitingPanel();
	}
	else
	{
		showSettingsPanel();

		if (connectedPlayer.size() == 0)
		{
			startGameButton->Disable();
		}
		else
		{
			startGameButton->Enable();
		}

		wxString playerNames = _T("");
		for (std::map<wxSocketBase*, wxString>::iterator elem = connectedPlayer.begin(); elem != connectedPlayer.end(); ++elem)
		{
			if (playerNames.length() > 0)
			{
				playerNames += _T(";");
			}

			playerNames += elem->second;
		}
		connectedPlayerLabel->SetLabel(playerNames);
	}

	this->Layout();
}

void CreateNetworkGameWindow::startGame()
{
	if (canStartGameLocal && canStartGameRemote)
	{
		// Game settings
		NetworkGameSettings settings;
		settings.stonesInFirstRound = stonesInFirstRound->GetValue();
		settings.sumInFirstRound = sumInFirstRound->GetValue();
		settings.stonesInOneRow = stonesInOneRow->IsChecked();
		if (timeLimitEnabled->IsChecked())
		{
			settings.timeLimit = timeLimitValue->GetValue();
		}
		else
		{
			settings.timeLimit = -1;
		}

		// Create game
		game = new RummyCubeNetwork(settings, connectedPlayer);

		this->EndModal(wxID_OK);
	}

	updateGui();
}

void CreateNetworkGameWindow::close()
{
	for (std::map<wxSocketBase*, wxString>::iterator elem = connectedPlayer.begin(); elem != connectedPlayer.end(); ++elem)
	{
		elem->first->Destroy();
	}
	server->Destroy();
	wxDELETE(server);
	this->EndModal(wxID_CANCEL);
}

void CreateNetworkGameWindow::parseData(wxSocketBase& sock)
{
	wxString msg = Network::read(sock);

	if (msg == _T("getStatus"))
	{
		if (connectedPlayer.size() == 1)
		{
			Network::write(sock, _T("connected"));
		}
		else
		{
			Network::write(sock, _T("toManyPlayers"));
		}
		return;
	}
	if (msg == _T("startGame"))
	{
		canStartGameRemote = true;
		startGame();
		return;
	}
	else if (msg == _T("dontStartGame"))
	{
		canStartGameRemote = false;
		canStartGameLocal = false;
		updateGui();
		return;
	}

	// Check for protocol and version
	else if (msg != _T("RummyCube 1"))
	{
		Network::write(sock, _T("wrongVersion"));
		return;
	}

	// Correct version, read player name
	sendSettings();
	connectedPlayer[&sock] = Network::read(sock);
	updateGui();
}

void CreateNetworkGameWindow::OnSpinCtrlChange(wxSpinEvent& WXUNUSED(e))
{
	this->sendSettings();
}

void CreateNetworkGameWindow::OnStonesInOneRowClick(wxCommandEvent& WXUNUSED(e))
{
	this->sendSettings();
}

void CreateNetworkGameWindow::OnTimeLimitEnabledClick(wxCommandEvent& WXUNUSED(e))
{
	if (timeLimitEnabled->IsChecked())
	{
		timeLimitValue->Enable();
		timeLimitLabel->Enable();
	}
	else
	{
		timeLimitValue->Disable();
		timeLimitLabel->Disable();
	}
	this->sendSettings();
}

void CreateNetworkGameWindow::OnOkClick(wxCommandEvent& WXUNUSED(e))
{
	for (std::map<wxSocketBase*, wxString>::iterator elem = connectedPlayer.begin(); elem != connectedPlayer.end(); ++elem)
	{
		// First send the names of the players
		Network::write(*elem->first, _T("playerNames"));

		int count = connectedPlayer.size() + 1;
		elem->first->Write(&count, sizeof(count));

		Network::write(*elem->first, _("Game creator")); // TODO Get game host name dynamically
		for (std::map<wxSocketBase*, wxString>::iterator names = connectedPlayer.begin(); names != connectedPlayer.end(); ++names)
		{
			Network::write(*elem->first, names->second);
		}

		Network::write(*elem->first, _T("startGame"));

		// Disable new incomming connections
		server->Notify(false);
	}
	startGameButton->Disable();
	canStartGameLocal = true;

	startGame();
}

void CreateNetworkGameWindow::OnCancelClick(wxCommandEvent& WXUNUSED(e))
{
	if (canStartGameLocal)
	{
		// Enable new incomming connections
		server->Notify(true);

		canStartGameLocal = false;
		startGameButton->Enable();
		for (std::map<wxSocketBase*, wxString>::iterator elem = connectedPlayer.begin(); elem != connectedPlayer.end(); ++elem)
		{
			Network::write(*elem->first, _T("dontStartGame"));
		}
		updateGui();
	}
	else
	{
		close();
	}
}

void CreateNetworkGameWindow::OnClose(wxCloseEvent& WXUNUSED(e))
{
	close();
}

void CreateNetworkGameWindow::OnTimeLimitSlide(wxScrollEvent& WXUNUSED(e))
{
	this->sendSettings();
	this->updateTimeLimitLabel();
}

void CreateNetworkGameWindow::OnServerEvent(wxSocketEvent& e)
{
	switch (e.GetSocketEvent())
	{
		case wxSOCKET_CONNECTION:
						{
							wxSocketBase* sock = ((wxSocketServer*)e.GetSocket())->Accept(true);
							connectedPlayer.insert(std::pair<wxSocketBase*, wxString>(sock, _T("")));
							sock->SetEventHandler(*this, ID_SOCKET);
							sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
							sock->Notify(true);
						}
						break;

		default:
						break;
	}
}

void CreateNetworkGameWindow::OnSocketEvent(wxSocketEvent& e)
{
	wxSocketBase* sock = e.GetSocket();

	switch (e.GetSocketEvent())
	{
		case wxSOCKET_LOST:
						connectedPlayer.erase(sock);
						canStartGameRemote = false;
						canStartGameLocal = false;
						sock->Destroy();
						updateGui();
						break;

		case wxSOCKET_INPUT:
						parseData(*sock);
						break;

		default:
						break;
	}
}

void CreateNetworkGameWindow::updateTimeLimitLabel()
{
	int min = timeLimitValue->GetValue() / 60;
	int sec = timeLimitValue->GetValue() % 60;
	timeLimitLabel->SetLabel(wxString::Format(_("%d:%02d minutes"), min, sec));
}
