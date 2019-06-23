#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
//#include <string>

#include "SenderBase.h"

class TCPSender : public SenderBase
{
private:
	WSADATA wsaData;
	SOCKET ConnectSocket;
	sockaddr_in ServerAddr;
	HRESULT result;

public:
	TCPSender(PCWSTR aAdress, int aPort);
	~TCPSender();
	HRESULT Write();
};

