#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <memory>

#include "TransmitterBase.h"
#include "QueueItem.h"

class UDPTransmitter : public TransmitterBase
{
private:
	const int WAIT_ON_WRITE = 10;

	void receive() override;
public:
	UDPTransmitter(std::string aAdress, int aPort);
	~UDPTransmitter();
	HRESULT Write(const char *aByte, int aLength) override;
	HRESULT Read(std::string channel, std::unique_ptr<QueueItem> &item) override;
};

