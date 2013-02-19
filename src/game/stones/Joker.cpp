/*
 * Joker.cpp
 *
 *  Created on: 04.01.2012
 *      Author: Felix Wohlfrom
 */

#include "Joker.h"

#include <wx/icon.h>
#include <wx/dcbuffer.h>

#ifdef _WIN32
	#include <wx/image.h>
#endif

#ifndef _WIN32
	#include "../gui/resources/joker.xpm"
#endif

/****************************************/
/* GENERAL STUFF, PLATTFORM INDEPENDEND */
/****************************************/
// Initialize static variables //
wxBitmap Joker::jokerIcon;

// Constructors //
Joker::Joker() :
	Gamestone(), oldColor(UNKNOWN), oldNumber(-1)
{}

Joker::Joker(wxWindow* parent, StoneManager* stoneManager, bool isFirst) :
	Gamestone(parent, stoneManager, isFirst, UNKNOWN, -1), oldColor(UNKNOWN), oldNumber(-1)
{
	jokerIcon = wxBitmap(wxBITMAP(joker));

	// Add alpha channel on windows
	#ifdef _WIN32
		wxImage image = jokerIcon.ConvertToImage();
		image.SetMaskColour(0xff, 0x00, 0xff);
		jokerIcon = wxBitmap(image);
	#endif
}

// Getters and setters //
bool Joker::isJoker() const
{
	return true;
}

Gamestone::StoneColor Joker::getColor()
{
	this->update();
	return color;
}

int Joker::getNumber()
{
	this->update();
	return number;
}

void Joker::update()
{
	this->color = UNKNOWN;
	this->number = -1;

	// If we are left of a 1 or right of a 13, the number can be set
	if (this->next != NULL && !this->next->isJoker() && this->next->getNumber() == 1)
	{
		this->number = 1;
	}
	if (this->prev != NULL && !this->prev->isJoker() && this->prev->getNumber() == 13)
	{
		this->number = 13;
	}

	// The two stones which should be checked for same stone color or number
	Gamestone* firstStone(NULL);
	Gamestone* secondStone(NULL);

	// The difference between first stone and current joker
	int diff(0);

	// First try the stones before
	if (this->prev != NULL)
	{
		firstStone = this->prev;
		diff = -1;

		if (firstStone->isJoker())
		{
			firstStone = firstStone->prev;
			diff = -2;
		}

		if (firstStone != NULL)
		{
			// The second stone can now be either the first non-joker before the first stone ...
			secondStone = firstStone->prev;
			if (secondStone != NULL && secondStone->isJoker())
			{
				secondStone = secondStone->prev;
			}

			// ... or the first non-joker after the current stone
			if (secondStone == NULL)
			{
				secondStone = this->next;
				if (secondStone != NULL && secondStone->isJoker())
				{
					secondStone = secondStone->next;
				}
			}
		}
	}
	// Now the same stuff with stones after
	else if (this->next != NULL)
	{
		firstStone = this->next;
		diff = 1;
		if (firstStone->isJoker())
		{
			firstStone = firstStone->next;
			diff = 2;
		}

		if (firstStone != NULL)
		{
			// The second stone is now the next non-joker after the first stone
			// Stones that are before current stone are handled in first case
			secondStone = firstStone->next;
			if (secondStone != NULL && secondStone->isJoker())
			{
				secondStone = secondStone->next;
			}
		}
	}
	
	// If we don't have a second stone to check with, the color can not be determined
	if (secondStone == NULL)
	{
		return;
	}

	// If is same color, then is row with same color and we can take that color for the joker
	// and calculate the number from diff
	if (firstStone->isSameColor(secondStone))
	{
		this->color = firstStone->getColor();
		this->number = firstStone->getNumber() - diff;
	}
	// Otherwise row with same number and we can take that number for the joker
	else 
	{
		this->number = firstStone->getNumber();

		// Check if we just have one more color left, so the joker has to have this color
		if (this->countStonesInRow() == 4)
		{
			if (!this->colorIsAlreadyInRow(BLACK))
			{
				this->color = BLACK;
			}
			else if (!this->colorIsAlreadyInRow(BLUE))
			{
				this->color = BLUE;
			}
			else if (!this->colorIsAlreadyInRow(YELLOW))
			{
				this->color = YELLOW;
			}
			else if (!this->colorIsAlreadyInRow(RED))
			{
				this->color = RED;
			}
		}
	}
}

bool Joker::colorIsAlreadyInRow(int color) const
{
	Gamestone* prev(this->prev);
	while (prev != NULL)
	{
		if (prev->color == color)
		{
			return true;
		}
		prev = prev->prev;
	}

	Gamestone* next(this->next);
	while (next != NULL)
	{
		if (next->color == color)
		{
			return true;
		}
		next = next->next;
	}

	return false;
}

void Joker::storeStone()
{
	Gamestone::storeStone();
	this->oldNumber = number;
	this->oldColor = color;
}

void Joker::restoreStone()
{
	Gamestone::restoreStone();
	this->number = oldNumber;
	this->color = oldColor;
}

// Private methods //
bool Joker::acceptDropping(Gamestone& otherStone)
{
	// Never accept if dragged stone or stone dragging over is in middle of row
	// TODO This is not needed if jokers can replace all stones.
	// TODO Then a menu is needed to select if a stone should be replaced or the joker should be appended
	if ((!this->isFirstInRow() && !this->isLastInRow()) ||
			(!otherStone.isFirstInRow() && !otherStone.isLastInRow()))
	{
		return false;
	}

	// Also don't accept if a first stone is dragged over a first or a last is dragged over a last
	// Just fail, if a row is dragged and not single stones
	if ((this->isFirstInRow() && otherStone.isFirstInRow() && !(this->isLastInRow() || otherStone.isLastInRow())
			) || (
			this->isLastInRow() && otherStone.isLastInRow() && !(this->isFirstInRow() || otherStone.isFirstInRow()))
		)
	{
		return false;
	}

	// Accept, if is same color and a row with same color
	if (this->isSameColor(&otherStone) && this->isRowWithSameColor() && otherStone.isRowWithSameColor() &&
			((this->isFirstInRow() && otherStone.isLastInRow() && this->isSucc(&otherStone)
			) || (
			this->isLastInRow() && otherStone.isFirstInRow() && this->isPred(&otherStone)))
		)
	{
		return true;
	}

	// Also accept if is same number and various color and row with same number
	if  (this->isSameNumber(&otherStone) && this->isRowWithSameNumber() && otherStone.isRowWithSameNumber() &&
			// Needed, because next test doesn't fail if a joker is in the middle of a row with already 4 stones
			this->countStonesInRow() + otherStone.countStonesInRow() <= 4 &&
			!otherStone.colorIsAlreadyInRow(this))
	{
		return true;
	}

	return false;
}

bool Joker::isSucc(Gamestone* otherStone)
{
	// If the number is defined, we can use default tests
	if (this->getNumber() != -1)
	{
		return Gamestone::isSucc(otherStone);
	}
	
	// Otherwise check if we have a non-joker before current stone
	// No need to check for non-jokers after current stone, because then the stone can not be dropped
	Gamestone* toTest(this->next);
	int diff(2);

	if (toTest != NULL && toTest->isJoker())
	{
		toTest = toTest->next;
		diff = 3;
	}

	// We are no successor if we are dropped left of a 1 in a row with same color
	return ((otherStone->isFirstInRow() || otherStone->getNumber() < 13) &&
			((toTest == NULL) || (toTest->getNumber() == otherStone->getNumber() + diff)));
}

bool Joker::isPred(Gamestone* otherStone)
{
	// If the number is defined, we can use default tests
	if (this->getNumber() != -1)
	{
		return Gamestone::isPred(otherStone);
	}
	
	// Otherwise check if we have a non-joker after current stone
	// No need to check for non-jokers before current stone, because then the stone can not be dropped
	Gamestone* toTest(this->prev);
	int diff(2);

	if (toTest != NULL && toTest->isJoker())
	{
		toTest = toTest->prev;
		diff = 3;
	}

	// We are no predessor if we are dropped left of a 1 in a row with same color
	return ((otherStone->isLastInRow() || otherStone->getNumber() > 1) &&
			((toTest == NULL) || (toTest->getNumber() == otherStone->getNumber() - diff)));
}

bool Joker::isSameNumber(Gamestone* otherStone)
{
	// If the number is defined, we can use default tests
	if (this->getNumber() != -1)
	{
		return Gamestone::isSameNumber(otherStone);
	}
	
	// Otherwise check if we have a non-joker after current stone
	Gamestone* toTest(this->next);

	if (toTest != NULL && toTest->isJoker())
	{
		toTest = toTest->next;
	}

	// If not, check if we have a non-joker before current stone
	if (toTest == NULL)
	{
		toTest = this->prev;
	}

	if (toTest != NULL && toTest->isJoker())
	{
		toTest = toTest->prev;
	}

	return (toTest == NULL || toTest->isSameNumber(otherStone));
}

bool Joker::isSameColor(Gamestone* otherStone)
{
	// If the color is defined, we can use default tests
	if (this->getColor() != UNKNOWN)
	{
		return Gamestone::isSameColor(otherStone);
	}
	
	// Otherwise check if we have a non-joker after current stone
	Gamestone* toTest(this->next);

	if (toTest != NULL && toTest->isJoker())
	{
		toTest = toTest->next;
	}

	// If not, check if we have a non-joker before current stone
	if (toTest == NULL)
	{
		toTest = this->prev;
	}

	if (toTest != NULL && toTest->isJoker())
	{
		toTest = toTest->prev;
	}

	return (toTest == NULL || toTest->isSameColor(otherStone));
}

bool Joker::isRowWithSameNumber()
{
	// If we have a defined number we can do the normal check
	if (this->getNumber() != -1)
	{
		return Gamestone::isRowWithSameNumber();
	}

	// If the number is undefined, we can assume that is row with same number
	return true;
}

bool Joker::isRowWithSameColor()
{
	// If we have a defined color we can do the normal check
	if (this->getColor() != UNKNOWN)
	{
		return Gamestone::isRowWithSameColor();
	}

	// If the number is undefined, we can assume that is row with same color
	return true;
}

/*************/
/* Operators */
/*************/
wxDataInputStream &operator>>(wxDataInputStream &input, Joker* stone)
{
	// Restore general gamestone settings
	input >> (Gamestone*)stone;

	// Restore joker specific settings
	int oldColor, oldNumber;
	input >> oldColor >> oldNumber;
	stone->oldColor = (Gamestone::StoneColor)oldColor;
	stone->oldNumber = oldNumber;

	stone->update();

	return input;
}

wxDataOutputStream &operator<<(wxDataOutputStream &output, Joker* stone)
{
	// Store general gamestone settings
	output << (Gamestone*)stone;

	// Store joker specific settings
	output << stone->oldColor << stone->oldNumber;

	return output;
}

#ifdef _DEBUG
std::ostream &operator<<(std::ostream &stream, Joker* stone)
{
	stream << (Gamestone*)stone << " (Joker)";

	return stream;
}
#endif

/******************************************/
/* HERE COMES THE WXWIDGET SPECIFIC STUFF */
/******************************************/
BEGIN_EVENT_TABLE(Joker, wxPanel)
	// The events for painting
	EVT_PAINT(Joker::OnPaint)
	EVT_ERASE_BACKGROUND(Gamestone::OnEraseBackground)

	// The events for drag'n'drop
	EVT_LEFT_DOWN(Joker::OnMouseDown)
	EVT_LEFT_UP(Joker::OnMouseUp)
	#ifdef _DEBUG
		EVT_ENTER_WINDOW(Joker::OnEnterJoker)
	#endif
	EVT_LEAVE_WINDOW(Gamestone::OnLeaveGamestone)
	EVT_MOTION(Gamestone::OnMove)
END_EVENT_TABLE()

void Joker::OnPaint(wxPaintEvent& WXUNUSED(e))
{
	// Get the device context to paint on
	// Always needed, otherwise wxWidgets spams us with paint events
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);

	// Just invisible, if it's on the holder of ai
	if (stoneParent == AIHOLDER)
	{
		dc.Clear();
		return;
	}

	// first draw the background
	if (!invalid)
	{
		dc.SetBrush(wxBrush(wxColour(0xFF, 0xFF, 0xCF)));
	}
	else
	{
		dc.SetBrush(wxBrush(wxColour(0xD0, 0xD0, 0xD0)));
	}
	dc.DrawRectangle(0, 0, width, height);

	// now draw the lines that make the stone look "3d"
	for (int i = 0; i < 3; ++i)
	{
		// Drawing left side
		dc.SetPen(wxPen(*wxWHITE_PEN));
		dc.DrawLine(i, 0, i, height - i);

		// Drawing top
		dc.DrawLine(0, i, width - i, i);

		// Drawing right side
		dc.SetPen(wxPen(*wxLIGHT_GREY_PEN));
		dc.DrawLine(width - i, i, width - i, height);

		// Drawing bottom
		dc.DrawLine(i, height - i, width, height - i);
	}

	// If the stone is still on heap, don't draw joker image
	if (stoneParent == HEAP)
	{
		return;
	}

	// otherwise draw joker image
	int x((this->getWidth() - jokerIcon.GetWidth()) / 2);
	int y((this->getHeight() - jokerIcon.GetHeight()) / 2);
	dc.DrawBitmap(jokerIcon, x, y, true);
}

void Joker::OnMouseDown(wxMouseEvent& e)
{
	Gamestone::OnMouseDown(e);
}

void Joker::OnMouseUp(wxMouseEvent& e)
{
	Gamestone::OnMouseUp(e);
}

#ifdef _DEBUG
void Joker::OnEnterJoker(wxMouseEvent& WXUNUSED(e))
{
	const wxString colorNames[] = { _T("Unknown"), _T("Black"), _T("Blue"), _T("Yellow"), _T("Red") };
	wxString out;
	out << this->number << _T(" (") << colorNames[this->color + 1] << _T(") ") << this->first;
	this->SetToolTip(out);
}
#endif
