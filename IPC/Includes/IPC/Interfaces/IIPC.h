#ifndef H_IIPC
#define H_IIPC

class IIPC
{
public:
	virtual void BroadcastMessage(char *szBuff, unsigned long length) = 0;
	virtual bool PeekMessages(unsigned long  milliseconds) = 0;
	virtual unsigned long Receive(char *szBuff, unsigned long length) = 0;
	virtual unsigned long Send(char *szBuff, unsigned long  length) = 0;
	virtual void Close() = 0;
};

#endif