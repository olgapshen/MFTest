#pragma once

#include <memory>
#include <thread>
#include <deque>
#include <map>
#include <string>
#include <memory>

#include "QueueItem.h"

class TransmitterBase
{
protected:
	std::map<std::string, std::deque<std::unique_ptr<QueueItem>>> channels;

	std::string address;
	int port;

	std::unique_ptr<std::thread> receiver;
	virtual void receive() = 0;

	struct addrinfo *resultSend;
	struct addrinfo *resultReceive;

	WSADATA wsaData;
	SOCKET ConnectSocket;
	SOCKET ListenSocket;
	bool is_running;
	
	sockaddr_in ServerAddr;
	HRESULT result;
	int lasterror;
public:
	TransmitterBase(std::string aAdress, int aPort);
	virtual HRESULT Write(const char *aByte, int aLength) = 0;
	virtual HRESULT Read(std::string channel, std::unique_ptr<QueueItem> &item) = 0;
	virtual ~TransmitterBase();
};

