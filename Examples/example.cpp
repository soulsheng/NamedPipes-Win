#include <IPCServer.h>
#include <IPCClient.h>
#include <stdio.h>

class ServerHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %s\n", length, szBuff);
		char szBuffer[128];
		sprintf(szBuffer, "Server Msg");
		this->Send(szBuffer, strlen(szBuffer) + 1);
		
	}
};


class ClientHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %s\n", length, szBuff);
		char szBuffer[128];
		sprintf(szBuffer, "Client Msg");
		this->Send(szBuffer, strlen(szBuffer) + 1);
		
	}
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
			while (server->PeekMessages(100))
			{
				
			}
			Sleep(100);
		}
	}
	else{
		// Client
		printf("Running client:\n");
		ClientHandler *MessageHandler = new ClientHandler();
		ClientHandler *client = IPCClient::CreateIPCClient(MessageHandler, "testpipe");
		if (!client)
		{
			printf("Failed to connect");
			return -1;
		}
		printf("Connected");
		char szBuffer[128];
		sprintf(szBuffer, "I am here!");
		if (client->Send(szBuffer, strlen(szBuffer)+1) == 0)
		{
			printf("Done\n");
		}
		while (1)
		{
			while (client->PeekMessages(100))
			{		
				printf("===========\n");
			}
			Sleep(100);
		}

	}
	return 0;
}
