#include "stdafx.h"
#include "MFDataWriter.h"
#include "MFormats.h"
#include "MFPipeTest_i.h"

#include <cassert>

#include "Stringer.h"

using namespace std;

MFDataWriter::MFDataWriter(TransmitterBase &aSender) : sender(aSender) { }

void MFDataWriter::getIMFBuffer(BSTR _bsChannel, IMFBuffer *buffer, char *&data, LONG &length) {
	LONG cbMaxSize = 0, cbActualSize = 0;
	LONGLONG lpData = NULL;
	buffer->BufferLock(eMFLT_NoLock, &cbMaxSize, &cbActualSize, &lpData);
	LPBYTE _q = (LPBYTE)lpData;
	data = reinterpret_cast<char*>(_q);
	length = cbActualSize;
	//return sender.Write(converted, cbActualSize);
}

HRESULT MFDataWriter::send(BSTR _bsChannel, IUnknown* _pMFrameOrPacket) {
	IMFBuffer* pBuffer = NULL;

	string channel = Stringer::getString(_bsChannel);
	char size = (char)channel.size();
	assert(channel.size() == size);

	union {
		char chars[sizeof(LONG)];
		LONG longvalue;
	} LongCharUnion;

	char* data;
	LONG length;

	if (pBuffer = dynamic_cast<IMFBuffer*>(_pMFrameOrPacket)) {
		getIMFBuffer(_bsChannel, pBuffer, data, length);
	}
	else {
		return E_FAIL;
	}

	LongCharUnion.longvalue = length;

	sender.Write(&size, 1);
	sender.Write(LongCharUnion.chars, sizeof(LONG));
	sender.Write(channel.c_str(), size);
	sender.Write(data, length);

	std::this_thread::sleep_for(std::chrono::milliseconds(100000));

	return S_OK;
}

MFDataWriter::~MFDataWriter()
{
}
