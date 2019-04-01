#include <IPCServer.h>
#include <IPCClient.h>
#include <stdio.h>

#define		SIZE_BUFFER	1024*1024*4*4
#define		TIME_INTERVAL	50

class ServerHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %s\n", length, szBuff);
		sprintf_s(szBuffer, SIZE_BUFFER, "Server Msg");
		IPC->Send(szBuffer, strlen(szBuffer) + 1);
		
	}
	char szBuffer[SIZE_BUFFER];
};


class ClientHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %s\n", length, szBuff);
		sprintf_s(szBuffer, SIZE_BUFFER, "Client Msg");
		IPC->Send(szBuffer, strlen(szBuffer) + 1);
		
	}
	char szBuffer[SIZE_BUFFER];
};

int main(int argc, char *argv[])
{
	if (argc >1)
	{
		// Server 
		printf("Running server:\n");
		ServerHandler *MessageHandler = new ServerHandler();
		IPCServer* server = IPCServer::CreateIPCServer(MessageHandler, "testpipe");
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
		IPCClient *client = IPCClient::CreateIPCClient(MessageHandler, "testpipe");
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
