#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <memory>

#include "TransmitterBase.h"

class TCPTransmitter : public TransmitterBase
{
private:
	void receive() override;
public:
	TCPTransmitter(std::string aAdress, int aPort);
	~TCPTransmitter();
	HRESULT Write(const char *aByte, int aLength) override;
	HRESULT Read(std::string channel, std::unique_ptr<QueueItem> &item) override;
};

