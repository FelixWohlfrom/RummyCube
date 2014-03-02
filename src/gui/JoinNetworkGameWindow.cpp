/*
 * JoinNetworkGameWindow.cpp
 *
 *  Created on: 31.03.2012
 *      Author: Felix Wohlfrom
 */

#include "JoinNetworkGameWindow.h"

#include "../network/Network.h"

#include <wx/panel.h>
#include <wx/msgdlg.h>
#include <wx/gbsizer.h>

// The word wrap size of description in pixels
#define WRAP_SIZE 340

// The event table for wxwidgets
BEGIN_EVENT_TABLE(JoinNetworkGameWindow, wxWindow)
	EVT_BUTTON(ID_CONNECTTOSERVER, JoinNetworkGameWindow::OnConnectClick)
	EVT_BUTTON(wxID_OK, JoinNetworkGameWindow::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, JoinNetworkGameWindow::OnCancelClick)
	EVT_CLOSE(JoinNetworkGameWindow::OnClose)
	EVT_SOCKET(ID_SOCKET, JoinNetworkGameWindow::OnSocketEvent)
END_EVENT_TABLE()

JoinNetworkGameWindow::JoinNetworkGameWindow(wxWindow* parent) :
	wxDialog(parent, wxID_ANY, wxString(_("Join Network game"))),
	game(NULL), sock(NULL), connected(false), canStartGameRemote(false),
	canStartGameLocal(false), connectedPlayer()
{
	// Main sizer
	mainSizer = new wxBoxSizer(wxVERTICAL);

	// Sizer to keep settings all together
	wxBoxSizer* settingsSizer = new wxBoxSizer(wxVERTICAL);

	// Insertion of the ip address of the server
	wxFlexGridSizer* ipAddressSizer = new wxFlexGridSizer(1, 2, 2, 30);
	ipAddressSizer->AddGrowableCol(1, 1);
	wxStaticText* infoText = new wxStaticText(this, wxID_ANY, wxString(_("IP Address of the created game")) + _T(":"));
	infoText->SetToolTip(_("Insert here the ip address of the server you want to connect to"));
	infoText->Wrap(WRAP_SIZE);
	ipAddressSizer->Add(infoText);
	hostname = new wxTextCtrl(this, wxID_ANY, _T("127.0.0.1"), wxDefaultPosition, wxSize(150, -1));
	ipAddressSizer->Add(hostname, 0, wxALIGN_RIGHT);

	// Nickname insertion and connection button
	wxFlexGridSizer* nicknameSizer = new wxFlexGridSizer(2, 3, 3, 15);
	nicknameSizer->AddGrowableCol(0);
	nicknameSizer->AddGrowableCol(1);
	nicknameSizer->AddGrowableCol(2);
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Nickname")) + _T(":"));
	infoText->SetToolTip(_("Insert here your nickname so that your opponent knows against who he is playing"));
	nicknameSizer->Add(infoText);
	nickname = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxSize(150, -1));
	nicknameSizer->Add(nickname, 0, wxALIGN_RIGHT);
	connectButton = new wxButton(this, ID_CONNECTTOSERVER, _("&Connect"));
	nicknameSizer->Add(connectButton, 0, wxALIGN_RIGHT);

	// The sizer for the settings of the network game
	wxGridBagSizer* networkGameSettingsSizer = new wxGridBagSizer(15, 15);
	// Number of stones in first round
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Number of stones that can be taken in first round")) + _T(":"));
	infoText->SetToolTip(_("The number of stones that you can get in the first round"));
	infoText->Wrap(WRAP_SIZE);
	networkGameSettingsSizer->Add(infoText, wxGBPosition(0, 0));
	stonesInFirstRound = new wxStaticText(this, wxID_ANY, _T(""));
	stonesInFirstRound->SetToolTip(_("The number of stones that you can get in the first round"));
	networkGameSettingsSizer->Add(stonesInFirstRound, wxGBPosition(0, 1));
	// Sum of stone numbers on first time playing out
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Sum of stone numbers that you need to play first time out")) + _T(":"));
	infoText->SetToolTip(_("The sum that you need to play out first time"));
	infoText->Wrap(WRAP_SIZE);
	networkGameSettingsSizer->Add(infoText, wxGBPosition(1, 0));
	sumInFirstRound = new wxStaticText(this, wxID_ANY, _T(""));
	sumInFirstRound->SetToolTip(_("The sum that you need to play out first time"));
	networkGameSettingsSizer->Add(sumInFirstRound, wxGBPosition(1, 1));

	// Sizer if stones should be in one row at start
	wxFlexGridSizer* stonesInOneRowSizer = new wxFlexGridSizer(2, 2, 2, 15);
	stonesInOneRowSizer->AddGrowableCol(0, 1);
	stonesInOneRowSizer->AddGrowableCol(1);
	// If stones should be in one row at start
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Stones must be in one row")) + _T(":"));
	infoText->SetToolTip(_("If the sum that you need to play out first time has to be in one row"));
	stonesInOneRowSizer->Add(infoText);
	stonesInOneRow = new wxStaticText(this, wxID_ANY, _T(""));
	stonesInOneRow->SetToolTip(_("If the sum that you need to play out first time has to be in one row"));
	stonesInOneRowSizer->Add(stonesInOneRow, 0, wxALIGN_RIGHT);

	// Sizer for time limit
	wxFlexGridSizer* timeLimitSizer = new wxFlexGridSizer(1, 2, 2, 15);
	timeLimitSizer->AddGrowableCol(0, 1);
	timeLimitSizer->AddGrowableCol(1);
	// Time limit enabled and/or value
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Time limit")) + _T(":"));
	infoText->SetToolTip(_("The time limit for one round"));
	timeLimitSizer->Add(infoText);
	timeLimit = new wxStaticText(this, wxID_ANY, _T(""));
	timeLimit->SetToolTip(_("The time limit for one round"));
	timeLimitSizer->Add(timeLimit, 0, wxALIGN_RIGHT);

	// Put all into settings sizer
	settingsSizer->Add(ipAddressSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(nicknameSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(networkGameSettingsSizer, 0, wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(stonesInOneRowSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);
	settingsSizer->Add(timeLimitSizer, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);

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

	// Set the size depending on settings size
	stonesInFirstRound->SetLabel(_T("00")); // This is needed to correctly calculate window size
	this->SetSize(this->GetEffectiveMinSize());
	this->Layout();
	this->Center();
	stonesInFirstRound->SetLabel(_T(""));

	// Based on settingsSizer size add the information that the host did not start the game with correct size and centered
	wxStaticText* waitForOpponent = new wxStaticText(this, wxID_ANY, _("Waiting for your opponent to start game"));
	waitForOpponent->SetToolTip(_("Waiting for your opponent to start game"));
	waitPanelSizer->Add(waitForOpponent, 0, wxALIGN_CENTER | wxTOP, settingsSizer->GetSize().y / 2 - waitForOpponent->GetSize().y / 2);
	waitForOpponent->Hide();
}

JoinNetworkGameWindow::~JoinNetworkGameWindow()
{
	connectedPlayer.clear();
}

RummyCubeNetwork* JoinNetworkGameWindow::getGame()
{
	return game;
}

void JoinNetworkGameWindow::showSettingsPanel()
{
	mainSizer->Show((size_t)0);
	mainSizer->Hide((size_t)1);
}

void JoinNetworkGameWindow::showWaitingPanel()
{
	mainSizer->Hide((size_t)0);
	mainSizer->Show((size_t)1);
}

void JoinNetworkGameWindow::updateGui()
{
	if (canStartGameLocal)
	{
		showWaitingPanel();
	}
	else
	{
		showSettingsPanel();

		if (this->connected)
		{
			stonesInFirstRound->SetLabel(wxString::Format(_T("%d"), settings.stonesInFirstRound));
			sumInFirstRound->SetLabel(wxString::Format(_T("%d"), settings.sumInFirstRound));
			if (settings.stonesInOneRow)
			{
				stonesInOneRow->SetLabel(_("Yes"));
			}
			else
			{
				stonesInOneRow->SetLabel(_("No"));
			}
			if (settings.timeLimit == -1)
			{
				timeLimit->SetLabel(_("None"));
			}
			else
			{
				int min = settings.timeLimit / 60;
				int sec = settings.timeLimit % 60;
				timeLimit->SetLabel(wxString::Format(_("%d:%02d minutes"), min, sec));
			}
		}
		else
		{
			stonesInFirstRound->SetLabel(_T(""));
			sumInFirstRound->SetLabel(_T(""));
			stonesInOneRow->SetLabel(_T(""));
			timeLimit->SetLabel(_T(""));
		}
	}

	this->Layout();
}

void JoinNetworkGameWindow::startGame()
{
	if (canStartGameLocal && canStartGameRemote)
	{
		// Stop events from sockets
		sock->Notify(false);

		// Create the game
		game = new RummyCubeNetwork(settings, sock, nickname->GetValue(), connectedPlayer);

		this->EndModal(wxID_OK);
	}

	updateGui();
}

void JoinNetworkGameWindow::disconnect()
{
	connected = false;
	canStartGameLocal = false;
	canStartGameRemote = false;
	sock->Destroy();
	connectButton->Enable();
	hostname->Enable();
	nickname->Enable();
	startGameButton->Disable();
	updateGui();
}

void JoinNetworkGameWindow::parseData()
{
	// Read first line of message, is always a wxString
	wxString msg = Network::read(*sock);
	if (msg == _T("toManyPlayers"))
	{
		disconnect();
		wxMessageDialog(this, _("Currently there are to many players connected to the server.\nPlease use another one."), _("To many players"), wxOK | wxICON_INFORMATION).ShowModal();
	}
	else if (msg == _T("wrongVersion"))
	{
		disconnect();
		wxMessageDialog(this, _("You are using an older version for the game than the server. Please upgrade your game to the latest version."), _("Wrong version"), wxOK | wxICON_INFORMATION).ShowModal();
	}
	else if (msg == _T("connected"))
	{
		Network::write(*sock, _T("RummyCube 1"));		// Send information that we use the RummyCube protocol version 1
		Network::write(*sock, nickname->GetValue());	// Send nickname of player

		connected = true;
		connectButton->Disable();
		hostname->Disable();
		nickname->Disable();
	}
	else if (msg == _T("settings"))
	{
		// Receive settings
		sock->ReadMsg(&settings, sizeof(settings));
		startGameButton->Enable();
		canStartGameLocal = false;
	}
	else if (msg == _T("startGame"))
	{
		canStartGameRemote = true;
		startGame();
		return;
	}
	else if (msg == _T("dontStartGame"))
	{
		canStartGameRemote = false;
		return;
	}
	else if (msg == _T("playerNames"))
	{
		connectedPlayer.clear();

		int count;
		sock->Read(&count, sizeof(count));
		while (count > 0)
		{
			connectedPlayer.push_back(Network::read(*sock));
			count--;
		}
	}

	updateGui();
}

void JoinNetworkGameWindow::OnConnectClick(wxCommandEvent& WXUNUSED(e))
{
	if (nickname->GetValue() != _T(""))
	{
		sock = Network::createClientSocket(hostname->GetValue(), 3020);

		sock->WaitOnConnect(10);

		if (!sock->IsConnected())
		{
			disconnect();
			wxMessageDialog(this, _("Could not connect to other player.\nPlease make sure that the server is running, you have the correct address and the port is open in the firewall."), _("Error"), wxOK | wxICON_ERROR).ShowModal();
			return;
		}

		sock->SetEventHandler(*this, ID_SOCKET);
		sock->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
		sock->Notify(true);

		Network::write(*sock, _T("getStatus"));

		connectedPlayer.clear();
	}
	else
	{
		wxMessageDialog(this, _("Please enter a nickname before connecting"), _("Enter nickname"), wxOK | wxICON_INFORMATION).ShowModal();
	}
}

void JoinNetworkGameWindow::OnOkClick(wxCommandEvent& WXUNUSED(e))
{
#ifdef __WXGTK__
	if (!startGameButton->IsEnabled())
	{
		return;
	}
#endif

	Network::write(*sock, _T("startGame"));
	startGameButton->Disable();
	canStartGameLocal = true;

	startGame();
}

void JoinNetworkGameWindow::OnCancelClick(wxCommandEvent& WXUNUSED(e))
{
	if (canStartGameLocal)
	{
		canStartGameLocal = false;
		startGameButton->Enable();
		Network::write(*sock, _T("dontStartGame"));
		updateGui();
	}
	else
	{
		if (sock != NULL && sock->IsConnected())
		{
			// This fails, if the socket is not connected
			sock->Destroy();
		}
		this->EndModal(wxID_CANCEL);
	}
}

void JoinNetworkGameWindow::OnClose(wxCloseEvent& WXUNUSED(e))
{
	if (sock != NULL)
	{
		sock->Destroy();
	}
	this->EndModal(wxID_CANCEL);
}

void JoinNetworkGameWindow::OnSocketEvent(wxSocketEvent& e)
{
	switch (e.GetSocketEvent())
	{
		case wxSOCKET_LOST:
						disconnect();
						updateGui();
						break;

		case wxSOCKET_INPUT:
						parseData();
						break;

		default:
						break;
	}
}
