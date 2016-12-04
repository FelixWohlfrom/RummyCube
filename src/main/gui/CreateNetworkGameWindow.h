/*
 * CreateNetworkGameWindow.h
 *
 *  Created on: 13.07.2016
 *      Author: Felix Wohlfrom
 */

#ifndef CREATENETWORKGAMEWINDOW_H_
#define CREATENETWORKGAMEWINDOW_H_

#include "../game/RummyCubeNetwork.h"

#include <map>

#include <QDialog>
#include <QTcpServer>

namespace Ui {
    class CreateNetworkGameWindow;
}

/**
 * The window to create a new network game.
 */
class CreateNetworkGameWindow : public QDialog
{
    Q_OBJECT

    public:
        /**
         * Constructor. Initializes the window elements.
         *
         * @param parent The parent window
         */
        CreateNetworkGameWindow(QWidget* parent);

        /**
         * Destructor. Deletes the ui object.
         */
        ~CreateNetworkGameWindow();

        /**
         * Returns the created game object.
         */
        RummyCubeNetwork* getGame();

    private:
        /**
         * The ui of the dialog.
         */
        Ui::CreateNetworkGameWindow* ui;

        /**
         * The network game.
         */
        RummyCubeNetwork* game;

        /**
         * The server object.
         */
        QTcpServer* server;

        /**
         * The client sockets and the associated player names.
         * Key is the socket, value is the name of the player.
         */
        std::map<QTcpSocket*, QString> connectedPlayer;

        /**
         * Stores if remote player started game.
         */
        bool canStartGameRemote;

        /**
         * Stores if current player started game.
         */
        bool canStartGameLocal;

        /**
         * Sends the current game settings to connected clients.
         */
        void sendSettings();

        /**
         * Shows settings.
         */
        void showSettingsPanel();

        /**
         * Shows the waiting panel.
         */
        void showWaitingPanel();

        /**
         * Updates the gui. Enables start game button if players
         * are connected, displays player names and different other things.
         */
        void updateGui();

        /**
         * Updates the time limit label.
         */
        void updateTimeLimitLabel();

        /**
         * Starts the game.
         */
        void startGame();

    public slots:
        /**
         * Parses the data received from server socket.
         */
        void parseData();

        /**
         * This event is called when the window gets the focus.
         *
         * @param event The show event
         */
        void showEvent(QShowEvent* event);

        /**
         * Event handler to handle incoming connections.
         */
        void acceptConnectionEvent();

        /**
         * Handles disconnects of the clients.
         */
        void disconnect();

        /**
         * Handles socket errors.
         */
        void displayError(QAbstractSocket::SocketError error);

        /**
         * The handler that sends the settings on changes of spin controls.
         */
        void spinCtrlChangedEvent(QString);

        /**
         * Handles clicks on stones in one row checkbox.
         */
        void stonesInOneRowClickEvent();

        /**
         * Handles clicks on the time limit checkbox.
         */
        void timeLimitEnabledClickEvent();

        /**
         * Updates time limit label. Value is ignored.
         */
        void timeLimitSlideEvent(int);

        /**
         * Event handler for start game button.
         */
        void startGameClickEvent();

        /**
         * Event handler for the cancel button.
         */
        void cancelClickEvent();

        /**
         * Event handler for window closing without network game creation.
         */
        void reject();
};

#endif /* CREATENETWORKGAMEWINDOW_H_ */
