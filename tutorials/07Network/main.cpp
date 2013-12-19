#include "../../Include/net/Network.h"
#include "../../Include/net/NetworkEvents.h"
#include "../../Include/net/NetworkMessages.h"
#include <iostream>
#include <RakNetTypes.h>
#include <RakString.h>
#include <BitStream.h>
#include <RakPeerInterface.h>

#pragma comment(lib, "../../bin/debug/RakNetKGE.lib")
#pragma comment(lib, "RakNetDLLDebug.lib")

kge::net::Network	network;
bool				g_bRun = true;

enum MyMessages
{
	EMM_Message = kge::net::ENM_UserNetworkMessage
};

// Create a class that inherited from NetworkEvents class for listening to Network events
class MyNetworkEvents: public kge::net::NetworkEvents
{
public:

	//! Is called when client connects to server
	void OnConnected(RakNet::SystemAddress address) 
	{
		// Use a BitStream to write a custom user message
		// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)EMM_Message);
		bsOut.Write("Hello world");
		m_pNetwork->GetRakPeer()->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,address,false);
	}

	//! The server is full and can't accept new client
	void OnServerIsFull() {g_bRun = false;}

	//! We can't connect to server
	void OnConnectionFailed() 
	{
		printf("Can't find the server.\n");
		g_bRun = false;
	}

	//! We lost our connection with server
	void OnConnectionLost() 
	{
		g_bRun = false;
	}

	//! Server disconnected
	virtual void OnDisconnect()
	{
		g_bRun = false;
	}

	//! When the network receives a user message
	virtual void OnMessage(RakNet::Packet *packet) 
	{
		switch (packet->data[0])
		{
		case EMM_Message:
			RakNet::RakString rs;
			RakNet::BitStream bsIn(packet->data,packet->length,false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			bsIn.Read(rs);
			printf("%s\n", rs.C_String());
			break;
		}
	} // OnMessage

};

int main()
{
	char str[512];

	// Ask the user if they want to connect as client or want to create a server
	printf("(C)lient or (S)erver?\n");
	gets(str);

	if ((str[0]=='c')||(str[0]=='C'))
	{
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		gets(str);
		if (str[0]==0){
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		// Connect the client to server
		network.ConnectToServer(str, 5555);
	} 
	else 
	{
		// Create a server that accepts 5 player on port 60000
		printf("Starting the server.\n");
		network.CreateServer(5, 5555);
	}

	// Listen to network events
	MyNetworkEvents mne;
	// Add our network event listener to our network object
	network.SetNetworkEvents(&mne);

	while(g_bRun)
		network.Run();

	return 0;

} // main
