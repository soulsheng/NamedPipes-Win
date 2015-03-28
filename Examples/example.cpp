#include <IPCServer.h>
#include <IPCClient.h>
#include <stdio.h>

class TestSRVIPC : public IPCMessageHandler
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


class TestCLTIPC : public IPCMessageHandler
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
		printf("Running server:\n");
		TestSRVIPC* SRVIPC = IPCServer::CreateIPCServer<TestSRVIPC>("testpipe");
		printf("Waiting for incoming connections!\n");
		while (1)
		{
			while (SRVIPC->PeekMessages(100))
			{
				printf("===========\n");
			}
		}
	}
	else{
		printf("Running client:\n");
		TestCLTIPC *CLTIPC = IPCClient::CreateIPCClient<TestCLTIPC>("testpipe");
		if (!CLTIPC)
		{
			printf("Failed to connect");
			while (1){};
		}
		printf("Connected");
		char szBuffer[128];
		sprintf(szBuffer, "I am here!");
		if (CLTIPC->Send(szBuffer, strlen(szBuffer)+1) == 0)
		{
			printf("Done\n");
		}
		while (1)
		{
			while (CLTIPC->PeekMessages(100))
			{		
				printf("===========\n");
			}
		}

	}

}