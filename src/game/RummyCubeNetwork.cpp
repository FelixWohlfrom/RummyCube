/*
 * RummyCubeNetwork.cpp
 *
 *  Created on: 08.01.2013
 *      Author: Felix Wohlfrom
 */

#include "RummyCubeNetwork.h"

#include "player/HumanPlayerServer.h"
#include "player/HumanPlayerClient.h"
#include "player/NetworkPlayerServer.h"
#include "player/NetworkPlayerClient.h"

RummyCubeNetwork::RummyCubeNetwork(NetworkGameSettings& settings, std::map<wxSocketBase*, wxString> opponentPlayers) :
	RummyCube(), eventHandler(NULL), sock(NULL)
{
	// Create the players
	std::list<wxSocketBase*> allSockets;
	for (std::map<wxSocketBase*, wxString>::iterator elem(opponentPlayers.begin()); elem != opponentPlayers.end(); ++elem)
	{
		allSockets.push_back(elem->first);
	}

	// Human player
	this->humanPlayer = new HumanPlayerServer(stoneManager, settings.stonesInFirstRound, settings.sumInFirstRound, settings.stonesInOneRow, allSockets);

	// Opponent players
	for (std::map<wxSocketBase*, wxString>::iterator elem(opponentPlayers.begin()); elem != opponentPlayers.end(); ++elem)
	{
		this->opponentPlayers.push_back(new NetworkPlayerServer(elem->second, stoneManager, *elem->first, allSockets));
	}
}

RummyCubeNetwork::RummyCubeNetwork(NetworkGameSettings& settings, wxSocketBase* sock, wxString humanPlayer, std::list<wxString> opponentPlayerNames) :
	RummyCube(), eventHandler(new NetworkEventHandlerClient(*sock)), sock(sock)
{
	timer = new CountdownTimer(settings.timeLimit);

	// Create human player
	this->humanPlayer = new HumanPlayerClient(stoneManager, *sock, settings.stonesInFirstRound, settings.sumInFirstRound, settings.stonesInOneRow);

	// Create opponent players
	std::deque<OpponentPlayer*>::iterator lastElemBeforeCurrPlayer; // Needed for the step after, list resorting
	bool skip = false;
	for (std::list<wxString>::iterator it(opponentPlayerNames.begin()); it != opponentPlayerNames.end(); ++it)
	{
		if ((*it) == humanPlayer)
		{
			skip = true;
			lastElemBeforeCurrPlayer = this->opponentPlayers.end() - 1;
			continue;
		}

		this->opponentPlayers.push_back(new NetworkPlayerClient((*it), stoneManager, *sock, *eventHandler, skip));
	}

	// Move opponentPlayers that are after current player to the beginning of list
	while (opponentPlayers.back() != *lastElemBeforeCurrPlayer)
	{
		this->opponentPlayers.push_front(opponentPlayers.back());
		this->opponentPlayers.pop_back();
	}
}

RummyCubeNetwork::~RummyCubeNetwork()
{
	delete timer;
	delete humanPlayer;
	for (std::deque<OpponentPlayer*>::iterator player(opponentPlayers.begin()); player != opponentPlayers.end(); ++player)
	{
		delete (*player);
	}
	wxDELETE(eventHandler);
	wxDELETE(sock);
}
