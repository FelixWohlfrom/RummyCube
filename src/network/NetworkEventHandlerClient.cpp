/*
 * NetworkEventHandlerClient.cpp
 *
 *  Created on: 05.07.2012
 *      Author: Felix Wohlfrom
 */

#include "NetworkEventHandlerClient.h"

#include "Network.h"
#include "../game/player/NetworkPlayerClient.h"

BEGIN_EVENT_TABLE(NetworkEventHandlerClient, wxEvtHandler)
	EVT_SOCKET(Network::SOCKET_EVENT, NetworkEventHandlerClient::OnSocketEvent)
END_EVENT_TABLE()

NetworkEventHandlerClient::NetworkEventHandlerClient(wxSocketBase& sock) :
	eventReceivers()
{
	// Start listening to events
	sock.SetEventHandler(*this, Network::SOCKET_EVENT);
	sock.SetNotify(wxSOCKET_LOST_FLAG);
	sock.Notify(true);
};

void NetworkEventHandlerClient::addEventReceiver(NetworkPlayerClient* newReceiver)
{
	eventReceivers.push_back(newReceiver);
}

void NetworkEventHandlerClient::removeEventReceiver(NetworkPlayerClient* toRemove)
{
	eventReceivers.remove(toRemove);
}

void NetworkEventHandlerClient::OnSocketEvent(wxSocketEvent& e)
{
	for (std::list<NetworkPlayerClient*>::iterator recvr(eventReceivers.begin()); recvr != eventReceivers.end(); ++recvr)
	{
		(*recvr)->OnSocketEvent(e);
	}
}
