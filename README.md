# NamedPipes-Win
Lightweight Named Pipes for windows!

No overwhelming dependencies like boost etc!

**Usage:**

Look at the Examples/example.cpp file for code example.

The IPCMessageHandler classes are used to handle incoming messages.

```
class MyMessageHandler : public IPCMessageHandler
{
public:
virtual void OnPacketReceived(IIPC* IPC, char *szBuff, unsigned long length)
{
   printf("Received[%d]: %s\n", length, szBuff);
}
};
```

You can then pass them to an IPC Server or Client.

**Server Version:**
```
MyMessageHandler *MyHandler = new MyMessageHandler();
IPCServer * Server = IPCServer::CreateIPCServer(MyHandler, "mypipename");
Server->PeekMessages(100); // 100ms waittime
```

**Client Version:**
```
MyMessageHandler *MyHandler = new MyMessageHandler();
IPCClient *Client = IPCClient::CreateIPCClient(MyHandler, "mypipename");
Client->PeekMessages(100); // 100ms waittime
```

The IPCServer and IPCClient both inherits from the IIPC interface so you can use them inside the OnPacketReceived.

The **PeekMessages** function updates the ipc client/server and broadcasts the messages to the message handlers.

**Other functions:**

Have a look at the **Receive()** and **BroadcastMessage()** functions.


