/*
 * NetworkPlayer.h
 *
 *  Created on: 22.04.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORKPLAYER_H_
#define NETWORKPLAYER_H_

#include "OpponentPlayer.h"

#include "../../network/Network.h"

#include <QObject>
#include <QTcpSocket>

/**
 * The base class for all network players. It will parse the gameboard status
 * and other status updates from other players. Inheritance has to be QObject
 * first to make the moc tool happy.
 */
class NetworkPlayer: public QObject, public OpponentPlayer
{
    Q_OBJECT

    public:
        /**
         * Creates a new opponent player for network game on client.
         *
         * @param name The name of the player
         * @param stoneManager The stone manager on which the player can play
         * @param sock The socket on which the network player should work
         * @param stonesAtBeginning The initial stones that the player can take
         * @param sumAtBeginning The sum that is needed for the player to play out first time
         * @param stonesInOneRow If the sum needs to be in one row or can be split in multiple rows
         */
        NetworkPlayer(QString name, StoneManager& stoneManager, QTcpSocket& sock,
                int stonesAtBeginning, int sumAtBeginning, bool stonesInOneRow);

        /**
         * Virtual destructor to avoid warnings.
         */
        virtual ~NetworkPlayer();

        /**
         * Lets the player play.
         */
        virtual void play() = 0;

        /**
         * Returns if the player has won.
         *
         * @return If the player has won or not
         */
        bool hasWon();

        /**
         * Returns the socket of the current player.
         *
         * @return The socket
         */
        QTcpSocket& getSocket();

    protected:
        /**
         * If the player has won the game or not.
         */
        bool playerHasWon;

        /**
         * Stores if the player finished his round or not.
         */
        bool roundFinished;

        /**
         * The socket on which the network player works.
         */
        QTcpSocket& sock;

    signals:
        /**
         * Emitted if an error apperead during network connection.
         *
         * @param error The error message.
         */
        void connectionError(QString error);

    private slots:
        /**
         * Parses the data received from socket associated with player.
         */
        void parseData();

        /**
         * Event handler if the opponent disconnected.
         */
        void opponentDisconnected();
};

#endif /* NETWORKPLAYER_H_ */
