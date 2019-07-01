#pragma once

#include <memory>
#include <thread>
#include <deque>
#include <map>
#include <string>

class TransmitterBase
{
protected:
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
	std::map<std::string, std::unique_ptr<std::deque<char>>> channels;
public:
	TransmitterBase(std::string aAdress, int aPort);
	virtual HRESULT Write(const char *aByte, int aLength) = 0;
	virtual HRESULT Read(char *aByte, int &aLength) = 0;
	virtual ~TransmitterBase();
};

