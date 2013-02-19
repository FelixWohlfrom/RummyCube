/*
 * ImagePanel.cpp
 *
 *  Created on: 21.08.2011
 *      Author: Felix Wohlfrom
 */

#include "ImagePanel.h"
#include <wx/dcbuffer.h>

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
	EVT_ERASE_BACKGROUND(ImagePanel::OnEraseBackground)
	EVT_SIZE(ImagePanel::OnSize)
END_EVENT_TABLE()

ImagePanel::ImagePanel(wxWindow* parent, wxString file, wxBitmap defaultImg, wxWindowID id,
						const wxPoint& pos, const wxSize& size, long style, const wxString& name):
	wxPanel(parent, id, pos, size, style, name), w(-1), h(-1), defaultImage(defaultImg), imagePath(file)
{
	if (file == _T("") || !image.LoadFile(file))
	{
		image = defaultImage.ConvertToImage();
	}
}

void ImagePanel::setImage(wxString file)
{
	imagePath = file;

	if (file != _T(""))
	{
		wxImage oldImage;
		oldImage = image;

		if (!image.LoadFile(file))
		{
			image = oldImage;
		}
	}
	else
	{
		image = defaultImage.ConvertToImage();
	}

	w = -1;
	h = -1;
	Refresh();
}

wxString ImagePanel::getImagePath()
{
	return imagePath;
}

void ImagePanel::OnPaint(wxPaintEvent& WXUNUSED(e))
{
	wxBufferedPaintDC dc(this);

	int neww, newh;
	this->GetSize(&neww, &newh);

	if (neww != w || newh != h)
	{
		resized = wxBitmap(image.Scale(neww, newh));
		dc.DrawBitmap(resized, 0, 0, true);
	}
	else
	{
		dc.DrawBitmap(resized, 0, 0, true);
	}

	w = neww;
	h = newh;
}

// Empty dummy to avoid flickering
void ImagePanel::OnEraseBackground(wxEraseEvent& WXUNUSED(e)) {}

void ImagePanel::OnSize(wxSizeEvent& e) {
    Refresh();
    e.Skip();
}
