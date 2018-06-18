#pragma once

#include <RakNetTypes.h>

namespace kge
{
namespace net
{
class Network;
class NetworkEvents
{
public:

	//! Constructor
	NetworkEvents() {}

	//! Destructor
	virtual ~NetworkEvents() {}

	//! Is called when client connects to server
	virtual void OnConnected(RakNet::SystemAddress address) {}

	//! The server is full and can't accept new client
	virtual void OnServerIsFull() {}

	//! We can't connect to server
	virtual void OnConnectionFailed() {}

	//! We lost our connection with server
	virtual void OnConnectionLost() {}

	//! Server disconnected
	virtual void OnDisconnect() {}

	//! When a new clients connects to server
	virtual void OnNewClientConnected(RakNet::Packet *packet) {}

	//! When the network receives a user message
	virtual void OnMessage(RakNet::Packet *packet) {}

	Network		* m_pNetwork;

protected:
	
}; // NetworkEvents

} // net

} // kge
