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
 * JoinNetworkGameWindow.cpp
 *
 *  Created on: 18.07.2016
 *      Author: Felix Wohlfrom
 */

#include "JoinNetworkGameWindow.h"
#include "ui_JoinNetworkGameWindow.h"

#include "../network/Network.h"

#include <QMessageBox>

JoinNetworkGameWindow::JoinNetworkGameWindow(QWidget* parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint
            | Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::JoinNetworkGameWindow), game(NULL), sock(NULL), connected(false),
    canStartGameRemote(false), canStartGameLocal(false), connectedPlayer()
{
    // Initialize window
    ui->setupUi(this);

    #ifdef _DEBUG
        ui->ipAddress->setText("127.0.0.1");
        ui->nickname->setText("Client");
    #endif
}

JoinNetworkGameWindow::~JoinNetworkGameWindow()
{
    delete ui;
}

RummyCubeNetwork* JoinNetworkGameWindow::getGame()
{
    return game;
}

void JoinNetworkGameWindow::showSettingsPanel()
{
    ui->statusDisplay->setCurrentIndex(0);
}

void JoinNetworkGameWindow::showWaitingPanel()
{
    ui->statusDisplay->setCurrentIndex(1);
}

void JoinNetworkGameWindow::updateGui()
{
    if (canStartGameLocal)
    {
        showWaitingPanel();
    }
    else
    {
        showSettingsPanel();

        if (this->connected)
        {
            ui->stonesAtBeginning->setText(
                    QString("%1").arg(settings.stonesAtBeginning));
            ui->sumAtBeginning->setText(
                    QString("%1").arg(settings.sumAtBeginning));
            if (settings.stonesInOneRow)
            {
                ui->stonesInOneRow->setText(tr("Yes"));
            }
            else
            {
                ui->stonesInOneRow->setText(tr("No"));
            }
            if (settings.timeLimit == -1)
            {
                ui->timeLimit->setText(tr("None"));
            }
            else
            {
                int min = settings.timeLimit / 60;
                int sec = settings.timeLimit % 60;
                ui->timeLimit->setText(tr("%1:%2 minutes")
                        .arg(min).arg(sec, 2, 10, QLatin1Char('0')));
            }
        }
        else
        {
            ui->stonesAtBeginning->setText("");
            ui->sumAtBeginning->setText("");
            ui->stonesInOneRow->setText("");
            ui->timeLimit->setText("");
        }
    }
}

void JoinNetworkGameWindow::startGame()
{
    if (canStartGameLocal && canStartGameRemote)
    {
        // Create the game
        game = new RummyCubeNetwork(
                settings, sock, ui->nickname->text(), connectedPlayer);

        sock->disconnect(this);

        this->accept();
    }

    updateGui();
}

void JoinNetworkGameWindow::parseData()
{
    // Read first line of message, it's always a QString
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    while (sock->bytesAvailable() > 0)
    {
        QString msg = Network::read(*sock);

        if (msg == "toManyPlayers")
        {
            disconnect();
            QMessageBox::information(this, "RummyCube",
                    tr("Currently there are to many players connected to the"
                            " server.\nPlease use another one."));
        }
        else if (msg == "wrongVersion")
        {
            disconnect();
            QMessageBox::information(this, "RummyCube",
                    tr("You are using an older version for the game than the"
                            " server. Please upgrade your game to the latest"
                            " version."));
        }
        else if (msg == "connected")
        {
            // Send information that we use the RummyCube protocol version 1
            Network::write(*sock, "version");
            Network::write(*sock, "RummyCube 1");
            // Send nickname of player
            Network::write(*sock, "nickname");
            Network::write(*sock, ui->nickname->text());

            connected = true;
            updateGui();
        }
        else if (msg == "settings")
        {
            // Receive settings
            settings.stonesAtBeginning = Network::read(*sock).toInt();
            settings.sumAtBeginning = Network::read(*sock).toInt();
            settings.stonesInOneRow = Network::read(*sock) == "1";
            settings.timeLimit = Network::read(*sock).toInt();
            ui->startGameButton->setEnabled(true);
            canStartGameLocal = false;
            updateGui();
        }
        else if (msg == "startGame")
        {
            canStartGameRemote = true;
            startGame();
            return;
        }
        else if (msg == "dontStartGame")
        {
            canStartGameRemote = false;
            return;
        }
        else if (msg == "playerNames")
        {
            connectedPlayer.clear();

            int count = Network::read(*sock).toInt();
            while (count > 0)
            {
                connectedPlayer.push_back(Network::read(*sock));
                count--;
            }
        }
    }
}

void JoinNetworkGameWindow::disconnect()
{
    connected = false;
    canStartGameLocal = false;
    canStartGameRemote = false;
    ui->connectButton->setEnabled(true);
    ui->ipAddress->setEnabled(true);
    ui->nickname->setEnabled(true);
    ui->startGameButton->setEnabled(false);

    sock->close();
    updateGui();
}

void JoinNetworkGameWindow::displayError(
        QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
    {
        return;
    }

    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    QMessageBox::information(this, "RummyCube",
            tr("The following error occurred: %1.").arg(sock->errorString()));
}

void JoinNetworkGameWindow::connectClickEvent()
{
    if (ui->nickname->text().length() > 0)
    {
        sock = Network::createClientSocket(
                ui->ipAddress->text(), NETWORK_GAME_PORT);

        sock->waitForConnected();

        if (sock->state() != QTcpSocket::ConnectedState)
        {
            QMessageBox::critical(this, "RummyCube",
                tr("Could not connect to other player.\n"
                "Please make sure that the server is running, "
                "you have the correct address and port %1 is open "
                "in the firewall.").arg(NETWORK_GAME_PORT));
            return;
        }

        connect(sock, SIGNAL(readyRead()), this, SLOT(parseData()));
        connect(sock, SIGNAL(disconnected()), this, SLOT(disconnect()));
        connect(sock, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(displayError(QAbstractSocket::SocketError)));

        ui->connectButton->setEnabled(false);
        ui->ipAddress->setEnabled(false);
        ui->nickname->setEnabled(false);

        Network::write(*sock, "getStatus");
    }
    else
    {
        QMessageBox::information(this, "RummyCube",
                tr("Please enter a nickname before connecting"));
    }
}

void JoinNetworkGameWindow::startGameClickEvent()
{
    Network::write(*sock, "startGame");
    ui->startGameButton->setEnabled(false);
    canStartGameLocal = true;

    startGame();
}

void JoinNetworkGameWindow::cancelClickEvent()
{
    if (canStartGameLocal)
    {
        canStartGameLocal = false;
        ui->startGameButton->setEnabled(true);
        Network::write(*sock, "dontStartGame");
        updateGui();
    }
    else
    {
        if (sock != NULL)
        {
            sock->close();
            delete sock;
            sock = NULL;
        }

        this->reject();
    }
}
