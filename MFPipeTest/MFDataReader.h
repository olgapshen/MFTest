#pragma once

#include "MFPipeTest_i.h"
#include "TransmitterBase.h"

class MFDataReader 
{
private:
	const int SLEEP_ON_WAIT = 10;

	TransmitterBase &receiver;
	HRESULT getIMFBuffer(IMFBuffer *buffer, char *data, LONG length);

public:
	MFDataReader(TransmitterBase &aReceiver);
	HRESULT recv(BSTR _bsChannel, IUnknown* _pMFrameOrPacket);
	~MFDataReader();
};

