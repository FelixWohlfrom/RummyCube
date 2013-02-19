/*
 * TutorialWindow.cpp
 *
 *  Created on: 05.01.2013
 *      Author: Felix Wohlfrom
 */

#include "TutorialWindow.h"

#include "../game/Settings.h"

#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

// The event table for wxwidgets
BEGIN_EVENT_TABLE(TutorialWindow, wxWindow)
	EVT_BUTTON(wxID_OK, TutorialWindow::OnOkClick)
	EVT_BUTTON(wxID_CANCEL, TutorialWindow::OnCancelClick)
	EVT_CLOSE(TutorialWindow::OnClose)
END_EVENT_TABLE()

TutorialWindow::TutorialWindow(wxWindow* parent) :
	wxDialog(parent, wxID_ANY, wxString(_("Tutorial")))
{
	// Initialize the main window parts
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString tutorials;
	tutorials.Add(_("Tutorial 1: Game rules"));
	tutorials.Add(_("Tutorial 2: Select a stone"));
	tutorials.Add(_("Tutorial 3: Some other stuff"));
	tutorialSelection = new wxRadioBox(this, wxID_ANY, _("Select tutorial"), wxDefaultPosition, wxDefaultSize, tutorials, 0, wxRA_SPECIFY_ROWS);

	int lastTutorial;
	Settings::getInstance()->Read(_T("LastPlayedTutorial"), &lastTutorial, 0);
	tutorialSelection->SetSelection(lastTutorial);

	// Add buttons
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* okButton = new wxButton(this, wxID_OK, _("&Start Tutorial"));
	okButton->SetToolTip(_("Start the selected tutorial"));
	okButton->SetDefault();
	buttonSizer->Add(okButton, 0, wxALL, 10);
	wxButton* cancelButton = new wxButton(this, wxID_CANCEL, _("&Cancel"));
	cancelButton->SetToolTip(_("Cancel tutorial selection"));
	buttonSizer->Add(cancelButton, 0, wxALL, 10);

	// Stick all together
	mainSizer->Add(tutorialSelection, 1, wxALL, 10);
	mainSizer->Add(buttonSizer, 0, wxALIGN_RIGHT);
	this->SetSizer(mainSizer);

	// Set the size
	this->SetSize(this->GetEffectiveMinSize());
	this->Layout();
	this->Center();
}

TutorialWindow::~TutorialWindow() {};

void TutorialWindow::OnOkClick(wxCommandEvent& WXUNUSED(e))
{
	Settings::getInstance()->Write(_T("LastPlayedTutorial"), tutorialSelection->GetSelection());

	this->EndModal(wxID_OK);
}

void TutorialWindow::OnCancelClick(wxCommandEvent& WXUNUSED(e))
{
	this->EndModal(wxID_CANCEL);
}

void TutorialWindow::OnClose(wxCloseEvent& WXUNUSED(e))
{
	this->EndModal(wxID_CANCEL);
}
