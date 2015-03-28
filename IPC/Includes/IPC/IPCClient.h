#ifndef H_IPCCLIENT
#define H_IPCCLIENT

#include <IPC/Interfaces/IIPC.h>
#include <IPC/Interfaces/IPCMessageHandler.h>
#include <Windows.h>

class IPCClient : public IIPC
{
private:
	HANDLE m_hPipe;
	char m_szPipeName[256];
	IPCMessageHandler* m_pMessageHandler;
protected:
	IPCClient();
	~IPCClient();
	bool CreateIPCFile(const char *PipeName);
public:
	static IPCClient* CreateIPCClient(IPCMessageHandler* MessageHandler, char *PipeName);


	virtual void BroadcastMessage(char *szBuff, unsigned long length);
	virtual bool PeekMessages(unsigned long  milliseconds);
	virtual unsigned long Receive(char *szBuff, unsigned long length);
	virtual unsigned long Send(char *szBuff, unsigned long  length);
	virtual void Close();
};

#endif