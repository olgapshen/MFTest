#include "stdafx.h"
#include "MFDataWriter.h"
//#include "resource.h"       // main symbols
#include "MFormats.h"
#include "MFPipeTest_i.h"

MFDataWriter::MFDataWriter(TransmitterBase &aSender) : sender(aSender) { }

HRESULT MFDataWriter::sendIMFBuffer(IMFBuffer *buffer) {
	LONG cbMaxSize = 0, cbActualSize = 0;
	LONGLONG lpData = NULL;
	buffer->BufferLock(eMFLT_NoLock, &cbMaxSize, &cbActualSize, &lpData);
	LPBYTE _q = (LPBYTE)lpData;
	char* converted = reinterpret_cast<char*>(_q);
	//for (DWORD i = 0; i < cbActualSize; i++)
	return sender.Write(converted, cbActualSize);
}

HRESULT MFDataWriter::send(IUnknown* _pMFrameOrPacket) {
	IMFBuffer* pBuffer = NULL;
	if (pBuffer = dynamic_cast<IMFBuffer*>(_pMFrameOrPacket)) {
	//HRESULT hr = _pMFrameOrPacket->QueryInterface(IID_IMFBuffer, (LPVOID*)&pBuffer);
	//_pMFrameOrPacket->Release();
	//if (SUCCEEDED(hr)) {
		return sendIMFBuffer(pBuffer);
	}

	return E_FAIL;
}

MFDataWriter::~MFDataWriter()
{
}
