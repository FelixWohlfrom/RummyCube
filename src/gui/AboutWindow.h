/*
 * AboutWindow.h
 *
 *  Created on: 31.01.2012
 *      Author: Felix Wohlfrom
 */

#ifndef ABOUTWINDOW_H_
#define ABOUTWINDOW_H_

#include <wx/dialog.h>

class AboutWindow : public wxDialog
{
	public:
		/**
		 * Constructor. Initializes the window elements
		 *
		 * @param parent The parent window
		 */
		AboutWindow(wxWindow* parent);

		/**
		 * Destructor. Dummy to avoid warnings
		 */
		virtual ~AboutWindow();

	private:
		/**
		 * Returns the compiler used for compilation
		 *
		 * @return The compiler name
		 */
		wxString getCompiler();

		/**
		 * Returns the compiler flags used for compilation
		 *
		 * @return The compiler flags if available
		 */
		wxString getCompilerFlags();

		/**
		 * Event handler for ok button
		 *
		 * @param e The event
		 */
		void OnOkClick(wxCommandEvent& WXUNUSED(e));

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

#endif /* ABOUTWINDOW_H_ */
