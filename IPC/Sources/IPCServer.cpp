#include <IPC/IPCServer.h>
#include <stdio.h>

IPCServer::IPCServer()
	: m_hPipe(0),
	m_pMessageHandler(0)
{

}

IPCServer::~IPCServer()
{
	if (!m_hPipe)
		return;
	CloseHandle(m_hPipe);
	m_hPipe = 0;
}

IPCServer* IPCServer::CreateIPCServer(IPCMessageHandler* MessageHandler, char *PipeName, int szBuffer)
{
	IPCServer* srv = new IPCServer();

	srv->m_szBuffer = szBuffer;
	srv->m_pBuffer = new char[szBuffer];

	if (!srv->CreateIPCPipe(PipeName))
	{
		delete srv;
		return 0;
	}
	srv->m_pMessageHandler = MessageHandler;
	return srv;
}

bool IPCServer::CreateIPCPipe(const char *pipeName)
{
	sprintf(m_szPipeName, "\\\\.\\pipe\\%s", pipeName);
	m_hPipe = CreateNamedPipeA(m_szPipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_NOWAIT,
		PIPE_UNLIMITED_INSTANCES,
		m_szBuffer, m_szBuffer, 0, NULL
		);

	if (!m_hPipe)
	{
		return false;
	}

	return true;
}


bool IPCServer::PeekMessages(unsigned long  milliseconds)
{
	if (!m_pMessageHandler)
		return false;

	if (!WaitNamedPipe(m_szPipeName, milliseconds))
	{
		int bConnected = ConnectNamedPipe(m_hPipe, 0) ?
		TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
		if (bConnected)
		{
				
			//char szBuffer[2048];
			unsigned long iBytes = this->Receive(m_pBuffer, m_szBuffer);

			if (iBytes > 0)
			{
				this->BroadcastMessage(m_pBuffer, iBytes);
				return true;
			}
			return false;
		}
	}

	

	
	return false;
}

void IPCServer::BroadcastMessage(char *szBuff, unsigned long length)
{
	m_pMessageHandler->OnPacketReceived(this, szBuff, length);
}

unsigned long IPCServer::Receive(char *szBuffer, unsigned long length)
{
	unsigned long iBytes = 0;

	int bReadSuccess = ReadFile(m_hPipe, szBuffer, length, &iBytes, 0);
	if (bReadSuccess)
	{
		return iBytes;
	}

	
	return 0;
}

unsigned long IPCServer::Send(char *szBuff, unsigned long  length)
{
	unsigned long iSent = 0;
	WriteFile(m_hPipe, szBuff, length, &iSent, 0);
	return iSent;
}

void IPCServer::Close()
{
	FlushFileBuffers(m_hPipe);
	DisconnectNamedPipe(m_hPipe);
}

