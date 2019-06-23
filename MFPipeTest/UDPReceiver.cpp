#include "stdafx.h"
#include "UDPReceiver.h"


UDPReceiver::UDPReceiver(PCWSTR aAdress, int aPort)
{
}


UDPReceiver::~UDPReceiver()
{
}

HRESULT UDPReceiver::Write() {
	WSADATA wsaData;
	SOCKET SendRecvSocket;  // ����� ��� ������ � ��������
	sockaddr_in ServerAddr, ClientAddr;  // ��� ����� ����� ������� � ��������
	int err, maxlen = 512, ClientAddrSize = sizeof(ClientAddr);  // ��� ������, ������ ������� � ������ ��������� ������
	char* recvbuf = new char[maxlen];  // ����� ������
	char* result_string = new char[maxlen];  // ����� ��������


	// Initialize Winsock
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	// Create a SOCKET for connecting to server
	SendRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	InetPton(AF_INET, _T("127.0.0.1"), &ServerAddr.sin_addr.s_addr);

	// Setup the TCP listening socket
	ServerAddr.sin_family = AF_INET;
	//ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(12345);
	err = bind(SendRecvSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr));
	if (err == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		closesocket(SendRecvSocket);
		WSACleanup();
		return 1;
	}

	while (true) {
		// Accept a client socket
		err = recvfrom(SendRecvSocket, recvbuf, maxlen, 0, (sockaddr *)&ClientAddr, &ClientAddrSize);
		if (err > 0) {
			recvbuf[err] = 0;
			printf("Received query: %s\n", (char*)recvbuf);
			// ��������� ���������
			int result = 72;
			_snprintf_s(result_string, maxlen, maxlen, "OK %d\n", result);
			// ���������� ��������� �� ������
			sendto(SendRecvSocket, result_string, strlen(result_string), 0, (sockaddr *)&ClientAddr, sizeof(ClientAddr));
			printf("Sent answer: %s\n", result_string);
		}
		else {
			printf("recv failed: %d\n", WSAGetLastError());
			closesocket(SendRecvSocket);
			WSACleanup();
			return 1;
		}
	}

	return S_OK;
}