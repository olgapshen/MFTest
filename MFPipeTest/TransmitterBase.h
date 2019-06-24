#pragma once

#include <memory>
#include <thread>
#include <deque>

class TransmitterBase
{
protected:
	PCWSTR adress;
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
	std::unique_ptr<std::deque<char>> input;
public:
	TransmitterBase(PCWSTR aAdress, int aPort);
	virtual HRESULT Write(char *aByte, int aLength) = 0;
	virtual HRESULT Read(char *aByte, int &aLength) = 0;
	virtual ~TransmitterBase();
};

