/*
 * DummyApp.h
 *
 *  Created on: 27.12.2013
 *      Author: Felix Wohlfrom
 */

#ifndef MAIN_H_
#define DUMMYAPP_H_

#include <wx/app.h>
#include <wx/window.h>

/**
 * This class represents a dummy app window for testing purposes.
 */
class DummyApp: public wxApp
{
	public:
		/**
		 * Returns the main window of the dummy app.
		 *
		 * @return The main window of dummy app.
		 */
		wxWindow* getTestWindow();

		/**
		 * This method is called on closing of the application.
		 */
		int OnExit();

	private:
		/**
		 * This method is called on initialization of the application.
		 */
		bool OnInit();

		wxWindow* testWindow;
};

DECLARE_APP(DummyApp)

#endif /* MAIN_H_ */
