#pragma once

#include "MFPipeTest_i.h"
#include "TransmitterBase.h"

class MFDataWriter
{
private:
	TransmitterBase &sender;
	HRESULT sendIMFBuffer(IMFBuffer *buffer);
public:
	MFDataWriter(TransmitterBase &sender);
	HRESULT send(IUnknown* _pMFrameOrPacket);
	~MFDataWriter();
};

