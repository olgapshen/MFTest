#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
//#include <string>

#include "ReceiverBase.h"

class TCPReceiver : public ReceiverBase
{
public:
	TCPReceiver(PCWSTR aAdress, int aPort);
	~TCPReceiver();

	HRESULT Read();
};