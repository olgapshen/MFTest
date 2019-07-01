#pragma once

#include "MFPipeTest_i.h"
#include "TransmitterBase.h"

class MFDataWriter
{
private:
	TransmitterBase &sender;
	void getIMFBuffer(IMFBuffer *buffer, char *&data, LONG &length);
public:
	MFDataWriter(TransmitterBase &sender);
	HRESULT send(BSTR _bsChannel, IUnknown* _pMFrameOrPacket);
	~MFDataWriter();
};

