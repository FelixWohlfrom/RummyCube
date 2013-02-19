/*
 * Network.h
 *
 *  Created on: 16.03.2012
 *      Author: Felix Wohlfrom
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#ifdef _WIN32
	// #include <winsock2.h> // This just works if compiled with gcc
	#include <winsock.h>
#else
	#include <arpa/inet.h>
#endif

#include <wx/socket.h>
#include <wx/string.h>

#include "../game/stones/StoneManager.h"

class Network
{
	public:
		/**
		 * More self explaining values for ipv4 and ipv6
		 */
		#if wxUSE_IPV6
			enum Protocol { IPV4 = AF_INET, IPV6 = AF_INET6 };
		#else
			enum Protocol { IPV4 = AF_INET };
		#endif

		/**
		 * Magic value for end of socket stream
		 */
		static const int SOCKET_STREAM_CLOSED = -1;

		/**
		 * The id for socket events.
		 */
		static const int SOCKET_EVENT = 4711;

		/**
		 * Creates a new server socket
		 *
		 * @param port The port which the server should listen to
		 */
		static wxSocketServer* createServerSocket(int port);

		/**
		 * Creates a new client socket and connects it to given host and port
		 *
		 * @param host The host to which the client should connect to
		 * @param port The port to which the client should connect to
		 */
		static wxSocketClient* createClientSocket(wxString host, int port);

		/**
		 * Writes the given string to given socket
		 *
		 * @param sock The socket to write to
		 * @param toWrite The string that should be written
		 */
		static void write(wxSocketBase& sock, wxString toWrite);

		/**
		 * Reads a message from given socket
		 *
		 * @param sock The socket from which should be read
		 * @return The string read from socket
		 */
		static wxString read(wxSocketBase& sock);

		/**
		 * Reads the board status from given socket
		 *
		 * @param sock The socket from which the board status should be read
		 * @param stonemanager The stone manager in which the read board status should be stored
		 */
		static void readBoardStatus(wxSocketBase& sock, StoneManager& stonemanager);

		/**
		 * Writes the board status to given socket.
		 * Just writes the stones which are on heap and board
		 *
		 * @param sock The socket to which the board status should be written.
		 * @param stonemanager The stone manager from which the board status should be written
		 */
		static void writeBoardStatus(wxSocketBase& sock, StoneManager& stonemanager);

		/**
		 * Returns the network address(es) of the current pc using the given protocol
		 * 127.0.0.1 (localhost) is not included
		 *
		 * @param protocol The used protocol. Default is ipv4
		 * @return A string containing the network address(es) of the current pc, separated by semicolon
		 */
		static wxString getNetworkAddresses(Protocol protocol = IPV4);
};

// This is needed for compilation in visual studio
#ifdef _WIN32
	#include "wx/msw/winundef.h"
#endif

#endif /* NETWORK_H_ */
