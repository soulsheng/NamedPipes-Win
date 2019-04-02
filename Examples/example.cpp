#include <IPCServer.h>
#include <IPCClient.h>
#include <stdio.h>
#include <time.h>

#define		SIZE_BUFFER_SEND_SERVER	(1024*1024*4)
#define		SIZE_BUFFER_SEND_CLIENT	(256)		//(>4*7*5) float[7]*5

#define		TIME_INTERVAL	50

class ServerHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %c, t %ld\n", length, szBuff[0], GetTickCount());
		sprintf_s(szBuffer, SIZE_BUFFER_SEND_SERVER, "Server Msg, t %ld", GetTickCount());
		IPC->Send(szBuffer, SIZE_BUFFER_SEND_SERVER);
		
	}
	char szBuffer[SIZE_BUFFER_SEND_SERVER];
};


class ClientHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %c, t %ld\n", length, szBuff[0], GetTickCount());
		sprintf_s(szBuffer, SIZE_BUFFER_SEND_CLIENT, "Client Msg, t %ld", GetTickCount());
		IPC->Send(szBuffer, SIZE_BUFFER_SEND_CLIENT);
		
	}
	char szBuffer[SIZE_BUFFER_SEND_CLIENT];
};

int main(int argc, char *argv[])
{
	if (argc >1)
	{
		// Server 
		printf("Running server:\n");
		ServerHandler *MessageHandler = new ServerHandler();
		IPCServer* server = IPCServer::CreateIPCServer(MessageHandler, "testpipe", SIZE_BUFFER_SEND_SERVER);
		printf("Waiting for incoming connections!\n");
		while (1)
		{
			while (server->PeekMessages(TIME_INTERVAL))
			{
				
			}
			Sleep(TIME_INTERVAL);
		}
	}
	else{
		// Client
		printf("Running client:\n");
		ClientHandler *MessageHandler = new ClientHandler();
		IPCClient *client = IPCClient::CreateIPCClient(MessageHandler, "testpipe", SIZE_BUFFER_SEND_SERVER);
		if (!client)
		{
			printf("Failed to connect");
			return -1;
		}
		printf("Connected");
		char szBuffer[128];
		sprintf_s(szBuffer, "I am here!");
		if (client->Send(szBuffer, strlen(szBuffer)+1) == 0)
		{
			printf("Done\n");
		}
		while (1)
		{
			while (client->PeekMessages(TIME_INTERVAL))
			{		
				printf("===========\n");
			}
			Sleep(TIME_INTERVAL);
		}

	}
	return 0;
}
