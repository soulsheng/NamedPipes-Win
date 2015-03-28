#include <IPC/IPCClient.h>
#include <stdio.h>

IPCClient::IPCClient()
	: m_hPipe(0),
	m_pMessageHandler(0)
{

}

IPCClient::~IPCClient()
{
	if (!m_hPipe)
		return;
	CloseHandle(m_hPipe);
	m_hPipe = 0;
}

IPCClient* IPCClient::CreateIPCClient(IPCMessageHandler* MessageHandler, char *PipeName)
{
	IPCClient* clt = new IPCClient();
	if (!clt->CreateIPCFile(PipeName))
	{
		delete clt;
		return 0;
	}
	clt->m_pMessageHandler = MessageHandler;
	return clt;

}

bool IPCClient::CreateIPCFile(const char *PipeName)
{
	sprintf(m_szPipeName, "\\\\.\\pipe\\%s", PipeName);
	m_hPipe = CreateFile(
		m_szPipeName, 
		GENERIC_READ |
		GENERIC_WRITE,
		0,            
		NULL,         
		OPEN_EXISTING, 
		0, 
		NULL
	);

	if (!m_hPipe)
		return false;

	if (GetLastError() != ERROR_SUCCESS)
	{
		return false;
	}

	unsigned long dwMode = PIPE_NOWAIT;
	SetNamedPipeHandleState(
		m_hPipe,
		&dwMode,
		NULL,
		NULL);


	return true;
}


bool IPCClient::PeekMessages(unsigned long  milliseconds)
{
	if (!m_pMessageHandler)
		return false;

	char szBuffer[2048];
	unsigned long iBytes = this->Receive(szBuffer, 2048);

	if (iBytes > 0)
	{
		this->BroadcastMessage(szBuffer, iBytes);
		return true;
	}
	return false;
}

void IPCClient::BroadcastMessage(char *szBuff, unsigned long length)
{
	m_pMessageHandler->OnPacketReceived(this, szBuff, length);
}

unsigned long IPCClient::Receive(char *szBuffer, unsigned long length)
{
	unsigned long iBytes = 0;
	if (!WaitNamedPipe(m_szPipeName, 100))
	{
		int bReadSuccess = ReadFile(m_hPipe, szBuffer, length, &iBytes, 0);
		if (bReadSuccess)
		{
			return iBytes;
		}

	}
	return 0;
}

unsigned long IPCClient::Send(char *szBuff, unsigned long  length)
{
	unsigned long iSent = 0;
	WriteFile(m_hPipe, szBuff, length, &iSent, 0);
	return iSent;
}

void IPCClient::Close()
{
	FlushFileBuffers(m_hPipe);
	DisconnectNamedPipe(m_hPipe);
}

