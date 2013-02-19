/*
 * Network.cpp
 *
 *  Created on: 16.03.2012
 *      Author: Felix Wohlfrom
 */

#include "Network.h"

#ifndef _WIN32
	#include <netdb.h>
	#include <ifaddrs.h>
#endif

#include <set>
#include <vector>

#include <wx/string.h>
#include <wx/datstrm.h>
#include <wx/sckstrm.h>

#define MAX_MSG_SIZE 10000

// This method tests, if the previous stone of next stone is same as original stone
// Sometimes happens on network games -.-
bool allStonesNextPrevIsThisStone(StoneManager& stonemanager)
{
	std::vector<Gamestone*> stones = stonemanager.getStones();
	for (std::vector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
	{
		if ((*stone)->getNext() != NULL &&
				(*stone)->getNext()->getPrev() != (*stone))
		{
			return false;
		}

		if ((*stone)->getPrev() != NULL &&
				(*stone)->getPrev()->getNext() != (*stone))
		{
			return false;
		}
	}

	return true;
}

wxSocketServer* Network::createServerSocket(int port)
{
	wxIPV4address addr;
	addr.Service(port);
	return new wxSocketServer(addr, wxSOCKET_WAITALL);
}

wxSocketClient* Network::createClientSocket(wxString host, int port)
{
	wxSocketClient* sock = new wxSocketClient();
	wxIPV4address addr;
	addr.Hostname(host);
	addr.Service(port);
	sock->Connect(addr, false);
	return sock;
}

void Network::write(wxSocketBase& sock, wxString toWrite)
{
	const wxChar* msg;
	msg = toWrite.c_str();
	size_t len = (wxStrlen(msg) + 1) * sizeof(wxChar);
	sock.WriteMsg(msg, len);
}

wxString Network::read(wxSocketBase& sock)
{
	wxChar* buf = new wxChar[MAX_MSG_SIZE];
	wxUint32 len;
	wxString msg(_T(""));

	len = sock.ReadMsg(buf, MAX_MSG_SIZE * sizeof(wxChar)).LastCount();
	if (len > 0)
	{
		msg.Printf(_T("%s"), buf);
	}

	delete[] buf;

	return msg;
}

void Network::readBoardStatus(wxSocketBase& sock, StoneManager& stonemanager)
{
	// Receive the board status from other player
	int value = 0;
	Gamestone* stone;
	wxSocketInputStream sockInStream(sock);
	wxDataInputStream in(sockInStream);

	// Check if stones need to be moved to ai holder
	std::vector<Gamestone*> gamestones = stonemanager.getStones();
	std::set<Gamestone*> toAiHolder;
	for (std::vector<Gamestone*>::iterator stone(gamestones.begin()); stone != gamestones.end(); ++stone)
	{
		toAiHolder.insert(*stone);
	}

	// Read stones from opponent
	while (value != Network::SOCKET_STREAM_CLOSED)
	{
		in >> value;
		if (value > 0)
		{
			stone = stonemanager.getStoneFromInt(value);
			in >> stone;

			toAiHolder.erase(stone);
		}
	}

	// Move the remaining stones to ai holder, if not own stones
	for (std::set<Gamestone*>::iterator stone(toAiHolder.begin()); stone != toAiHolder.end(); ++stone)
	{
		if ((*stone)->getParent() != Gamestone::HOLDER)
		{
			(*stone)->setParent(Gamestone::AIHOLDER);
			(*stone)->storeStone();
		}
	}

	wxASSERT(allStonesNextPrevIsThisStone(stonemanager));
}

void Network::writeBoardStatus(wxSocketBase& sock, StoneManager& stonemanager)
{
	wxASSERT(allStonesNextPrevIsThisStone(stonemanager));

	write(sock, _T("boardStatus"));

	wxSocketOutputStream sockOutStream(sock);
	wxDataOutputStream out(sockOutStream);

	std::vector<Gamestone*> stones = stonemanager.getStones();
	for (std::vector<Gamestone*>::iterator stone(stones.begin()); stone != stones.end(); ++stone)
	{
		if ((*stone)->getParent() == Gamestone::HEAP || (*stone)->getParent() == Gamestone::BOARD)
		{
			out << (*stone);
		}
	}
	out << Network::SOCKET_STREAM_CLOSED;
	sockOutStream.Close();
}

wxString Network::getNetworkAddresses(Protocol protocol)
{
	wxString addresses;

	// Windows code from http://tangentsoft.net/wskfaq/examples/ipaddr.html
	#ifdef _WIN32
		if (protocol == IPV4) {};	// avoid warning

		WSAData wsaData;
		if (WSAStartup(MAKEWORD(1, 1), &wsaData) == 0)
		{
			char hostname[80];
			if (gethostname(hostname, sizeof(hostname)) != SOCKET_ERROR)
			{
				struct hostent* phe = gethostbyname(hostname);
				if (phe != 0)
				{
					for (int i = 0; phe->h_addr_list[i] != 0; ++i)
					{
						struct in_addr addressBuffer;
						memcpy(&addressBuffer, phe->h_addr_list[i], sizeof(struct in_addr));

						wxString address = wxString::FromAscii(inet_ntoa(addressBuffer));
						if (addresses.length() > 0)
						{
							addresses += wxString(_T("; "));
						}
						addresses += address;
					}
				}
			}
		}

		WSACleanup();

	// Unix code from http://stackoverflow.com/questions/212528/linux-c-get-the-ip-address-of-local-computer
	#else
		struct ifaddrs* ifAddrStruct(NULL);
		struct ifaddrs* ifa(NULL);
		void* tmpAddrPtr(NULL);

		getifaddrs(&ifAddrStruct);

		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
		{
			if (ifa->ifa_addr->sa_family != protocol) continue;

			wxString address;
			if (ifa->ifa_addr->sa_family == IPV4)
			{
				tmpAddrPtr = &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(protocol, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

				address = wxString::FromAscii(addressBuffer);
			}
			#if wxUSE_IPV6
				else if (ifa->ifa_addr->sa_family == IPV6)
				{
					tmpAddrPtr = &((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr;
					char addressBuffer[INET6_ADDRSTRLEN];
					inet_ntop(protocol, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);

					address = wxString::FromAscii(addressBuffer);
				}
			#endif

			if (address != _T("127.0.0.1") && address != _T("::1"))
			{
				if (addresses.length() > 0)
				{
					addresses += wxString(_T(";"));
				}
				addresses += address;
			}
		}

		if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
	#endif

	return addresses;
}

#undef MAX_MSG_SIZE
