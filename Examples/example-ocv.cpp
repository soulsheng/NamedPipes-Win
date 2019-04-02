#include <IPCServer.h>
#include <IPCClient.h>
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define		SIZE_BUFFER_SEND_SERVER	(1024*1024*4)
#define		SIZE_BUFFER_SEND_CLIENT	(256)		//(>4*7*5) float[7]*5
#define		IMAGE_WIDTH		852
#define		IMAGE_HEIGHT	640
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
			detect[i] = *(int*)(szBuff + 4 * i);
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
		printf("Client Received a image[%d]: %c, t %ld\n", length, szBuff[0], GetTickCount());
		cv::Mat	img(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC3);
		img.data = (uchar*)szBuff;
		cv::imshow("Receive img", img);
		cv::waitKey();		
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

		// server prepare data
		cv::Mat img = cv::imread("D:\\file\\code\\git\\tool\\NamedPipes-Win\\Examples\\2.bmp", -1);

		if (img.empty())
			return -1;

		while (1)
		{
			while (server->PeekMessages(TIME_INTERVAL))
			{
				// server send data
				int nSizeBuffer = img.rows * img.step;
				server->Send((char*)img.data, nSizeBuffer);
				printf("server send a image");
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
		char szBuffer0[128];
		sprintf_s(szBuffer0, "I am here!");
		if (client->Send(szBuffer0, strlen(szBuffer0)+1) == 0)
		{
			printf("Done\n");
		}

		// client prepare data
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

				// client send data
				sprintf_s(szBuffer + sizeof(int) * 5, 128, "Client Msg, t %ld", GetTickCount());
				client->Send(szBuffer, sizeof(int) * 5 + 128);
			}
			Sleep(TIME_INTERVAL);
		}

	}
	return 0;
}
