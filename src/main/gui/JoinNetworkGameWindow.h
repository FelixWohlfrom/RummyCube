/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  RummyCube. A small board game.                                       *
 *  Copyright (C) 2016 Felix Wohlfrom                                    *
 *                                                                       *
 *  This program is free software: you can redistribute it and/or modify *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, either version 3 of the License, or    *
 *  (at your option) any later version.                                  *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
 * JoinNetworkGameWindow.h
 *
 *  Created on: 18.07.2016
 *      Author: Felix Wohlfrom
 */

#ifndef JOINNETWORKGAMEWINDOW_H_
#define JOINNETWORKGAMEWINDOW_H_

#include "../game/RummyCubeNetwork.h"
#include "../network/NetworkGameSettings.h"

#include <list>

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
    class JoinNetworkGameWindow;
}

/**
 * This class represents the window that is shown to connect to a network game.
 */
class JoinNetworkGameWindow : public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window.
         */
        JoinNetworkGameWindow(QWidget* parent);

        /**
         * Destructor. Deletes the ui object.
         */
        virtual ~JoinNetworkGameWindow();

        /**
         * Returns the created game object.
         */
        RummyCubeNetwork* getGame();

    private:
        /**
         * The ui of the dialog.
         */
        Ui::JoinNetworkGameWindow* ui;

        /**
         * The network game.
         */
        RummyCubeNetwork* game;

        /**
         * The socket which is used to communicate with the server.
         */
        QTcpSocket* sock;

        /**
         * If we are connected to an server or not.
         */
        bool connected;

        /**
         * Stores if remote player started game.
         */
        bool canStartGameRemote;

        /**
         * Stores if current player started game.
         */
        bool canStartGameLocal;

        /**
         * The name of the connected player.
         */
        std::list<QString> connectedPlayer;

        /**
         * The settings of the network game.
         */
        NetworkGameSettings settings;

        /**
         * Shows settings.
         */
        void showSettingsPanel();

        /**
         * Shows the waiting panel.
         */
        void showWaitingPanel();

        /**
         * Updates the settings on gui.
         */
        void updateGui();

        /**
         * Starts the game.
         */
        void startGame();

    private slots:
        /**
         * Parses the data received from server socket.
         */
        void parseData();

        /**
         * Handles disconnects from the server.
         */
        void disconnect();

        /**
         * Handles socket errors.
         */
        void displayError(QAbstractSocket::SocketError error);

        /**
         * Event handler for connect button.
         */
        void connectClickEvent();

        /**
         * Event handler for start game button.
         */
        void startGameClickEvent();

        /**
         * Event handler for the cancel button.
         */
        void cancelClickEvent();
};

#endif /* JOINNETWORKGAMEWINDOW_H_ */
