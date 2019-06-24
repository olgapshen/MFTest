#include "stdafx.h"
#include "TCPTransmitter.h"

using namespace std;

TCPTransmitter::TCPTransmitter(PCWSTR aAdress, int aPort) : TransmitterBase(aAdress, aPort)
{
	//int err;
	//WSAStartup(MAKEWORD(2, 2), &wsaData);
	//ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//InetPton(AF_INET, aAdress, &ServerAddr.sin_addr.s_addr);

	//ServerAddr.sin_family = AF_INET;
	//ServerAddr.sin_port = htons(12345);

	//err = connect(ConnectSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr));

	//if (err == SOCKET_ERROR) {
	//	closesocket(ConnectSocket);
	//	WSACleanup();
	//	result = E_FAIL;
	//}

	//result = S_OK;

	int iResult = listen(ConnectSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		int err = WSAGetLastError();
		freeaddrinfo(resultSend);
		WSACleanup();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		result = E_FAIL;
		return;
	}

	// Accept a client socket
	ListenSocket = accept(ConnectSocket, NULL, NULL);
	if (ListenSocket == INVALID_SOCKET) {
		int err = WSAGetLastError();
		freeaddrinfo(resultSend);
		WSACleanup();
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		result = E_FAIL;
		return;
	}
}

void TCPTransmitter::receive() {};

TCPTransmitter::~TCPTransmitter() {}

HRESULT TCPTransmitter::Read(char *aByte, int &aLength) {
	//WSADATA wsaData;
	//SOCKET ListenSocket, ClientSocket;  // впускающий сокет и сокет для клиентов
	//sockaddr_in ServerAddr;  // это будет адрес сервера
	//int err, maxlen = 512;  // код ошибки и размер буферов
	//char* recvbuf = new char[maxlen];  // буфер приема
	//char* result_string = new char[maxlen];  // буфер отправки


	//// Initialize Winsock
	//WSAStartup(MAKEWORD(2, 2), &wsaData);

	//// Create a SOCKET for connecting to server
	//ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	//InetPton(AF_INET, _T("127.0.0.1"), &ServerAddr.sin_addr.s_addr);

	//// Setup the TCP listening socket
	//ServerAddr.sin_family = AF_INET;
	////ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//ServerAddr.sin_port = htons(12345);
	//err = bind(ListenSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr));
	//if (err == SOCKET_ERROR) {
	//	printf("bind failed: %d\n", WSAGetLastError());
	//	closesocket(ListenSocket);
	//	WSACleanup();
	//	return 1;
	//}

	//err = listen(ListenSocket, 50);
	//if (err == SOCKET_ERROR) {
	//	printf("listen failed: %d\n", WSAGetLastError());
	//	closesocket(ListenSocket);
	//	WSACleanup();
	//	return 1;
	//}
	//while (true) {
	//	// Accept a client socket
	//	ClientSocket = accept(ListenSocket, NULL, NULL);
	//	err = recv(ClientSocket, recvbuf, maxlen, 0);
	//	if (err > 0) {
	//		recvbuf[err] = 0;
	//		printf("Received query: %s\n", (char*)recvbuf);
	//		// вычисляем результат
	//		int result = 72;
	//		_snprintf_s(result_string, maxlen, maxlen, "OK %d\n", result);
	//		// отправляем результат на сервер
	//		send(ClientSocket, result_string, strlen(result_string), 0);
	//		printf("Sent answer: %s\n", result_string);
	//	}
	//	else if (err == 0)
	//		printf("Connection closing...\n");
	//	else {
	//		printf("recv failed: %d\n", WSAGetLastError());
	//		closesocket(ClientSocket);
	//		WSACleanup();
	//		return 1;
	//	}

	//	// shutdown the connection since we're done
	//	closesocket(ClientSocket);
	//}

	return S_OK;
}

HRESULT TCPTransmitter::Write(char *aByte, int aLength) {

	int iResult;//,maxlen = 512;
	//char* recvbuf = new char[maxlen];
	//char* query = new char[maxlen];

	//_snprintf_s(query, maxlen, maxlen, "CALC * 12 6\n");
	//// отправляем запрос на сервер
	//send(ConnectSocket, query, strlen(query), 0);
	//printf("Sent: %s\n", query);

	// получаем результат
	//(int) strlen(sendbuf)
	iResult = send(ConnectSocket, aByte, 1, 0);
	if (iResult == SOCKET_ERROR) {
		int err = WSAGetLastError();
		return E_FAIL;
	}
	else {
		return S_OK;
	}
}
