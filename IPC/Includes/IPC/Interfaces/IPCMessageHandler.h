#ifndef H_IIPCCLIENT
#define H_IIPCCLIENT

class IPCClient;
class IPCServer;
class IIPC;

class IPCMessageHandler
{
public:
	virtual void OnPacketReceived(IIPC *IPC, char *szBuff, unsigned long length) = 0;
};

#endif