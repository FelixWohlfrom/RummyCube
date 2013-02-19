/*
 * MainWindow.cpp
 *
 *  Created on: 21.07.2011
 *      Author: Felix Wohlfrom
 */

#include "MainWindow.h"

#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/statbmp.h>
#include <wx/filename.h>

#include "TutorialWindow.h"
#include "PropertiesWindow.h"
#include "CreateNetworkGameWindow.h"
#include "JoinNetworkGameWindow.h"
#include "AboutWindow.h"

#include "../Internationalization.h"
#include "../game/Settings.h"
#include "../game/RummyCube.h"
#include "../network/Network.h"
#include "../game/player/NetworkPlayer.h"

#ifndef _WIN32
	// On win systems these files are included using resources/msw/RummyCube.rc
	#include "resources/icon.xpm"
	#include "resources/hand.xpm"
	#include "resources/gameboard.xpm"
	#include "resources/holder.xpm"
#endif

#ifdef _DEBUG
	#include <iostream>
#endif

// The event table for wxwidgets
BEGIN_EVENT_TABLE(MainWindow, wxWindow)
	// Window events
	#ifdef _WIN32
		EVT_SET_FOCUS(MainWindow::OnFocus)
	#endif
	EVT_CLOSE(MainWindow::OnClose)
	EVT_SIZE(MainWindow::OnResize)

	// Main menu events
	EVT_MENU(wxID_NEW, MainWindow::OnNewGame)
	EVT_MENU(ID_PASS, MainWindow::OnNextRound)
	EVT_MENU(wxID_OPEN, MainWindow::OnLoadGame)
	EVT_MENU(wxID_SAVE, MainWindow::OnSaveGame)
	EVT_MENU(ID_TUTORIAL, MainWindow::OnShowTutorial)
	EVT_MENU(wxID_EXIT, MainWindow::OnExit)
	EVT_MENU(wxID_PROPERTIES, MainWindow::OnProperties)
	EVT_MENU(ID_MOVEALLSTONES, MainWindow::OnMoveAllStones)
	//EVT_MENU(ID_LANGUAGE, MainWindow::OnChangeLanguage)  // This is not needed, the languages use their language id as event ids
	EVT_MENU(ID_CREATENETWORKGAME, MainWindow::OnCreateNetworkGame)
	EVT_MENU(ID_JOINNETWORKGAME, MainWindow::OnJoinNetworkGame)
	EVT_MENU(ID_TESTSTONES, MainWindow::OnTestStonesOnGameboard)
	EVT_MENU(wxID_ABOUT, MainWindow::OnAboutRummyCube)

	// Show context menu
	EVT_CONTEXT_MENU(MainWindow::ShowContextMenu)

	// Gamestone events
	EVT_PARENT_CHANGED(MainWindow::OnGamestoneParentChanged)

	// Timer event
	EVT_TIMER(ID_UPDATETIMERLABEL, MainWindow::OnTimeLimitTimer)

	// Network game closed event
	EVT_NETWORK_GAME_CLOSED(MainWindow::OnNetworkGameClosed)
END_EVENT_TABLE()

MainWindow::MainWindow() :
	wxFrame(NULL, wxID_ANY, wxTheApp->GetAppName()), gameState(GAME_IDLE),
	game(NULL), heap(NULL), board(NULL), holder(NULL), infoLabel(NULL),
	timerLabel(NULL), timerLabelUpdater(this, ID_UPDATETIMERLABEL), mainMenu(NULL)
{
	// This is needed to allow jpeg/png/gif/... image handling
	wxInitAllImageHandlers();

	// Fire up the game
	game = new RummyCube();

	// Initialize settings, i18n and timer
	Settings* settings = Settings::getInstance();
	Internationalization::getInstance();
	timerLabelUpdater.Start(1000);

	// Set program icon and main font
	this->SetIcon(wxICON(appicon));

	// Initialize the main window parts
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	this->SetBackgroundColour(wxColour(0x00, 0xAE, 0x00));

	// Top area, including button and heap
	wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
	ImagePanel* passButton = new ImagePanel(this, _T(""), wxBITMAP(hand), ID_PASS, wxPoint(0, 0), wxSize(40, 55));
	// Normally bitmaps don't allow static listeners, so we need to connect on runtime
	passButton->Connect(wxEVT_LEFT_UP, wxCommandEventHandler(MainWindow::OnNextRound), NULL, this);
	heap = new wxPanel(this);
	heap->SetBackgroundColour(wxColor(0x00, 0xAE, 0x00));
	topSizer->Add(passButton, wxSizerFlags(0).Left());
	topSizer->Add(heap, wxSizerFlags(1).Expand());
	mainSizer->Add(topSizer, wxSizerFlags(0).Expand());

	// The main board
	wxBoxSizer* boardSizer = new wxBoxSizer(wxVERTICAL);
	board = new ImagePanel(this, settings->Read(_T("General/GameboardPath"), _T("")), wxBITMAP(gameboard), wxID_ANY, wxDefaultPosition, wxDefaultSize);
	board->SetBackgroundColour(wxColor(0x00, 0xAE, 0x00));
	boardSizer->Add(board, wxSizerFlags(1).Expand());
	mainSizer->Add(boardSizer, wxSizerFlags(2).Expand());

	// The info panel
	wxBoxSizer* infoPanelSizer = new wxBoxSizer(wxHORIZONTAL);
	infoLabel = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	// Normally labels don't allow static listeners, so we need to connect on runtime
	infoLabel->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MainWindow::takeAllStones), NULL, this);
	#ifdef _WIN32
		wxFont font(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	#else
		wxFont font(12, wxFONTFAMILY_DECORATIVE, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	#endif
	infoLabel->SetFont(font);
	infoLabel->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
	timerLabel = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	timerLabel->SetFont(font);
	infoPanelSizer->Add(infoLabel, wxSizerFlags(1).Left().Expand());
	infoPanelSizer->Add(timerLabel, wxSizerFlags(1).Right().Expand());
	mainSizer->Add(infoPanelSizer, wxSizerFlags(0).Expand());

	// The holder
	wxBoxSizer* holderSizer = new wxBoxSizer(wxVERTICAL);
	holder = new ImagePanel(this, settings->Read(_T("General/HolderPath"), _T("")), wxBITMAP(holder));
	holder->SetBackgroundColour(wxColour(0xD2, 0x69, 0x1E));
	holderSizer->Add(holder, wxSizerFlags(1).Expand());
	mainSizer->Add(holderSizer, wxSizerFlags(1).Expand());
	this->SetSizer(mainSizer);

	// Init main window position and size
	// Can not be done earlier, because SetSize() fires an OnResize event
	// and event handler will run into a null pointer
	int screenWidth, screenHeight;
	wxDisplaySize(&screenWidth, &screenHeight);
	int height, width, top, left;
	settings->Read(_T("Position/Height"), &height, screenHeight - (screenHeight / 5));
	settings->Read(_T("Position/Width"), &width, screenWidth - (screenWidth / 3));
	settings->Read(_T("Position/Top"), &top, 100);
	settings->Read(_T("Position/Left"), &left, 30);
	this->SetSize(width, height);
	this->Move(wxPoint(left, top));

	// Generate main menu
	// Also here a OnResize event will be fired
	this->generateMainMenu();

	#ifdef __WXGTK__
		// Force redraw of the menu to fix some issues on gamestone placement in linux
		this->SendSizeEvent();
	#endif

	#ifndef _WIN32
		// Initialisation on non-windows systems
		// On windows initialisation is done in OnFocus event
		this->initStones();
	#endif
}

void MainWindow::generateMainMenu()
{
	// Game submenu
	wxMenu* menuGame = new wxMenu();
	menuGame->Append(wxID_NEW, _("&New Game\tCtrl+N"));
	menuGame->Append(ID_PASS, _("&Pass (Next Round)\tN"));
	menuGame->AppendSeparator();
	menuGame->Append(wxID_OPEN, _("&Load Game...\tCtrl+L"));
	menuGame->Append(wxID_SAVE, _("&Save Game...\tCtrl+S"));
	menuGame->AppendSeparator();
	menuGame->Append(ID_TUTORIAL, _("&Tutorial..."));
	#ifndef __WXMAC__
		menuGame->AppendSeparator();
	#else
		wxApp::s_macExitMenuItemId = wxID_EXIT;
	#endif
	menuGame->Append(wxID_EXIT, _("&Close Game\tAlt+F4"));

	// Language subsubmenu for options submenu
	wxMenu* menuLanguages = new wxMenu();
	long curLang = Internationalization::getInstance()->getLanguage();
	std::set<Language>& langs = Internationalization::getInstance()->getInstalledLanguages();
	// First append default entry
	Language defaultLang = Internationalization::getInstance()->getDefaultLanguage();
	int id = wxLANGUAGE_DEFAULT + 1; // ID of default language is 0. The event handler doesn't like this, so we just add 1 here and substract it on OnChangeLanguage Method
	wxMenuItem* item = menuLanguages->Append(id, wxString::Format(_("System-Default (%s)"), defaultLang.name.c_str()), defaultLang.name, wxITEM_RADIO);
	if (curLang == wxLANGUAGE_DEFAULT)
	{
		item->Check();
	}
	this->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnChangeLanguage));
	// Now append all other languages
	for (std::set<Language>::iterator lang(langs.begin()); lang != langs.end(); ++lang)
	{
		int id = (*lang).identifier + 1; // Same as above
		wxMenuItem* item = menuLanguages->Append(id, (*lang).name, (*lang).name, wxITEM_RADIO);
		if ((*lang).identifier == curLang)
		{
			item->Check();
		}
		this->Connect(id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainWindow::OnChangeLanguage));
	}

	// Options submenu
	wxMenu* menuOptions = new wxMenu();
	#ifdef __WXMAC__
		wxApp::s_macPreferencesMenuItemId = wxID_PROPERTIES;
	#endif
	menuOptions->Append(wxID_PROPERTIES, _("&Settings...\tCtrl+P"));
	menuOptions->Append(ID_MOVEALLSTONES, _("Move &All Stones\tA"), _("Move all stones in the moved row"), wxITEM_CHECK);
	menuOptions->AppendSeparator();
	menuOptions->Append(ID_SOUNDS, _("S&ounds"), _("Enable/disable sounds"), wxITEM_CHECK);
	menuOptions->AppendSeparator();
	menuOptions->AppendSubMenu(menuLanguages, _("&Language"));

	// Multiplayer submenu
	wxMenu* menuMultiplayer = new wxMenu();
	menuMultiplayer->Append(ID_CREATENETWORKGAME, _("Create &Network Game..."));
	menuMultiplayer->AppendSeparator();
	menuMultiplayer->Append(ID_JOINNETWORKGAME, _("&Join A Game..."));

	// Help submenu
	wxMenu* menuHelp = new wxMenu();
	menuHelp->Append(ID_TESTSTONES, _("&Test Stones On Gameboard\tT"));
	menuHelp->AppendSeparator();
	menuHelp->Append(ID_UPDATE, _("&Search For Updates..."));
	#ifndef __WXMAC__
		menuHelp->AppendSeparator();
	#else
		wxApp::s_macAboutMenuItemId = wxID_ABOUT;
	#endif
	menuHelp->Append(wxID_ABOUT, _("&About RummyCube..."));

	// The main menu bar
	this->SetMenuBar(NULL); // Avoid memory leak
	wxDELETE(mainMenu);
	mainMenu = new wxMenuBar();
	mainMenu->Append(menuGame, _("&Game"));
	mainMenu->Append(menuOptions, _("Options"));
	mainMenu->Append(menuMultiplayer, _("&Multiplayer"));
	#ifdef __WXMAC__
		wxApp::s_macHelpMenuTitleName = _("&Help");
	#endif
	mainMenu->Append(menuHelp, _("&Help"));

	this->SetMenuBar(mainMenu);

	// Update some menu entries
	if (game->getHumanPlayer()->getStonesLeftToTake() == 0 ||
				game->getStoneCountOnHeap() == 0)
	{
		mainMenu->Enable(ID_PASS, true);
	}
	else
	{
		mainMenu->Enable(ID_PASS, false);
	}
	mainMenu->Check(ID_MOVEALLSTONES, game->moveAllStonesInRow());
}

MainWindow::~MainWindow()
{
	// Stop timer
	timerLabelUpdater.Stop();

	// Close game
	wxDELETE(game);

	// Store position
	Settings* settings = Settings::getInstance();
	settings->Write(_T("Position/Height"), this->GetSize().GetHeight());
	settings->Write(_T("Position/Width"), this->GetSize().GetWidth());
	settings->Write(_T("Position/Top"), this->GetPosition().y);
	settings->Write(_T("Position/Left"), this->GetPosition().x);

	settings->Flush();
}

void MainWindow::ShowContextMenu(wxContextMenuEvent& e)
{
	wxPoint pos = this->ScreenToClient(e.GetPosition());

	wxMenu* contextMenu = new wxMenu;
	contextMenu->Append(ID_MOVEALLSTONES, _("Move &All Stones\tA"), _("Move all stones in the moved row"), wxITEM_CHECK);
	contextMenu->Check(ID_MOVEALLSTONES, game->moveAllStonesInRow());
	contextMenu->Append(ID_TESTSTONES, _("&Test Stones On Gameboard\tT"));
	contextMenu->Append(ID_PASS, _("&Pass (Next Round)\tN"));
	contextMenu->Enable(ID_PASS, mainMenu->IsEnabled(ID_PASS));
	PopupMenu(contextMenu, pos);
	wxDELETE(contextMenu);
}

#ifdef _WIN32
void MainWindow::OnFocus(wxFocusEvent& WXUNUSED(e))
{
	// Just init the stones if not already initialized
	if (game->getStones().size() == 0)
	{
		this->initStones();
	}
}
#endif

void MainWindow::OnClose(wxCloseEvent& e)
{
	if (gameState == GAME_CLOSING)
	{
		return;
	}

	if (!e.CanVeto())
	{
		if (gameState == GAME_OPPONENT_PLAYING)
		{
			game->stopGame();
			gameState = GAME_CLOSING;
		}
		else
		{
			this->Destroy();
		}
	}
	else
	{
		// First stop timer
		bool timeLimitRunning = game->getTimer()->isRunning();
		game->getTimer()->pause();

		// Show dialog
		// Security question on window close
		wxMessageDialog dialog(this, _("Do you really want to close the game?"), _("RummyCube"), wxYES_NO | wxICON_QUESTION);
		switch (dialog.ShowModal())
		{
			case wxID_YES:
				if (gameState == GAME_OPPONENT_PLAYING)
				{
					game->stopGame();
					gameState = GAME_CLOSING;
					break;
				}
				else
				{
					this->Destroy();
					break;
				}
			case wxID_NO:
				e.Veto();
				break;
		}

		// Resume timer if was running
		if (timeLimitRunning)
		{
			game->getTimer()->resume();
		}
	}
}

void MainWindow::OnResize(wxSizeEvent& e)
{
	this->SetSize(e.GetSize());
	this->Layout();
	this->setInfoLabelText();
}

void MainWindow::restartGame()
{
	// Reset menu entries, cursor and settings
	mainMenu->Enable(wxID_OPEN, true);
	mainMenu->Enable(wxID_SAVE, true);
	mainMenu->Enable(ID_PASS, false);
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
	Gamestone::canMoveStones = true;

	// Reset game
	wxDELETE(game);
	game = new RummyCube();
	this->gameState = GAME_IDLE;

	this->initStones();
}

void MainWindow::initStones()
{
	// Create the stones
	game->createStones(heap);
	std::vector<Gamestone*>& stones = game->getStones();

	int y(5);
	for (unsigned int i(0); i < stones.size(); ++i)
	{
		int x((rand() % (heap->GetSize().GetWidth() - 60)) + 20);
		stones.at(i)->SetPosition(wxPoint(x, y));
		stones.at(i)->storeStone();

		// This makes the stones appear one after another
		stones.at(i)->Show();
		wxYield();
	}

	// Init information panel
	this->setInfoLabelText();

	// Update pass buttons
	mainMenu->Enable(ID_PASS, false);
}

void MainWindow::setInfoLabelText()
{
	// If more then one stone can be taken
	if (game->getHumanPlayer()->getStonesLeftToTake() > 1 && game->getStoneCountOnHeap() > 0)
	{
		infoLabel->SetLabel(wxString::Format(_("Please get a stone (%d more) or click here to get all at once"), game->getHumanPlayer()->getStonesLeftToTake()));
		infoLabel->SetToolTip(infoLabel->GetLabel());
	}
	// If only one stone can be taken
	else if (game->getHumanPlayer()->getStonesLeftToTake() == 1 && game->getStoneCountOnHeap() > 0)
	{
		infoLabel->SetLabel(_("Please get a stone"));
		infoLabel->SetToolTip(_("Please get a stone"));
	}
	// If no more stones can be taken
	else
	{
		infoLabel->SetLabel(_("It's your turn"));
		infoLabel->SetToolTip(_("It's your turn"));
	}

	// Make the label text fit the label
	// FIXME Find out in which cases it works and in which not
	wxCoord textWidth, textHeight;
	wxClientDC dc(infoLabel);
	dc.GetTextExtent(infoLabel->GetLabel(), &textWidth, &textHeight);

	while (textWidth >= (this->GetSize().GetWidth() / 2)) // Label width is not updated on screen resize, so the main window size is used
	{
		wxString newLabel(infoLabel->GetLabel().SubString(0, infoLabel->GetLabel().Length() - 5) + _T("..."));
		infoLabel->SetLabel(newLabel);
		dc.GetTextExtent(infoLabel->GetLabel(), &textWidth, &textHeight);
		this->Layout();
	}
}

void MainWindow::setTimerLabelText()
{
	if (game->getTimer()->getTime() > -1)
	{
		// Mark background red if < 10 seconds left
		if (game->getTimer()->timeLeft() <= 10 &&
				// and even time value
				!(game->getTimer()->timeLeft() & 0x01))
		{
			timerLabel->SetForegroundColour(wxColor(0xff, 0x00, 0x00));
		}
		else
		{
			timerLabel->SetForegroundColour(wxColor(0x00, 0x00, 0x00));
		}

		int min = game->getTimer()->timeLeft() / 60;
		int sec = game->getTimer()->timeLeft() % 60;
		timerLabel->SetLabel(wxString::Format(_("%d:%02d minutes"), min, sec));
		timerLabel->SetToolTip(wxString::Format(_("%d:%02d minutes"), min, sec));
	}
	else
	{
		timerLabel->SetLabel(_T(""));
		timerLabel->SetToolTip(_T(""));
	}

	this->Layout();
}

void MainWindow::takeAllStones(wxMouseEvent& WXUNUSED(e))
{
	// If the stones can not be moved, don't take the stones
	if (!Gamestone::canMoveStones) return;

	std::vector<Gamestone*> stones = game->getStones();
	while (game->getHumanPlayer()->getStonesLeftToTake() > 0 && game->getStoneCountOnHeap() > 0)
	{
		int index = rand() % stones.size();
		while (stones.at(index)->getParent() != Gamestone::HEAP)
		{
			index = rand() % stones.size();
		}
		stones.at(index)->setParent(Gamestone::HOLDER);
	}
}

void MainWindow::OnGamestoneParentChanged(ParentChangedEvent& e)
{
	Gamestone& sender(*(e.GetSender()));

	switch (e.GetParent())
	{
		case Gamestone::AIHOLDER:
			// Move stone to invisible area on heap
			// No break needed, needs to be reparented too on game loading
			sender.SetPosition(wxPoint(0, 100));
			/* no break */
		case Gamestone::HEAP:
			sender.Reparent(heap);
			break;
		case Gamestone::BOARD:
			sender.Reparent(board);
			if (!e.GetStoneMoved() && // Stones are moved if is network game
				sender.getParent() == Gamestone::AIHOLDER && sender.isFirstInRow()) // Otherwise ai plays, stones are automatically moved to board
			{
				this->moveToBoard(sender);
			}
			break;
		case Gamestone::HOLDER:
			// Just move to holder if there are still stones left to take
			// or if the stone is drag'n'dropped
			if (game->getHumanPlayer()->getStonesLeftToTake() > 0 || e.GetStoneMoved())
			{
				if (!e.GetStoneMoved())
				{
					// Start game timer if clicked on stone
					game->getTimer()->start();
				}

				sender.Reparent(holder);
				if (!e.GetStoneMoved())
				{
					game->getHumanPlayer()->addStone(sender);
					moveToHolder(sender);
					setInfoLabelText();
					if (game->getHumanPlayer()->getStonesLeftToTake() == 0)
					{
						mainMenu->Enable(ID_PASS, true);
					}
					sender.storeStone(Gamestone::HOLDER);
				}
			}
			else
			{
				e.Veto();
			}
			break;
	}
}

void MainWindow::moveToHolder(Gamestone& gamestone)
{
	// Get screen size
	int screenWidth, screenHeight;
	wxDisplaySize(&screenWidth, &screenHeight);

	int y(10);
	int x(0);

	// If stone is joker, move completely to right
	if (gamestone.isJoker())
	{
		x = 14;
	}
	else
	{
		// Top of stone depends on color
		switch (gamestone.getColor())
		{
			case Gamestone::BLACK:	y = 0;
									break;
			case Gamestone::BLUE:	y = 1;
									break;
			case Gamestone::YELLOW:	y = 2;
									break;
			case Gamestone::RED:	y = 3;
									break;
			default: 				break;	// Avoid warning
		}
		// Calculate top position
		y = y * (gamestone.getHeight() + 2) + screenHeight / 200 + 10;

		// Init left position
		x = gamestone.getNumber();
	}

	// Calculate left position
	// Height, not width to calculate position so a little space between the stones is added
	x = x * gamestone.getHeight() + screenHeight / 100;
	if (gamestone.isFirst())
	{
		x += 5;
	}
	else
	{
		x += 10;
	}

	// Move the stone
	gamestone.SetPosition(wxPoint(x, y));

	// Bring stones to front
	// FIXME Needs a custom panel which draws the overlapping stones correctly
	for (std::vector<Gamestone*>::iterator stone(game->getStones().begin()); stone != game->getStones().end(); ++stone)
	{
		if ((*stone)->getParent() == Gamestone::HOLDER &&
				!(*stone)->isFirst())
		{
			(*stone)->Refresh();
		}
	}
}

void MainWindow::moveToBoard(Gamestone& stone)
{
	// Calculate new x and y values of position
	int x(50);
	int y(20);
	int rowLength(stone.countStonesInRow() * stone.GetSize().x);
	#ifdef _DEBUG
		std::cout << "Try to play out stone " << &stone << std::endl;
	#endif

	while (this->stoneUnderRow(x, y, rowLength))
	{
		y += stone.GetSize().y + 10;

		if (y + stone.GetSize().y > this->board->GetSize().y)
		{
			y = 20;
			x += stone.GetSize().x + 10;
		}
	}

	// Set position
	#ifdef _DEBUG
		std::cout << "Moving stone " << &stone << " to pos (" << x << ", " << y << ")" << std::endl;
	#endif
	stone.SetPosition(wxPoint(x, y));
}

bool MainWindow::stoneUnderRow(int x, int y, int rowLength)
{
	// Check if any of the stones are under the new position of the row
	std::vector<Gamestone*> gamestones = game->getStones();
	for (std::vector<Gamestone*>::iterator stone(gamestones.begin()); stone != gamestones.end(); ++stone)
	{
		if ((*stone)->getParent() == Gamestone::BOARD &&
			(
				// Horizontal left end is in the row
				((*stone)->GetPosition().x >= x && (*stone)->GetPosition().x <= x + rowLength) ||
				// Horizontal right end is in the row
				((*stone)->GetPosition().x + (*stone)->GetSize().x >= x && (*stone)->GetPosition().x + (*stone)->GetSize().x <= x + rowLength)
			) && (
				// Vertical top end is in the row
				((*stone)->GetPosition().y >= y && (*stone)->GetPosition().y <= y + (*stone)->GetSize().y) ||
				((*stone)->GetPosition().y + (*stone)->GetSize().y >= y && (*stone)->GetPosition().y + (*stone)->GetSize().y <= y  +(*stone)->GetSize().y)
			))
		{
			#ifdef _DEBUG
				std::cout << " Stone at pos (" << x << ", " << y << "): " << *stone << std::endl;
			#endif
			return true;
		}
	}

	return false;
}

// Menu handlers //
void MainWindow::OnNewGame(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	// Show dialog
	wxMessageDialog newGame(this, _("Do you really want to close the current game and start a new one?"), _("RummyCube"), wxYES_NO | wxICON_QUESTION);
	if (newGame.ShowModal() == wxID_YES)
	{
		if (gameState == GAME_OPPONENT_PLAYING)
		{
			game->stopGame();
			gameState = GAME_RESTARTING;
		}
		else
		{
			this->restartGame();
		}
	}

	// Resume timer if was running
	if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

void MainWindow::OnNextRound(wxCommandEvent& WXUNUSED(e))
{
	// Small check, because we can't disable the hand "button" and adding/removing listeners looks like overkill to me
	if (!mainMenu->IsEnabled(ID_PASS)) return;

	// Check if all rules have been observed
	if (!game->testStonesOnGameboard())
	{
		// If not, ask if stones should be reset
		wxMessageDialog ruleFailed(this, _("At least one stone doesn't lie on the game board as told in the rules (shown in gray). Do you want to continue and move ALL stones to the position they had at the beginning of the round?"), _("RummyCube"), wxYES_NO | wxICON_QUESTION);
		if (ruleFailed.ShowModal() == wxID_NO)
		{
			return;
		}
		else
		{
			// FIXME Wenn im Netzwerkspiel der Client das hier aufruft, werden alle Steine (auch die des Servers) auf den Halter zurückgelegt
			std::vector<Gamestone*> gamestones(game->getStones());
			for (std::vector<Gamestone*>::iterator stone(gamestones.begin()); stone != gamestones.end(); ++stone)
			{
				(*stone)->restoreStone();
				(*stone)->setInvalid(false);
			}
		}
	}

	this->SetCursor(wxCursor(wxCURSOR_WAIT));
	mainMenu->Enable(ID_PASS, false);
	mainMenu->Enable(wxID_OPEN, false);
	mainMenu->Enable(wxID_SAVE, false);
	Gamestone::canMoveStones = false;
	game->getTimer()->pause();

	// Notifies the opponents that the round is finished
	game->roundFinished();

	if (game->getHumanPlayer()->hasWon())
	{
		wxMessageDialog youWon(this, _("You won the game. Congratulations! Do you want to start a new singleplayer game? Otherwise game will be closed."), _("RummyCube"), wxYES_NO | wxICON_QUESTION);
		if (youWon.ShowModal() == wxID_YES)
		{
			this->restartGame();
		}
		else
		{
			this->Close(true);
		}
		return;
	}

	gameState = GAME_OPPONENT_PLAYING;
	std::deque<OpponentPlayer*> opponentPlayers = game->getOpponentPlayers();
	for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
	{
		infoLabel->SetLabel(wxString::Format(_("Player %s: I'm playing"), (*player)->getPlayerName().c_str()));
		infoLabel->SetToolTip(wxString::Format(_("Player %s: I'm playing"), (*player)->getPlayerName().c_str()));
		wxYield();

		(*player)->play();

		if ((*player)->hasWon())
		{
			wxMessageDialog youWon(this, wxString::Format(_("Player %s won the game.\nDo you want to try it one more time? Otherwise game will be closed."), (*player)->getPlayerName().c_str()), _("RummyCube"), wxYES_NO | wxICON_QUESTION);
			if (youWon.ShowModal() == wxID_YES)
			{
				this->restartGame();
			}
			else
			{
				this->Close(true);
			}
			return;
		}
	}

	// Check gamestate, can be changed during wxYield calls in (*player)->play()
	GameState state = gameState;
	gameState = GAME_IDLE;
	switch (state)
	{
		case GAME_CLOSING:		this->Close(true);
								return;
		case GAME_RESTARTING:	this->restartGame();
								return;
		// TODO Handle other events
		default:				break;
	}

	Gamestone::canMoveStones = true;
	game->getTimer()->start();
	game->getHumanPlayer()->newRound();
	this->setInfoLabelText();
	if (game->getStoneCountOnHeap() == 0)
	{
		mainMenu->Enable(ID_PASS, true);
	}
	mainMenu->Enable(wxID_OPEN, true);
	mainMenu->Enable(wxID_SAVE, true);
	this->SetCursor(wxCursor(wxCURSOR_ARROW));
}

void MainWindow::OnLoadGame(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	// Show dialog
	wxFileDialog dialog(this, _("Load a game"), _T(""), _T(""), _("RummyCube Savegame (*.rcs)|*.rcs"),
			wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxString result = game->loadGame(dialog.GetFilename());

		// Loading failure, show error message and return
		if (result != _T(""))
		{
			wxMessageDialog(this, _("Error during file loading:\n") + result, _("Error"), wxOK | wxICON_ERROR).ShowModal();

			// Resume timer if was running
			if (timeLimitRunning)
			{
				game->getTimer()->resume();
			}

			return;
		}

		// Loading succeded, do some final updates on gui
		if (game->getHumanPlayer()->getStonesLeftToTake() == 0 ||
				game->getStoneCountOnHeap() == 0)
		{
			mainMenu->Enable(ID_PASS, true);
		}
		else
		{
			mainMenu->Enable(ID_PASS, false);
		}
		this->setInfoLabelText();
		return;
	}

	// Resume timer if was running
	if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

void MainWindow::OnSaveGame(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	// Show dialog
	wxFileDialog dialog(this, _("Save current game"), _T(""), _T(""), _("RummyCube Savegame (*.rcs)|*.rcs"),
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT | wxFD_CHANGE_DIR);

	if (dialog.ShowModal() == wxID_OK)
	{
		wxFileName filename(dialog.GetFilename());
		wxString filenameToSave;
		if (filename.GetExt() != _T("rcs"))
		{
			filenameToSave = filename.GetName() + _T(".rcs");
		}
		else
		{
			filenameToSave = filename.GetFullName();
		}

		if (!game->saveGame(filenameToSave))
		{
			wxMessageDialog(this, _("Error during file saving"), _("Error"), wxID_OK | wxICON_ERROR).ShowModal();
		}
	}

	// Resume timer if was running
	if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

void MainWindow::OnShowTutorial(wxCommandEvent& WXUNUSED(e))
{
	TutorialWindow(this).ShowModal();
}

void MainWindow::OnExit(wxCommandEvent& WXUNUSED(e))
{
	this->Close();
}

void MainWindow::OnProperties(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	// Show dialog
	PropertiesWindow(this).ShowModal();

	// Change images to new values
	Settings* settings = Settings::getInstance();
	holder->setImage(settings->Read(_T("General/HolderPath"), _T("")));
	board->setImage(settings->Read(_T("General/GameboardPath"), _T("")));

	// Resume timer if was running
	if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

void MainWindow::OnMoveAllStones(wxCommandEvent& e)
{
	game->moveAllStonesInRow(e.IsChecked());
	// Update main menu item, context menu item is updated on creation of context menu
	mainMenu->Check(ID_MOVEALLSTONES, game->moveAllStonesInRow());
}

void MainWindow::OnChangeLanguage(wxCommandEvent& e)
{
	// Reinitialize language file (id - 1 because it's incremented in menu entry to workaround event handling issues with 0 id)
	Internationalization::getInstance()->setLanguage(e.GetId() - 1);

	// Reset main menu and info labels
	this->generateMainMenu();
	this->setInfoLabelText();
}

void MainWindow::OnCreateNetworkGame(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	// Show dialog and resume timer if no network game was created
	CreateNetworkGameWindow networkGame(this);
	if (networkGame.ShowModal() == wxID_OK)
	{
		std::deque<OpponentPlayer*> opponentPlayers = game->getOpponentPlayers();
		for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
		{
			((NetworkPlayer*)(*player))->setCloseEventHandler(this);
		}

		if (gameState != GAME_IDLE)
		{
			gameState = GAME_CREATE_NETWORKGAME;
			game->stopGame();
		}
		else
		{
			wxDELETE(game);
			game = networkGame.getGame();
			this->initStones();
		}
	}
	else if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

void MainWindow::OnJoinNetworkGame(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	JoinNetworkGameWindow networkGame(this);
	if (networkGame.ShowModal() == wxID_OK)
	{
		// Handle close events just with first opponent player.
		// This event just can happen, if the server closes the game, so the sender is known
		std::deque<OpponentPlayer*> opponentPlayers = game->getOpponentPlayers();
		((NetworkPlayer*)(*(opponentPlayers.begin())))->setCloseEventHandler(this);

		// Reset stones
		game = networkGame.getGame();
		this->initStones();

		// TODO Receive initial stone positions from host

		// Let the players play which are before current player
		this->SetCursor(wxCursor(wxCURSOR_WAIT));
		mainMenu->Enable(ID_PASS, false);
		mainMenu->Enable(wxID_OPEN, false);
		mainMenu->Enable(wxID_SAVE, false);
		Gamestone::canMoveStones = false;
		game->getTimer()->pause();

		gameState = GAME_OPPONENT_PLAYING;
		for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
		{
			infoLabel->SetLabel(wxString::Format(_("Player %s: I'm playing"), (*player)->getPlayerName().c_str()));
			infoLabel->SetToolTip(wxString::Format(_("Player %s: I'm playing"), (*player)->getPlayerName().c_str()));
			wxYield();

			(*player)->play();

			if ((*player)->hasWon())
			{
				wxMessageDialog youWon(this, wxString::Format(_("Player %s won the game.\nDo you want to try it one more time? Otherwise game will be closed."), (*player)->getPlayerName().c_str()), _("RummyCube"), wxYES_NO | wxICON_QUESTION);
				if (youWon.ShowModal() == wxID_YES)
				{
					this->restartGame();
				}
				else
				{
					this->Close(true);
				}
				return;
			}
		}

		// Check gamestate, can be changed during wxYield calls in (*player)->play()
		GameState state = gameState;
		gameState = GAME_IDLE;
		switch (state)
		{
			case GAME_CLOSING:		this->Close(true);
									return;
			case GAME_RESTARTING:	this->restartGame();
									return;
			// TODO Handle other events
			default:				break;
		}

		Gamestone::canMoveStones = true;
		game->getTimer()->start();
		this->setInfoLabelText();
		if (game->getStoneCountOnHeap() == 0)
		{
			mainMenu->Enable(ID_PASS, true);
		}
		mainMenu->Enable(wxID_OPEN, true);
		mainMenu->Enable(wxID_SAVE, true);
		this->SetCursor(wxCursor(wxCURSOR_ARROW));
		return;
	}

	// Show dialog and resume timer if no network game was created
	if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

void MainWindow::OnTestStonesOnGameboard(wxCommandEvent& WXUNUSED(e))
{
	game->testStonesOnGameboard();
}

void MainWindow::OnAboutRummyCube(wxCommandEvent& WXUNUSED(e))
{
	// First stop timer
	bool timeLimitRunning = game->getTimer()->isRunning();
	game->getTimer()->pause();

	// Show dialog
	AboutWindow(this).ShowModal();

	// Resume timer if was running
	if (timeLimitRunning)
	{
		game->getTimer()->resume();
	}
}

// Timer handler
void MainWindow::OnTimeLimitTimer(wxTimerEvent& WXUNUSED(e))
{
	setTimerLabelText();

	// If no more time is left, force round finish
	if (game->getTimer()->getTime() > -1 &&
			game->getTimer()->timeLeft() == 0)
	{
		// If the player still has some stones left to take, take random stones from holder
		{
			wxMouseEvent tmp; // Dummy, because the method takes an (unused) parameter for the wxWidgets events
			this->takeAllStones(tmp);
		}

		if (!game->testStonesOnGameboard())
		{
			wxMessageDialog(this, _("At least one stone doesn't lie on the game board as told in the rules (shown in gray). ALL stones will be moved to the position they had at the beginning of the round!"), _("RummyCube"), wxOK | wxICON_EXCLAMATION).ShowModal();

			std::vector<Gamestone*> gamestones(game->getStones());
			for (std::vector<Gamestone*>::iterator stone(gamestones.begin()); stone != gamestones.end(); ++stone)
			{
				// FIXME Fehler im Netzwerkspiel
				(*stone)->restoreStone();
				(*stone)->setInvalid(false);
			}
		}

		// Continue next round
		{
			wxCommandEvent tmp; // Also here dummy for wxWidgets event handling
			this->OnNextRound(tmp);
		}
	}
}

// Handler for event that is fired if network game is closed
void MainWindow::OnNetworkGameClosed(NetworkGameClosedEvent& e)
{
	wxMessageDialog(this, wxString::Format(_("Player %s disconnected.\nA new singleplayer game will be started."), e.getClosedBy().c_str()), _("RummyCube"), wxOK).ShowModal();

	if (gameState == GAME_OPPONENT_PLAYING)
	{
		game->stopGame();
		gameState = GAME_RESTARTING;
	}
	else
	{
		this->restartGame();
	}
}
