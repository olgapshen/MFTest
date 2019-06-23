#include "stdafx.h"
#include "TCPSender.h"

using namespace std;

TCPSender::TCPSender(PCWSTR aAdress, int aPort)
{
	
	
	int err;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	InetPton(AF_INET, aAdress, &ServerAddr.sin_addr.s_addr);

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(12345);

	err = connect(ConnectSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr));

	if (err == SOCKET_ERROR) {
		closesocket(ConnectSocket);
		WSACleanup();
		result = E_FAIL;
	}

	result = S_OK;
}


TCPSender::~TCPSender()
{
	if (result == S_OK)
		closesocket(ConnectSocket);
}

HRESULT TCPSender::Write() {

	int err,maxlen = 512;
	char* recvbuf = new char[maxlen];
	char* query = new char[maxlen];

	//_snprintf_s(query, maxlen, maxlen, "CALC * 12 6\n");
	//// отправляем запрос на сервер
	//send(ConnectSocket, query, strlen(query), 0);
	//printf("Sent: %s\n", query);

	// получаем результат
	err = recv(ConnectSocket, recvbuf, maxlen, 0);
	if (err > 0) {
		recvbuf[err] = 0;
		printf("Result: %s\n", (char*)recvbuf);
	}
	else if (err == 0)
		printf("Connection closing...\n");
	else {
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return 1;
	}

	return S_OK;
}
