/*
 * CreateNetworkGameWindow.cpp
 *
 *  Created on: 10.03.2012
 *      Author: Felix Wohlfrom
 *
 */

#include "CreateNetworkGameWindow.h"
#include "ui_CreateNetworkGameWindow.h"

#include "../network/Network.h"
#include "../network/NetworkGameSettings.h"
#include "../game/RummyCubeNetwork.h"

#include <QMessageBox>

CreateNetworkGameWindow::CreateNetworkGameWindow(QWidget* parent) :
    QDialog(parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint
		| Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::CreateNetworkGameWindow), game(NULL),
    connectedPlayer(), canStartGameRemote(false), canStartGameLocal(false)
{
    // Initialize window
    ui->setupUi(this);

    ui->ipAddress->setText(Network::getNetworkAddresses());

    // Server socket creation is done within showEvent() method
    // to allow closing the window if server creation fails
}

CreateNetworkGameWindow::~CreateNetworkGameWindow()
{
    delete ui;
}

RummyCubeNetwork* CreateNetworkGameWindow::getGame()
{
    return game;
}

void CreateNetworkGameWindow::sendSettings()
{
    for (std::map<QTcpSocket*, QString>::iterator elem(connectedPlayer.begin());
            elem != connectedPlayer.end();
            ++elem)
    {
        Network::write(*elem->first, "settings");
        Network::write(*elem->first, ui->stonesAtBeginning->text());
        Network::write(*elem->first, ui->sumAtBeginning->text());
        Network::write(*elem->first, QString::number(ui->stonesInOneRow->isChecked()));
        if (ui->timeLimitEnabled->isChecked())
        {
            Network::write(*elem->first, QString::number(ui->timeLimitValue->value()));
        }
        else
        {
            Network::write(*elem->first, "-1");
        }
    }

    // If we send settings, the remote player first needs to confirm
    // the new settings
    canStartGameRemote = false;
}

void CreateNetworkGameWindow::showSettingsPanel()
{
    ui->statusDisplay->setCurrentIndex(0);
}

void CreateNetworkGameWindow::showWaitingPanel()
{
    ui->statusDisplay->setCurrentIndex(1);
}

void CreateNetworkGameWindow::updateGui()
{
    if (canStartGameLocal)
    {
        showWaitingPanel();
    }
    else
    {
        showSettingsPanel();

        if (connectedPlayer.size() == 0)
        {
            ui->startGameButton->setEnabled(false);
        }
        else
        {
            ui->startGameButton->setEnabled(true);
        }

        QString playerNames = "";
        for (std::map<QTcpSocket*, QString>::iterator elem(connectedPlayer.begin());
                elem != connectedPlayer.end();
                ++elem)
        {
            if (playerNames.length() > 0)
            {
                playerNames += ";";
            }

            playerNames += elem->second;
        }
        ui->connectedPlayer->setText(playerNames);
    }
}

void CreateNetworkGameWindow::updateTimeLimitLabel()
{
    int min = ui->timeLimitValue->value() / 60;
    int sec = ui->timeLimitValue->value() % 60;
    ui->timeLimitLabel->setText(tr("%1:%2 minutes").arg(min).arg(sec, 2, 10, QLatin1Char('0')));
}

void CreateNetworkGameWindow::startGame()
{
    if (canStartGameLocal && canStartGameRemote)
    {
        // Game settings
        NetworkGameSettings settings;
        settings.stonesAtBeginning = ui->stonesAtBeginning->value();
        settings.sumAtBeginning = ui->sumAtBeginning->value();
        settings.stonesInOneRow = ui->stonesInOneRow->isChecked();
        if (ui->timeLimitEnabled->isChecked())
        {
            settings.timeLimit = ui->timeLimitValue->value();
        }
        else
        {
            settings.timeLimit = -1;
        }

        // Create game
        game = new RummyCubeNetwork(settings, connectedPlayer);

        for (std::map<QTcpSocket*, QString>::iterator elem(connectedPlayer.begin());
            elem != connectedPlayer.end();
            ++elem)
        {
            elem->first->disconnect(this);
        }

        this->accept();
    }

    updateGui();
}

void CreateNetworkGameWindow::parseData()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    while (sock->bytesAvailable() > 0)
    {
        QString msg = Network::read(*sock);

        if (msg == "getStatus")
        {
            if (connectedPlayer.size() == 1)
            {
                Network::write(*sock, "connected");
            }
            else
            {
                Network::write(*sock, "toManyPlayers");
            }
            return;
        }

        if (msg == "startGame")
        {
            canStartGameRemote = true;
            startGame();
            return;
        }
        else if (msg == "dontStartGame")
        {
            canStartGameRemote = false;
            canStartGameLocal = false;
            updateGui();
            return;
        }
        else if (msg == "version")
        {
            // Check for protocol and version
            QString version = Network::read(*sock);
            if (version != "RummyCube 1")
            {
                Network::write(*sock, "wrongVersion");
                return;
            }
        }
        else if (msg == "nickname")
        {
            connectedPlayer[sock] = Network::read(*sock);

            // If we receive the nickname this means that the protocol version
            // is correct
            sendSettings();
            updateGui();
        }
    }
}

void CreateNetworkGameWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    // This centers the error message on failures of server creation
    QCoreApplication::processEvents();

    // Create the server socket for network game
    server = Network::createServerSocket(NETWORK_GAME_PORT);

    if (!server->isListening())
    {
        QMessageBox serverCreationError(this);
        serverCreationError.setWindowTitle(QString("RummyCube"));
        serverCreationError.setText(tr("Could not start server. %1.").arg(server->errorString()));
        serverCreationError.setStandardButtons(QMessageBox::Ok);
        serverCreationError.setIcon(QMessageBox::Critical);
        serverCreationError.exec();
        server->close();
        delete server;
        server = NULL;
        // We close here the QMetaObject to close the window since the usual
        // this->reject() call does not work in this early phase.
        QMetaObject::invokeMethod(this, "reject", Qt::QueuedConnection);
        return;
    }

    connect(server, SIGNAL(newConnection()),
              this, SLOT(acceptConnectionEvent()));
}

void CreateNetworkGameWindow::acceptConnectionEvent()
{
    QTcpSocket* sock = server->nextPendingConnection();
    connectedPlayer.insert(std::pair<QTcpSocket*, QString>(sock, ""));

    connect(sock, SIGNAL(readyRead()),
            this, SLOT(parseData()));
    connect(sock, SIGNAL(disconnected()),
            this, SLOT(disconnect()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void CreateNetworkGameWindow::disconnect()
{
    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());

    connectedPlayer.erase(sock);
    canStartGameRemote = false;
    canStartGameLocal = false;
    sock->close();

    server->resumeAccepting(); // Continue to accept new connections
    updateGui();
}

void CreateNetworkGameWindow::displayError(
        QAbstractSocket::SocketError socketError)
{
    if (socketError == QTcpSocket::RemoteHostClosedError)
    {
        return;
    }

    QTcpSocket* sock = qobject_cast<QTcpSocket*>(sender());
    QMessageBox::information(this, tr("Network error"),
                             tr("The following error occurred: %1.").arg(sock->errorString()));
}

void CreateNetworkGameWindow::spinCtrlChangedEvent(QString)
{
    this->sendSettings();
}

void CreateNetworkGameWindow::stonesInOneRowClickEvent()
{
    this->sendSettings();
}

void CreateNetworkGameWindow::timeLimitEnabledClickEvent()
{
    ui->timeLimitLabel->setEnabled(ui->timeLimitEnabled->isChecked());
    ui->timeLimitValue->setEnabled(ui->timeLimitEnabled->isChecked());
    this->sendSettings();
}

void CreateNetworkGameWindow::timeLimitSlideEvent(int)
{
    this->updateTimeLimitLabel();
    this->sendSettings();
}

void CreateNetworkGameWindow::startGameClickEvent()
{
    for (std::map<QTcpSocket*, QString>::iterator elem(connectedPlayer.begin());
            elem != connectedPlayer.end();
            ++elem)
    {
        // First send the names of the players
        Network::write(*elem->first, "playerNames");

        int count = connectedPlayer.size() + 1;
        Network::write(*elem->first, QString::number(count));

        // TODO Get game host name dynamically
        Network::write(*elem->first, tr("Game creator"));
        for (std::map<QTcpSocket*, QString>::iterator names(connectedPlayer.begin());
                names != connectedPlayer.end();
                ++names)
        {
            Network::write(*elem->first, names->second);
        }

        Network::write(*elem->first, "startGame");
    }

    // Disable new incomming connections
    server->pauseAccepting();

    ui->startGameButton->setEnabled(false);
    canStartGameLocal = true;

    startGame();
}

void CreateNetworkGameWindow::cancelClickEvent()
{
    if (canStartGameLocal)
    {
        canStartGameLocal = false;
        server->resumeAccepting();
        ui->startGameButton->setEnabled(true);
        for (std::map<QTcpSocket*, QString>::iterator elem(connectedPlayer.begin());
                elem != connectedPlayer.end();
                ++elem)
        {
            Network::write(*elem->first, "dontStartGame");
        }
        updateGui();
    }
    else
    {
        this->reject();
    }
}

void CreateNetworkGameWindow::reject()
{
    // Add the clients first to a list since visual studio doesn't like
    // sockets to be closed while they are keys of a map.
    std::list<QTcpSocket*> clients;
    for (std::map<QTcpSocket*, QString>::iterator elem(connectedPlayer.begin());
            elem != connectedPlayer.end();
            ++elem)
    {
        clients.push_back(elem->first);
    }
    connectedPlayer.clear();

    for (std::list<QTcpSocket*>::iterator elem(clients.begin());
            elem != clients.end();
            ++elem)
    {
        // Disconnect all signals from the socket
        QObject::disconnect((*elem), 0, 0, 0);
        (*elem)->close();
    }

    if (server != NULL)
    {
        server->close();
        delete server;
    }

    QDialog::reject();
}
