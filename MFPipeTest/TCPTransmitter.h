#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#include "TransmitterBase.h"

class TCPTransmitter : public TransmitterBase
{
private:
	void receive() override;
public:
	TCPTransmitter(std::string aAdress, int aPort);
	~TCPTransmitter();
	HRESULT Write(const char *aByte, int aLength) override;
	HRESULT Read(char *aByte, int &aLength) override;
};

