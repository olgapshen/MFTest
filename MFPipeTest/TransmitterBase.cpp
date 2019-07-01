#include "stdafx.h"
#include "TransmitterBase.h"

#include <comutil.h>
#include <atlstr.h>
#include <comdef.h> 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <string>

using namespace std;

TransmitterBase::TransmitterBase(std::string aAdress, int aPort)
{
	address = aAdress;
	port = aPort;
	ConnectSocket = INVALID_SOCKET;
}

TransmitterBase::~TransmitterBase()
{
	freeaddrinfo(resultSend);
	WSACleanup();
	closesocket(ConnectSocket);
	is_running = false;
	receiver->join();
}
