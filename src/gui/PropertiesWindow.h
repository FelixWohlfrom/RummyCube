/*
 * PropertiesWindow.h
 *
 *  Created on: 22.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef PROPERTIESWINDOW_H_
#define PROPERTIESWINDOW_H_

#include <wx/dialog.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/spinctrl.h>

#include "ImagePanel.h"

/**
 * The properties window
 */
class PropertiesWindow : public wxDialog
{
	public:
		/**
		 * Constructor. Initializes the window elements
		 *
		 * @param parent The parent window
		 */
		PropertiesWindow(wxWindow* parent);

		/**
		 * Destructor. Dummy to avoid warnings
		 */
		virtual ~PropertiesWindow();

	private:
		/**
		 * IDs for wxwidgets
		 */
		enum
		{
			ID_TIMELIMITENABLED = 1,
			ID_TIMELIMITVALUE
		};

		/**
		 * Selector for stones to take in first round
		 */
		wxSpinCtrl* stonesInFirstRound;

		/**
		 * Selector for sum that has to be played out first time by the player
		 */
		wxSpinCtrl* sumInFirstRound;

		/**
		 * If the stone sum has to be in one row
		 */
		wxCheckBox* stonesInOneRow;

		/**
		 * The count of the ai players
		 */
		wxChoice* aiPlayerCount;

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
		 * The panel for the holder image
		 */
		ImagePanel* holderPanel;

		/**
		 * The panel for the gameboard image
		 */
		ImagePanel* gameboardPanel;

		/**
		 * Event for the time limit enabled checkbox
		 *
		 * @param e The event
		 */
		void OnTimeLimitEnabledClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for background images
		 *
		 * @param e The event
		 */
		void OnImageClick(wxCommandEvent& e);

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
		 * Event handler for reset button click
		 *
		 * @param e The event
		 */
		void OnResetClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for standard button click
		 *
		 * @param e the event
		 */
		void OnStandardClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for the time limit slider
		 *
		 * @param e The event
		 */
		void OnTimeLimitSlide(wxScrollEvent& WXUNUSED(e));

		/**
		 * Updates the time limit label
		 */
		void updateTimeLimitLabel();

		/**
		 * wxWidgets Event table
		 */
		DECLARE_EVENT_TABLE()
};

#endif /* PROPERTIESWINDOW_H_ */
