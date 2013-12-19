#pragma once

#include "../kgedef.h"

namespace RakNet
{
	class RakPeerInterface;

} // RakNet

namespace kge
{
namespace net
{
class NetworkEvents;

class KGELIB_API Network
{
public:
	
	//! Constructor
	Network();
	
	//! Destructor
	~Network();

	//! Creates a server that clients can connect to
	/*!
		\param maxPlayers The maximum number of players can connect to this server.
		\param port The server port that listen to and clients connect to this port.
	 */
	void CreateServer(int maxPlayers, int port);

	//! Connects the client to a server
	/*!
		\param serverip The server IP that client wants to connect.
		\param port The server port that client wants to connect.
	 */
	void ConnectToServer(char* serverip, int port);

	//! Runs the network system and process packets.
	void Run();

	//! Sets the NetworkEvents pointer to send events to it.
	void SetNetworkEvents(NetworkEvents* p);

	//! Returns true if this connection is acting as server
	bool GetIsServer() {return m_bIsServer;}

	//! Is it connected to server or not
	bool GetIsConnected() {return m_bIsConnected;}

	//! Returns the RakPeerInterface pointer.
	RakNet::RakPeerInterface* GetRakPeer() {return m_pPeer;}
	
protected:

	RakNet::RakPeerInterface		*	m_pPeer;
	bool								m_bIsServer,
										m_bIsConnected,
										m_bIsServerChanged;
	NetworkEvents					*	m_pEvents;
		
}; // Network

} // net

} // kge
