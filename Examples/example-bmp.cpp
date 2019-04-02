#include <IPCServer.h>
#include <IPCClient.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include "simpleBMP.h"

#define		SIZE_BUFFER_SEND_SERVER	(1024*1024*4)
#define		SIZE_BUFFER_SEND_CLIENT	(256)		//(>4*7*5) float[7]*5

#define		TIME_INTERVAL	50

class ServerHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %c, t %ld\n", length, szBuff[0], GetTickCount());
		int	detect[5];
		for (int i = 0; i < 5; i++)
		{
			detect[i] = *(int*)(szBuff+4*i);
			printf("%d, ", detect[i]);
		}
		printf("\n");	
	}
};


class ClientHandler : public IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
	{
		printf("Received[%d]: %c, t %ld\n", length, szBuff[0], GetTickCount());
		int  szByteIn = 0;
		int nWidth = *(int*)(szBuff + szByteIn);

		szByteIn += sizeof(int);
		int nHeight = *(int*)(szBuff + szByteIn);

		printf("(w, h)=(%d, %d)\n", nWidth, nHeight);		
	}	
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

		// prepare data
		ClImgBMP simpleBMP;
		char* szBuffer = new char[SIZE_BUFFER_SEND_SERVER];
		memset(szBuffer, 0, SIZE_BUFFER_SEND_SERVER);
		simpleBMP.LoadImage("1.bmp");

		int nWidth = simpleBMP.Width();
		int nHeight = simpleBMP.Height();

		memcpy_s(szBuffer, SIZE_BUFFER_SEND_SERVER, &nWidth, sizeof(int));
		int szByteOut = sizeof(int);
		memcpy_s(szBuffer + szByteOut, SIZE_BUFFER_SEND_SERVER - szByteOut, &nHeight, sizeof(int));
		szByteOut += sizeof(int);

		while (1)
		{
			while (server->PeekMessages(TIME_INTERVAL))
			{
				// ready to send image
				server->Send(szBuffer, SIZE_BUFFER_SEND_SERVER);

			}
			Sleep(TIME_INTERVAL*2);
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
		char szBuffer0[128];
		sprintf_s(szBuffer0, "I am here!");
		if (client->Send(szBuffer0, strlen(szBuffer0)+1) == 0)
		{
			printf("Done\n");
		}

		// prepare data 
		int	detect[5];
		char* szBuffer = new char[SIZE_BUFFER_SEND_CLIENT];
		memset(szBuffer, 0, SIZE_BUFFER_SEND_CLIENT);
		std::ifstream file("1.txt", std::ios::in);
		for (int i = 0; i < 5; i++)
			file >> detect[i];
		file.close();
		memcpy(szBuffer, &detect[0], sizeof(int) * 5);

		while (1)
		{
			while (client->PeekMessages(TIME_INTERVAL))
			{		
				printf("===========\n");

				// ready to send struct 
				sprintf_s(szBuffer + sizeof(int) * 5, 128, "Client Msg, t %ld", GetTickCount());
				client->Send(szBuffer, sizeof(int) * 5 + 128);
			}
			Sleep(TIME_INTERVAL);
		}

	}
	return 0;
}
