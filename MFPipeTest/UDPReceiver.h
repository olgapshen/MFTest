#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
//#include <string>

#include "ReceiverBase.h"

class UDPReceiver : public ReceiverBase
{
public:
	UDPReceiver(PCWSTR aAdress, int aPort);
	~UDPReceiver();
	HRESULT Write();
};

