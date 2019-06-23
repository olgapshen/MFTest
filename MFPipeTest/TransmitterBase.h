#pragma once
class SenderBase
{
protected:
	WSADATA wsaData;
	SOCKET ConnectSocket;
	sockaddr_in ServerAddr;
	HRESULT result;
public:
	SenderBase();
	~SenderBase();
};

