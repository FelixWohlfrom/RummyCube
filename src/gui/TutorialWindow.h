/*
 * TutorialWindow.h
 *
 *  Created on: 31.01.2012
 *      Author: Felix Wohlfrom
 */

#ifndef TUTORIALWINDOW_H_
#define TUTORIALWINDOW_H_

#include <wx/dialog.h>
#include <wx/radiobox.h>

class TutorialWindow : public wxDialog
{
	public:
		/**
		 * Constructor. Initializes the window elements
		 *
		 * @param parent The parent window
		 */
		TutorialWindow(wxWindow* parent);

		/**
		 * Destructor. Dummy to avoid warnings
		 */
		virtual ~TutorialWindow();

	private:
		/**
		 * The selector for the tutorials
		 */
		wxRadioBox* tutorialSelection;

		/**
		 * Event handler for "start tutorial" button
		 *
		 * @param e The event
		 */
		void OnOkClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for "cancel" button
		 *
		 * @param e The event
		 */
		void OnCancelClick(wxCommandEvent& WXUNUSED(e));

		/**
		 * Event handler for window closing
		 *
		 * @param e The event
		 */
		void OnClose(wxCloseEvent& WXUNUSED(e));

		/**
		 * wxWidgets Event table
		 */
		DECLARE_EVENT_TABLE()
};

#endif /* TUTORIALWINDOW_H_ */
