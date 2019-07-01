#include "stdafx.h"
#include "UDPTransmitter.h"

#include <cassert>
#include "MFTypes.h"

using namespace std;

UDPTransmitter::UDPTransmitter(string aAdress, int aPort) : TransmitterBase(aAdress, aPort)
{
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	char _buffer[6];
	errno_t res = _itoa_s(aPort, _buffer, 10);

	if (res != 0) {
		result = E_FAIL;
		return;
	}

	int iResult = getaddrinfo(address.c_str(), _buffer, &hints, &resultSend);
	if (iResult != 0) {
		result = E_FAIL;
		WSACleanup();
		return;
	}

	ConnectSocket = socket(resultSend->ai_family, resultSend->ai_socktype, resultSend->ai_protocol);

	if (ConnectSocket == INVALID_SOCKET) {
		int err = WSAGetLastError();
		freeaddrinfo(resultSend);
		WSACleanup();
		result = E_FAIL;
		return;
	}

	iResult = connect(ConnectSocket, resultSend->ai_addr, (int)resultSend->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		int err = WSAGetLastError();
		freeaddrinfo(resultSend);
		WSACleanup();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		result = E_FAIL;
		return;
	}

	is_running = true;
	receiver.reset(new thread(&UDPTransmitter::receive, this));

	result = S_OK;
}

UDPTransmitter::~UDPTransmitter() {}

void UDPTransmitter::receive()
{
	ListenSocket = INVALID_SOCKET;

	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	char _buffer[6];
	errno_t res = _itoa_s(port, _buffer, 10);

	if (res != 0) {
		result = E_FAIL;
		return;
	}

	int iResult = getaddrinfo(address.c_str(), _buffer, &hints, &resultReceive);
	if (iResult != 0) {
		result = E_FAIL;
		WSACleanup();
		return;
	}

	ListenSocket = socket(resultReceive->ai_family, resultReceive->ai_socktype, resultReceive->ai_protocol);

	iResult = ::bind(ListenSocket, resultReceive->ai_addr, (int)resultReceive->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		int err = WSAGetLastError();
		freeaddrinfo(resultReceive);
		WSACleanup();
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		result = E_FAIL;
		return;
	}

	freeaddrinfo(resultReceive);

	char recvbuf[512];
	const int recvbuflen = 512;

	do {
		int received = recv(ListenSocket, recvbuf, sizeof(char), 0);

		// For future use, when asserts will be removed
		if (received == SOCKET_ERROR)
			lasterror = WSAGetLastError();
				
		assert(received == sizeof(char));

		char channelnamesize = recvbuf[0];

		received = recv(ListenSocket, recvbuf, sizeof(char), 0);

		if (received == SOCKET_ERROR)
			lasterror = WSAGetLastError();

		assert(received == sizeof(char));

		MFTypes mfType = (MFTypes)recvbuf[0];

		union {
			char chars[4];
			long longvalue;
		} LongCharUnion;

		received = recv(ListenSocket, recvbuf, sizeof(LONG), 0);

		if (received == SOCKET_ERROR)
			lasterror = WSAGetLastError();

		assert(received == 4);

		LongCharUnion.chars[0] = recvbuf[0];
		LongCharUnion.chars[1] = recvbuf[1];
		LongCharUnion.chars[2] = recvbuf[2];
		LongCharUnion.chars[3] = recvbuf[3];

		LONG datalength = LongCharUnion.longvalue;

		received = recv(ListenSocket, recvbuf, channelnamesize, 0);
		if (received == SOCKET_ERROR)
			lasterror = WSAGetLastError();

		assert(received == channelnamesize);
		
		string channelname(recvbuf, channelnamesize);

		QueueItem *item = new QueueItem(datalength, mfType);
		unique_ptr<QueueItem> itemPtr(item);
		channels[channelname].push_front(move(itemPtr));

		received = 0;
		while (received < datalength) {
			int iResult = recv(ListenSocket, recvbuf, recvbuflen, 0);

			if (iResult == SOCKET_ERROR)
				lasterror = WSAGetLastError();

			assert(iResult != SOCKET_ERROR);

			for (int i = 0; i < iResult; i++) {
				item->push(recvbuf[i]);
			}

			received += iResult;
		}
	} while (is_running);
}

HRESULT UDPTransmitter::Write(const char *aByte, int aLength) {
	int tempSize = aLength;
	std::unique_ptr<std::deque<char>> input;
	input.reset(new std::deque<char>());

	int copied = 0;
	int toCopy = 512;

	do {
		int size;
		if (copied + toCopy > tempSize) {
			size = tempSize - copied;
		}
		else size = toCopy;

		int iResult = send(ConnectSocket, aByte + copied, size, 0);

		// Sure that data readen in other side
		std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_ON_WRITE));

		if (iResult == SOCKET_ERROR) {
			int err = WSAGetLastError();
			return E_FAIL;
		}

		copied += size;
	} while (copied < tempSize);

	return S_OK;
}

HRESULT UDPTransmitter::Read(string channelName, unique_ptr<QueueItem> &item) {
	if (channels[channelName].empty())
		return E_FAIL;

	item = move(channels[channelName].front());
	channels[channelName].pop_back();

	return S_OK;
}