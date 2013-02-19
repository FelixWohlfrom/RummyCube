/*
 * AboutWindow.cpp
 *
 *  Created on: 31.01.2012
 *      Author: Felix Wohlfrom
 */

#include "AboutWindow.h"

#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/version.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>

#include "ImagePanel.h"
#include "Version.h"

#ifndef _WIN32
	// On win systems these files are included using resources/msw/RummyCube.rc
	#include "resources/icon.xpm"
#endif

// The event table for wxwidgets
BEGIN_EVENT_TABLE(AboutWindow, wxWindow)
	EVT_BUTTON(wxID_OK, AboutWindow::OnOkClick)
	EVT_CLOSE(AboutWindow::OnClose)
END_EVENT_TABLE()

AboutWindow::AboutWindow(wxWindow* parent) :
	wxDialog(parent, wxID_ANY, wxString(_("About RummyCube")))
{
	// Initialize the main window parts
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	//this->SetBackgroundColour(wxColor(0x00, 0x80, 0x00));
	//this->SetForegroundColour(wxColor(0x00, 0xff, 0x00));

	// The logo, version, copyright and url
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	ImagePanel* logo = new ImagePanel(this, _T(""), wxICON(appicon), wxID_ANY, wxPoint(0, 0), wxSize(48, 48));
	topSizer->Add(logo, 0, wxALL | wxCENTER, 10);
	wxBoxSizer* versionInfoSizer = new wxBoxSizer(wxVERTICAL);
	wxStaticText* gameName = new wxStaticText(this, wxID_ANY, _T("RummyCube"));
	gameName->SetFont(wxFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	versionInfoSizer->Add(gameName);
	wxString version = wxString::Format(_T("Version %d.%d"), VERSION_MAJOR, VERSION_MINOR);
	#ifdef VERSION_BETA
		version << _T("beta");
	#endif
	#ifdef _DEBUG
		version << _T(" Debug");
	#endif
	wxStaticText* versionString = new wxStaticText(this, wxID_ANY, version);
	versionString->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	versionInfoSizer->Add(versionString);
	versionInfoSizer->Add(new wxStaticText(this, wxID_ANY, _T("Copyright (C) 2013 Felix Wohlfrom")));
	topSizer->Add(versionInfoSizer, 0, wxTOP | wxBOTTOM, 5);
	mainSizer->Add(topSizer, 0, wxALL, 10);

	// Information
	mainSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, _T("http://www.RummyCube.net.ms"), _T("http://www.RummyCube.net.ms")), 0, wxLEFT | wxRIGHT, 10);
	wxFlexGridSizer* licenseSizer = new wxFlexGridSizer(2, 3, 0);
	licenseSizer->Add(new wxStaticText(this, wxID_ANY, _("This program is licensed under ")));
	licenseSizer->Add(new wxHyperlinkCtrl(this, wxID_ANY, _T("CC BY"), _T("http://creativecommons.org/licenses/by/3.0/")));
	mainSizer->Add(licenseSizer, 0, wxTOP | wxLEFT | wxRIGHT, 10);

	// The build information
	wxStaticBoxSizer* buildInformationBorder = new wxStaticBoxSizer(wxVERTICAL, this, _("Build information"));
	wxFlexGridSizer* buildInformation = new wxFlexGridSizer(2, 3, 5);
	buildInformation->Add(new wxStaticText(this, wxID_ANY, wxString(_("Compiler")) + _T(": ")));
	buildInformation->Add(new wxStaticText(this, wxID_ANY, getCompiler()));
	buildInformation->Add(new wxStaticText(this, wxID_ANY, wxString(_("Compiler flags")) + _T(": ")));
	wxStaticText* flags = new wxStaticText(this, wxID_ANY, getCompilerFlags());
	flags->Wrap(200);
	buildInformation->Add(flags);
	buildInformation->Add(new wxStaticText(this, wxID_ANY, wxString(_("wxWidgets Version")) + _T(": ")));
	buildInformation->Add(new wxStaticText(this, wxID_ANY, wxString::Format(_T("%d.%d.%d"), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER)));
	buildInformationBorder->Add(buildInformation);
	mainSizer->Add(buildInformationBorder, 0, wxALL | wxGROW, 10);

	// Add ok button
	wxButton* okButton = new wxButton(this, wxID_OK, _("&Ok"));
	okButton->SetDefault();
	mainSizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

	// Update layout and position
	this->SetSizer(mainSizer);
	// These three lines need to be in this order to make sure, the border of the buildInformation is shown correctly
	this->SetSize(this->GetEffectiveMinSize());
	//this->SetSize(mainSizer->CalcMin());
	buildInformationBorder->SetMinSize(buildInformation->CalcMin());
	this->Layout();
	this->Center();
}

AboutWindow::~AboutWindow() {};

wxString AboutWindow::getCompiler()
{
	#ifdef __GNUG__
		wxString compiler = wxString::Format(_T("GCC %d.%d.%d"), __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
		#ifdef __x86_64
			compiler << _T(" (x64)");
		#endif
		return compiler;
	#elif defined _MSC_VER
		#if _MSC_VER >= 1600
			wxString compiler = _T("Visual Studio 2010");
		#else
			wxString compiler = wxString::Format(_T("Visual C++ (%d)"), _MSC_VER);
		#endif
		#ifdef _WIN64
			compiler << _T(" (x64)");
		#endif
		return compiler;
	#else
		return _T("Unknown compiler");
	#endif
}

wxString AboutWindow::getCompilerFlags()
{
	#ifdef _DEBUG
		#ifdef USED_CXXFLAGS_DEBUG
			return wxString(USED_CXXFLAGS_DEBUG, wxConvLocal);
		#else
			return _T("");
		#endif
	#elif defined USED_CXXFLAGS
		return wxString(USED_CXXFLAGS, wxConvLocal);
	#else
		return _T("");
	#endif
}

void AboutWindow::OnOkClick(wxCommandEvent& WXUNUSED(e))
{
	this->EndModal(wxID_OK);
}

void AboutWindow::OnClose(wxCloseEvent& WXUNUSED(e))
{
	this->EndModal(wxID_OK);
}
