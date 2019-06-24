#include "stdafx.h"
#include "TransmitterBase.h"

//#include "resource.h"       // main symbols
//#include "MFPipeTest_i.h"

//#include <afx.h>
#include <comutil.h>
#include <atlstr.h>
#include <comdef.h> 
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
//#include <comutil.h>
//#include <comdef.h>
#include <string>

//#include <memory>
//#include "resource.h"       // main symbols
//#include "MFPipeTest_i.h"

//#include "resource.h"       // main symbols
////#include "CoMFPipeTest.h"
//#include <memory>
////#include "MFPipeTest_i.h"
//#include <comutil.h>
//#include <atlstr.h>
//#include <comdef.h> 
//#include <assert.h>
//#include <string>
//#include <regex>

using namespace std;

TransmitterBase::TransmitterBase(PCWSTR aAdress, int aPort) //: receiver(&TransmitterBase::receive, this)
{
	adress = aAdress;
	port = aPort;
	input.reset(new deque<char>());
	ConnectSocket = INVALID_SOCKET;

	
}

TransmitterBase::~TransmitterBase()
{
	if (result == S_OK) 
	{
		freeaddrinfo(resultSend);
		//freeaddrinfo(resultReceive);
		WSACleanup();
		closesocket(ConnectSocket);
		is_running = false;
		receiver->join();
	}
}
