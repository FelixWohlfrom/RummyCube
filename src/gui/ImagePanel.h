/*
 * ImagePanel.h
 *
 *  Created on: 21.08.2011
 *      Author: Felix Wohlfrom
 */

#ifndef IMAGEPANEL_H_
#define IMAGEPANEL_H_

#include <wx/panel.h>
#include <wx/image.h>
#include <wx/bitmap.h>
#include <wx/sizer.h>

/**
 * A panel containing an image as background. Image resizes with panel.
 */
class ImagePanel : public wxPanel
{
	public:
		/**
		 * Constructor
		 *
		 * @param parent The parent window of the panel
		 * @param file The image file to load
		 * @param defaultImg The default image shown if the image file can not be loaded
		 * @param id An identifier for the panel. A value of -1 is taken to mean a default
		 * @param pos The panel position. A value of (-1, -1) indicates a default position, chosen by either the windowing system or wxWidgets, depending on platform
		 * @param size The panel size. A value of (-1, -1) indicates a default size, chosen by either the windowing system or wxWidgets, depending on platform
		 * @param style The window style @see wxPanel
		 * @param name Used to associate a name with the window, allowing the application user to set Motif resource values for individual dialog boxes.
		 */
		ImagePanel(wxWindow* parent,
				wxString file,
				wxBitmap defaultImg,
				wxWindowID id = wxID_ANY,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
	            long style = wxTAB_TRAVERSAL | wxNO_BORDER,
	            const wxString& name = wxPanelNameStr);

		/**
		 * Sets the image to the specified image
		 *
		 * @param file The path to the image file
		 */
		void setImage(wxString file);

		/**
		 * Returns the current image path.
		 * If default image is used, an emtpy string will be returned
		 *
		 * @return The current image path. "" if default image
		 */
		wxString getImagePath();

	private:
		/**
		 * The image that is shown in the panel in original size
		 */
		wxImage image;

		/**
		 * The resized image
		 */
		wxBitmap resized;

		/**
		 * With and height of the image
		 */
		int w, h;

		/**
		 * Default image if no file could be loaded
		 */
		wxBitmap defaultImage;

		/**
		 * Path to the image on disk
		 */
		wxString imagePath;

		/**
		 * Paints the panel
		 *
		 * @param e The paint event
		 */
		void OnPaint(wxPaintEvent& WXUNUSED(e));

		/**
		 * Dummy to avoid flickering
		 */
		void OnEraseBackground(wxEraseEvent& WXUNUSED(e));

		/**
		 * Resizes the panel image
		 *
		 * @param e The paint event
		 */
		void OnSize(wxSizeEvent& e);

    DECLARE_EVENT_TABLE()
};

#endif /* IMAGEPANEL_H_ */
