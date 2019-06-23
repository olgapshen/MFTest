#include "stdafx.h"
#include "IMFDataWriter.h"
#include "MFormats.h"

IMFDataWriter::IMFDataWriter(SenderBase &sender)
{

}

HRESULT IMFDataWriter::send(IUnknown* _pMFrameOrPacket) {
	IMFBuffer* pBuffer = NULL;
	HRESULT hr = _pMFrameOrPacket->QueryInterface(IID_IMFBuffer, (LPVOID*)&pBuffer);
	_pMFrameOrPacket->Release();

	if (SUCCEEDED(hr)) {

	}

	return hr;
}

IMFDataWriter::~IMFDataWriter()
{
}
