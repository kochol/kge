#include "../../include/kge/net/Network.h"
#include "../../include/kge/net/NetworkEvents.h"
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <iostream>

namespace kge
{
namespace net
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Network::Network(): m_bIsServer(false), m_bIsConnected(false), m_pEvents(NULL),
		m_bIsServerChanged(false)
	{
		m_pPeer = RakNet::RakPeerInterface::GetInstance();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Network::~Network()
	{
		m_pPeer->Shutdown(0);
		RakNet::RakPeerInterface::DestroyInstance(m_pPeer);
			
	} // Destructor

	//------------------------------------------------------------------------------------
	// Creates a server that clients can connect to
	//------------------------------------------------------------------------------------
	void Network::CreateServer( int maxPlayers, int port )
	{
		RakNet::SocketDescriptor sd(port, 0);
		m_pPeer->Startup(maxPlayers, &sd, 1);
		m_bIsServer = true;
		m_bIsConnected = true;
		m_pPeer->SetMaximumIncomingConnections(maxPlayers);

	} // CreateServer

	//------------------------------------------------------------------------------------
	// Connects the client to a server
	//------------------------------------------------------------------------------------
	void Network::ConnectToServer( char* serverip, int port )
	{
		RakNet::SocketDescriptor sd;
		if (m_bIsConnected)
		{
			m_bIsServerChanged = true;
			m_pPeer->Shutdown(0);
		}
		m_pPeer->Startup(1, &sd, 1);
		m_bIsServer = false;
		m_pPeer->Connect(serverip, port, 0, 0);

	} // ConnectToServer

	//------------------------------------------------------------------------------------
	// Runs the network system and process packets.
	//------------------------------------------------------------------------------------
	void Network::Run()
	{
		RakNet::Packet *packet;

		for (packet=m_pPeer->Receive(); packet; m_pPeer->DeallocatePacket(packet), packet=m_pPeer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
				if (m_pEvents)
					m_pEvents->OnConnected(packet->systemAddress);
				m_bIsConnected = true;
				break;					
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				if (m_pEvents)
					m_pEvents->OnNewClientConnected(packet);
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				if (m_pEvents)
					m_pEvents->OnServerIsFull();
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (m_bIsServer)
				{
					printf("A client has disconnected.\n");
				} 
				else 
				{
					printf("We have been disconnected.\n");
					m_bIsConnected = false;
					if (m_pEvents)
						m_pEvents->OnDisconnect();
				}
				break;
			case ID_CONNECTION_LOST:
				if (m_bIsServer)
				{
					printf("A client lost the connection.\n");
				} 
				else 
				{
					printf("Connection lost.\n");
					m_bIsConnected = false;
					if (m_pEvents)
						m_pEvents->OnConnectionLost();
				}
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Can't connect to server.\n");
				if (m_pEvents)
					m_pEvents->OnConnectionFailed();
				break;
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				if (m_pEvents)
				{
					m_pEvents->OnMessage(packet);
					if (m_bIsServerChanged)
						return;
				}
				break;
			}
		}

	} // Run

	//------------------------------------------------------------------------------------
	// Sets the NetworkEvents pointer to send events to it.
	//------------------------------------------------------------------------------------
	void Network::SetNetworkEvents( NetworkEvents* p )
	{
		m_pEvents = p;
		m_pEvents->m_pNetwork = this;

	} // SetNetworkEvents

} // net

} // kge
