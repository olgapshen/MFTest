#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#include "TransmitterBase.h"

class UDPTransmitter : public TransmitterBase
{
private:
	void receive() override;
public:
	UDPTransmitter(PCWSTR aAdress, int aPort);
	~UDPTransmitter();
	HRESULT Write(char *aByte, int aLength) override;
	HRESULT Read(char *aByte, int &aLength) override;
};

