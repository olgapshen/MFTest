#include "stdafx.h"
#include <atlcomcli.h>
#include <cassert>
#include <memory>

#include "MFDataReader.h"
#include "MFormats.h"
#include "Stringer.h"

using namespace std;
using namespace ATL;

MFDataReader::MFDataReader(TransmitterBase &aReceiver) : receiver(aReceiver) {}

MFDataReader::~MFDataReader() {}

HRESULT MFDataReader::getIMFBuffer(IMFBuffer *&buffer, char *data, LONG length) {
	CComPtr<IMFBuffersAllocator> cpMFBufferAllocator;
	cpMFBufferAllocator.CoCreateInstance(__uuidof(MFFactory));
	if (cpMFBufferAllocator.p == NULL)
		return E_FAIL;

	//// Allocation
	//CComPtr<IMFBuffer> MFBuffer;

	cpMFBufferAllocator->MFBufferAlloc(length, NULL, &buffer, eMFBF_Empty);
	
	if (buffer == NULL)
		return E_FAIL;

	// Fill buffer
	LONG cbMaxSize = 0, cbActualSize = length;
	LONGLONG lpData = NULL;
	buffer->BufferLock(eMFLT_NoLock, &cbMaxSize, &cbActualSize, &lpData);

	LPBYTE _q = (LPBYTE)lpData;
	for (DWORD i = 0; i < length; i++)
		_q[i] = data[i];

	return S_OK;
}

HRESULT MFDataReader::recv(BSTR _bsChannel, IUnknown *&_pMFrameOrPacket) {
	IMFBuffer* pBuffer = NULL;
	
	string channel = Stringer::getString(_bsChannel);
	unique_ptr<QueueItem> item;
	HRESULT _h = receiver.Read(channel, item);

	if (_h != S_OK) return _h;

	LONG length = item->getLength();
	MFTypes type = item->getType();

	while (item->size() < length)
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_ON_WAIT));

	unique_ptr<char> buffer;
	buffer.reset(new char[length]);
	char *data = buffer.get();

	int readen = 0;
	while (readen < length) {
		data[readen] = item->pop();
		readen++;
	}

	switch (type)
	{
	case MFTypes::MFBuffer: {
		HRESULT ret = getIMFBuffer(pBuffer, data, length);
		if (ret != S_OK) return ret;
		_pMFrameOrPacket = pBuffer;
		return S_OK;
	}
	default:
		return E_FAIL;
	}
}
