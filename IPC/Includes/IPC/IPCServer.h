#ifndef H_IPCSERVER
#define H_IPCSERVER
#include <IPC/Interfaces/IIPC.h>
#include <IPC/Interfaces/IPCMessageHandler.h>
#include <Windows.h>

class IPCServer : public IIPC
{
private:
	HANDLE m_hPipe;
	char m_szPipeName[256];
	bool m_bIsConnected;
	IPCMessageHandler* m_pMessageHandler;

	int m_szBuffer;
	char* m_pBuffer;
protected:
	IPCServer();
	~IPCServer();
	bool CreateIPCPipe(const char *pipename);
public:
	static IPCServer* CreateIPCServer(IPCMessageHandler* MessageHandler, char *PipeName, int szBuffer=2048);



	virtual bool PeekMessages(unsigned long  milliseconds);
	virtual void BroadcastMessage(char *szBuff, unsigned long length);
	virtual unsigned long Receive(char *szBuff, unsigned long length);
	virtual unsigned long Send(char *szBuff, unsigned long  length);
	virtual void Close();

};

#endif