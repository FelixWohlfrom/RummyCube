/*
 * PropertiesWindow.cpp
 *
 *  Created on: 22.08.2011
 *      Author: Felix Wohlfrom
 *
 */

#include "PropertiesWindow.h"

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/gbsizer.h>
#include <wx/stattext.h>

#include "../game/Settings.h"

// The word wrap size of description in pixels
#define WRAP_SIZE 340

#ifndef _WIN32
	// On win systems these files are included using resources/msw/RummyCube.rc
	#include "resources/gameboard.xpm"
	#include "resources/holder.xpm"
#endif

// The event table for wxwidgets
BEGIN_EVENT_TABLE(PropertiesWindow, wxWindow)
	EVT_CHECKBOX(ID_TIMELIMITENABLED, PropertiesWindow::OnTimeLimitEnabledClick)
	EVT_BUTTON(wxID_OK, PropertiesWindow::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, PropertiesWindow::OnCancelClick)
	EVT_BUTTON(wxID_RESET, PropertiesWindow::OnResetClick)
	EVT_BUTTON(wxID_CLEAR, PropertiesWindow::OnStandardClick)
	EVT_COMMAND_SCROLL(ID_TIMELIMITVALUE, PropertiesWindow::OnTimeLimitSlide)
	EVT_CLOSE(PropertiesWindow::OnClose)
END_EVENT_TABLE()

PropertiesWindow::PropertiesWindow(wxWindow* parent) :
	wxDialog(parent, wxID_ANY, wxString(_("Settings")))
{
	// Initialize settings
	Settings* settings = Settings::getInstance();

	// Main sizer
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// The sizer for the upper half of the settings
	wxGridBagSizer* topSizer = new wxGridBagSizer(15, 15);
	// Number of stones in first round
	wxStaticText* infoText = new wxStaticText(this, wxID_ANY, wxString(_("Number of stones that can be taken in first round")) + _T(": *"));
	infoText->SetToolTip(_("Insert here the number of Stones that you can get in the first round"));
	infoText->Wrap(WRAP_SIZE);
	topSizer->Add(infoText, wxGBPosition(0, 0));
	stonesInFirstRound = new wxSpinCtrl(this, wxID_ANY, settings->Read(_T("General/StonesAtBeginning"), _T("14")), wxDefaultPosition, wxSize(50, 21), wxSP_ARROW_KEYS, 5, 30, 14);
	stonesInFirstRound->SetToolTip(_("Insert here the number of Stones that you can get in the first round"));
	topSizer->Add(stonesInFirstRound, wxGBPosition(0, 1));
	// Sum of stone numbers on first time playing out
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Sum of stone numbers that you need to play first time out")) + _T(": *"));
	infoText->SetToolTip(_("Insert the sum that you need to play out first time"));
	infoText->Wrap(WRAP_SIZE);
	topSizer->Add(infoText, wxGBPosition(1, 0));
	sumInFirstRound = new wxSpinCtrl(this, wxID_ANY, settings->Read(_T("General/SumAtBeginning"), _T("27")), wxDefaultPosition, wxSize(50, 21), wxSP_ARROW_KEYS, 3, 39, 27);
	sumInFirstRound->SetToolTip(_("Insert the sum that you need to play out first time"));
	topSizer->Add(sumInFirstRound, wxGBPosition(1, 1));
	// Checkbox for stones in one row at start
	stonesInOneRow = new wxCheckBox(this, wxID_ANY, wxString(_("Stones must be in one row")) + _T(" *"));
	stonesInOneRow->SetToolTip(_("If this option is selected the sum that you need to play out first time has to be in one row"));
	bool tmp;
	settings->Read(_T("General/StonesInOneRow"), &tmp, true);
	stonesInOneRow->SetValue(tmp);
	topSizer->Add(stonesInOneRow, wxGBPosition(2, 0), wxGBSpan(1, 2));
	// Number of pc players
	infoText = new wxStaticText(this, wxID_ANY, wxString(_("Number of PC-Players")) + _T(": *"));
	infoText->SetToolTip(_("Choose the number of computer players here"));
	topSizer->Add(infoText, wxGBPosition(3, 0));
	wxArrayString aiPlayerCountTexts;
	aiPlayerCountTexts.Add(_T("1"));
	aiPlayerCountTexts.Add(_T("2"));
	aiPlayerCountTexts.Add(_T("3"));
	aiPlayerCountTexts.Add(_T("4"));
	aiPlayerCount = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxSize(50, 21), aiPlayerCountTexts);
	aiPlayerCount->SetToolTip(_("Choose the number of computer players here"));
	aiPlayerCount->SetSelection(settings->Read(_T("General/AIPlayers"), 1) - 1);
	topSizer->Add(aiPlayerCount, wxGBPosition(3, 1));

	// The time limit selectors
	wxGridBagSizer* timeLimitSizer = new wxGridBagSizer(15, 15);
	timeLimitEnabled = new wxCheckBox(this, ID_TIMELIMITENABLED, wxString(_("Time limit on/off")) + _T(" *"));
	timeLimitEnabled->SetToolTip(_("Click here to switch the time limit on and off"));
	settings->Read(_T("General/TimeLimit"), &tmp, false);
	timeLimitEnabled->SetValue(tmp);
	timeLimitSizer->Add(timeLimitEnabled, wxGBPosition(0, 0));
	timeLimitLabel = new wxStaticText(this, wxID_ANY, wxString(_T("1:00 ")) + _("minutes"));
	if (!timeLimitEnabled->GetValue())
	{
		timeLimitLabel->Disable();
	}
	timeLimitSizer->Add(timeLimitLabel, wxGBPosition(0, 1), wxDefaultSpan, wxALIGN_RIGHT);
	timeLimitValue = new wxSlider(this, ID_TIMELIMITVALUE, settings->Read(_T("General/TimeLimitTime"), 60), 15, 300, wxDefaultPosition, wxSize(380, 41), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_TOP);
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

	// The background images
	wxGridBagSizer* imageSizer = new wxGridBagSizer(15, 15);
	imageSizer->Add(new wxStaticText(this, wxID_ANY, wxString(_("Holder")) + _T(":")), wxGBPosition(0, 0));
	imageSizer->Add(new wxStaticText(this, wxID_ANY, wxString(_("Table")) + _T(":")), wxGBPosition(0, 2));
	holderPanel = new ImagePanel(this, settings->Read(_T("General/HolderPath"), _T("")), wxBITMAP(holder));
	holderPanel->SetMinSize(wxSize(140, 40));
	holderPanel->SetToolTip(_("Click here to choose another background picture for the holder"));
	holderPanel->Connect(wxEVT_LEFT_UP, wxCommandEventHandler(PropertiesWindow::OnImageClick), NULL, this);
	imageSizer->Add(holderPanel, wxGBPosition(1, 0), wxDefaultSpan, wxEXPAND);
	gameboardPanel = new ImagePanel(this, settings->Read(_T("General/GameboardPath"), _T("")), wxBITMAP(gameboard));
	gameboardPanel->SetMinSize(wxSize(140, 40));
	gameboardPanel->SetToolTip(_("Click here to choose another background picture for the gameboard"));
	gameboardPanel->Connect(wxEVT_LEFT_UP, wxCommandEventHandler(PropertiesWindow::OnImageClick), NULL, this);
	imageSizer->Add(gameboardPanel,  wxGBPosition(1, 2), wxDefaultSpan, wxEXPAND);
	wxButton* standardButton = new wxButton(this, wxID_CLEAR, _("Standard"));
	standardButton->SetSize(40, 80);
	standardButton->SetToolTip(_("Click here to use the default pictures"));
	imageSizer->Add(standardButton, wxGBPosition(1, 1));

	// The buttons
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* resetButton = new wxButton(this, wxID_RESET, _("&Reset"));
	resetButton->SetToolTip(_("Reset all settings"));
	buttonSizer->Add(resetButton, 0, wxALL, 10);
	wxButton* okButton = new wxButton(this, wxID_OK, _("&Ok"));
	okButton->SetToolTip(_("Save settings and close this window"));
	okButton->SetDefault();
	buttonSizer->Add(okButton, 0, wxALL, 10);
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, _("&Cancel"));
	cancelButton->SetToolTip(_("Cancel changes and close this window"));
	buttonSizer->Add(cancelButton, 0, wxALL, 10);

	// Stick all together
	mainSizer->Add(topSizer, 0, wxTOP | wxLEFT | wxRIGHT, 15);
	mainSizer->Add(timeLimitSizer, 0, wxTOP | wxLEFT | wxRIGHT, 15);
	infoText = new wxStaticText(this, wxID_ANY, wxString(_T("* ")) + _("These Settings are used the next time you start a new game"));
	infoText->Wrap(this->GetSize().x - 30);
	mainSizer->Add(infoText, 0, wxLEFT | wxRIGHT, 15);
	mainSizer->Add(imageSizer, 0, wxTOP | wxLEFT | wxRIGHT, 15);
	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
	this->SetSizer(mainSizer);

	// Set the size
	this->SetSize(this->GetEffectiveMinSize());
	this->Layout();
	this->Center();
}

PropertiesWindow::~PropertiesWindow() {};

void PropertiesWindow::OnTimeLimitEnabledClick(wxCommandEvent& WXUNUSED(e))
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
}

void PropertiesWindow::OnImageClick(wxCommandEvent& e)
{
	wxFileDialog dialog(this, _("Please select image"), _T(""), _T(""),
			_("All Image Files (*.bmp; *.gif; *.png; *.jpg; *.jpeg; *.pcx; *.pnm; *.tiff; *.tga; *.iff; *.xpm)|*.bmp; *.gif; *.png; *.jpg; *.jpeg; *.pcx; *.pnm; *.tiff; *.tga; *.iff; *.xpm"),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR | wxFD_PREVIEW);

	if (dialog.ShowModal() == wxID_OK)
	{
		((ImagePanel*)(e.GetEventObject()))->setImage(dialog.GetPath());
	}
}

void PropertiesWindow::OnOkClick(wxCommandEvent& WXUNUSED(e))
{
	Settings* settings = Settings::getInstance();

	settings->Write(_T("General/StonesAtBeginning"), stonesInFirstRound->GetValue());
	settings->Write(_T("General/SumAtBeginning"), sumInFirstRound->GetValue());
	settings->Write(_T("General/StonesInOneRow"), stonesInOneRow->GetValue());
	settings->Write(_T("General/AIPLayers"), aiPlayerCount->GetSelection() + 1);
	settings->Write(_T("General/TimeLimit"), timeLimitEnabled->GetValue());
	settings->Write(_T("General/TimeLimitTime"), timeLimitValue->GetValue());
	settings->Write(_T("General/HolderPath"), holderPanel->getImagePath());
	settings->Write(_T("General/GameboardPath"), gameboardPanel->getImagePath());
	settings->Flush();

	this->EndModal(wxID_OK);
}

void PropertiesWindow::OnCancelClick(wxCommandEvent& WXUNUSED(e))
{
	this->EndModal(wxID_CANCEL);
}

void PropertiesWindow::OnClose(wxCloseEvent& WXUNUSED(e))
{
	this->EndModal(wxID_CANCEL);
}

void PropertiesWindow::OnResetClick(wxCommandEvent& WXUNUSED(e))
{
	wxMessageDialog dialog(this, _("Do you really want to reset the settings?"), _("RummyCube"), wxYES_NO | wxICON_ERROR);

	if (dialog.ShowModal() == wxID_YES)
	{
		stonesInFirstRound->SetValue(_T("14"));
		sumInFirstRound->SetValue(_T("27"));
		stonesInOneRow->SetValue(true);
		aiPlayerCount->SetSelection(0);
		timeLimitEnabled->SetValue(false);
		timeLimitValue->SetValue(60);
		holderPanel->setImage(_T(""));
		gameboardPanel->setImage(_T(""));
	}
}

void PropertiesWindow::OnStandardClick(wxCommandEvent& WXUNUSED(e))
{
	wxMessageDialog dialog(this, _("Do you really want to use the default pictures?"), _("RummyCube"), wxYES_NO | wxICON_ERROR);

	if (dialog.ShowModal() == wxID_YES)
	{
		holderPanel->setImage(_T(""));
		gameboardPanel->setImage(_T(""));
	}
}

void PropertiesWindow::OnTimeLimitSlide(wxScrollEvent& WXUNUSED(e))
{
	updateTimeLimitLabel();
}

void PropertiesWindow::updateTimeLimitLabel()
{
	int min = timeLimitValue->GetValue() / 60;
	int sec = timeLimitValue->GetValue() % 60;
	timeLimitLabel->SetLabel(wxString::Format(_("%d:%02d minutes"), min, sec));
}
