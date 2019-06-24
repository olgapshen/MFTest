#include "stdafx.h"
#include "UDPTransmitter.h"

using namespace std;

UDPTransmitter::UDPTransmitter(PCWSTR aAdress, int aPort) : TransmitterBase(aAdress, aPort)
{
	struct addrinfo hints;

	wstring ws(aAdress);
	string str(ws.begin(), ws.end());

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

	int iResult = getaddrinfo(str.c_str(), _buffer, &hints, &resultSend);
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

	//https://docs.microsoft.com/en-us/windows/desktop/winsock/windows-sockets-error-codes-2
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
	//receive();
	receiver.reset(new thread(&UDPTransmitter::receive, this));

	result = S_OK;
}

void UDPTransmitter::receive()
{
	ListenSocket = INVALID_SOCKET;

	struct addrinfo hints;

	wstring ws(adress);
	string str(ws.begin(), ws.end());

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

	int iResult = getaddrinfo(str.c_str(), _buffer, &hints, &resultReceive);
	if (iResult != 0) {
		result = E_FAIL;
		WSACleanup();
		return;
	}

	ListenSocket = socket(resultReceive->ai_family, resultReceive->ai_socktype, resultReceive->ai_protocol);

	// Setup the TCP listening socket
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
	int recvbuflen = 512;
	//int toCopy = 512;

	int j = 0;
	//try {
		do {
			//std::this_thread::sleep_for(std::chrono::milliseconds(300));
			int iResult = recv(ListenSocket, recvbuf, recvbuflen, 0);
			if (iResult == SOCKET_ERROR) {
				int err = WSAGetLastError();
			}
			else {
				for (int i = 0; i < iResult; i++) {
					j++;
					input->push_back(recvbuf[i]);
				}
			}

			if (j > 10000) {
				int r = 0;
				//std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

			if (j > 100000) {
				int r = 0;
				//std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

			if (j > 130000) {
				int r = 0;
				//std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

			if (iResult < recvbuflen) {
				int r = 0;
				//std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}

		} while (is_running);
	//}
	//catch (int a) {
	//	int r = 0;
	//	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	//}


}

UDPTransmitter::~UDPTransmitter()
{
}

HRESULT UDPTransmitter::Write(char *aByte, int aLength) {
	//int err;
	//, maxlen = 512;  // код ошибки, размер буферов и размер структуры адреса
	//char* recvbuf = new char[maxlen];  // буфер приема
	//char* query = new char[maxlen];  // буфер отправки

	//_snprintf_s(query, maxlen, maxlen, "CALC * 12 6\n");
	// отправляем запрос на сервер

	//char *buffer[100];
	int tempSize = aLength;
	std::unique_ptr<std::deque<char>> input;
	input.reset(new std::deque<char>());
	//std::deque<char> input;
	int i = 0;
	int toCopy = 512;
	do {
		int size;
		if (i + toCopy > tempSize) {
			size = tempSize - i;
		}
		else size = toCopy;

		int iResult = send(ConnectSocket, aByte + i, size, 0);// , (sockaddr *)&ServerAddr, sizeof(ServerAddr));
		/*for (int j = 0; j < size; j++) {
			input->push_back(aByte[i + j]);
		}*/
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (iResult == SOCKET_ERROR) {
			int err = WSAGetLastError();
			return E_FAIL;
		}

		//memcpy(buffer + i, aByte + i, size);
		i += size;
	} while (i < tempSize);

	//printf("Sent: %s\n", query);

	//// получаем результат
	//err = recvfrom(ConnectSocket, recvbuf, maxlen, 0, 0, 0);
	//if (err > 0) {
	//	recvbuf[err] = 0;
	//	printf("Result: %s\n", (char*)recvbuf);
	//}
	//else {
	//	printf("recv failed: %d\n", WSAGetLastError());
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	return 1;
	//}

	////closesocket(ConnectSocket);
	std::this_thread::sleep_for(std::chrono::milliseconds(100000));

	return S_OK;
}

HRESULT UDPTransmitter::Read(char *aByte, int &aLength) {
	//WSADATA wsaData;
	//SOCKET SendRecvSocket;  // сокет для приема и передачи
	//sockaddr_in ServerAddr, ClientAddr;  // это будет адрес сервера и клиентов
	//int err, maxlen = 512, ClientAddrSize = sizeof(ClientAddr);  // код ошибки, размер буферов и размер структуры адреса
	//char* recvbuf = new char[maxlen];  // буфер приема
	//char* result_string = new char[maxlen];  // буфер отправки


	//// Initialize Winsock
	//WSAStartup(MAKEWORD(2, 2), &wsaData);

	//// Create a SOCKET for connecting to server
	//SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//InetPton(AF_INET, _T("127.0.0.1"), &ServerAddr.sin_addr.s_addr);

	//// Setup the TCP listening socket
	//ServerAddr.sin_family = AF_INET;
	////ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ServerAddr.sin_port = htons(12345);
	//err = ::bind(SendRecvSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr));
	//
	//if (err == SOCKET_ERROR) {
	//	printf("bind failed: %d\n", WSAGetLastError());
	//	closesocket(SendRecvSocket);
	//	WSACleanup();
	//	return 1;
	//}

	//while (true) {
	//	// Accept a client socket
	//	err = recvfrom(SendRecvSocket, recvbuf, maxlen, 0, (sockaddr *)&ClientAddr, &ClientAddrSize);
	//	if (err > 0) {
	//		recvbuf[err] = 0;
	//		printf("Received query: %s\n", (char*)recvbuf);
	//		// вычисляем результат
	//		int result = 72;
	//		_snprintf_s(result_string, maxlen, maxlen, "OK %d\n", result);
	//		// отправляем результат на сервер
	//		sendto(SendRecvSocket, result_string, strlen(result_string), 0, (sockaddr *)&ClientAddr, sizeof(ClientAddr));
	//		printf("Sent answer: %s\n", result_string);
	//	}
	//	else {
	//		printf("recv failed: %d\n", WSAGetLastError());
	//		closesocket(SendRecvSocket);
	//		WSACleanup();
	//		return 1;
	//	}
	//}

	return S_OK;
}