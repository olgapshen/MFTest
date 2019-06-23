#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
//#include <string>

#include "SenderBase.h"

class UDPSender : public SenderBase
{
public:
	UDPSender(PCWSTR aAdress, int aPort);
	~UDPSender();
	HRESULT Write();
};

