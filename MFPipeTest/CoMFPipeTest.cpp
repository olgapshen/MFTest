// CoMFPipeTest.cpp : Implementation of CCoMFPipeTest

#include "stdafx.h"
#include "CoMFPipeTest.h"
#include <comutil.h>
#include <atlstr.h>
#include <comdef.h> 
#include <assert.h>
#include <string>
#include <regex>

#include "UDPTransmitter.h"
#include "TCPTransmitter.h"
#include "MFDataReader.h"
#include "MFDataWriter.h"
#include "MFormats.h"
#include "Stringer.h"

// CCoMFPipeTest

using namespace std;

STDMETHODIMP CCoMFPipeTest::PipeInfoGet(BSTR* _pbsPipeName, BSTR _bsChannel, MF_PIPE_INFO* _pPipeInfo)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeConnectedGetByIndex( /*[in]*/ int _nIndex, /*[out]*/ IMFPipe** _ppPipe)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeCreate( /*[in]*/ BSTR _bsPipeID, /*[in]*/ BSTR _bsHints)
{
	assert(_bsPipeID != nullptr);
	string saddr = Stringer::getString(_bsPipeID);

	std::regex pattern("(.{3}):\\/\\/([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}):([0-9]+)");
	std::smatch match;
	std::regex_search(saddr, match, pattern);
	int size = match.size();
	assert(size == 4);
	string proto = match[1];
	string ip = match[2];
	string port = match[3];
	
	int iPort = atoi(port.c_str());

	if (proto == "udp") {
		transmitter.reset(new UDPTransmitter(ip, iPort));
	}
	else if (proto == "tcp") {
		transmitter.reset(new TCPTransmitter(ip, iPort));
	}
	else {
		return E_FAIL;
	}

	return S_OK;
}

STDMETHODIMP CCoMFPipeTest::PipeOpenDirect( /*[in]*/ IMFPipe* _pSourcePipeObj, /*[in]*/ int _nMaxBuffers, /*[in]*/ BSTR _bsHints)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeOpen( /*[in]*/ BSTR _bsPipeID, /*[in]*/ int _nMaxBuffers, /*[in]*/ BSTR _bsHints)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipePut( /*[in]*/ BSTR _bsChannel, /*[in]*/ IUnknown* _pMFrameOrPacket, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsHints)
{
	TransmitterBase *base = transmitter.get();
	MFDataWriter writer(*base);
	return writer.send(_bsChannel, _pMFrameOrPacket);
}

STDMETHODIMP CCoMFPipeTest::PipeGet( /*[in]*/ BSTR _bsChannel, /*[out]*/ IUnknown** _ppMFrameOrPacket, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsHints)
{
	TransmitterBase *base = transmitter.get();
	MFDataReader reader(*base);
	return reader.recv(_bsChannel, *_ppMFrameOrPacket);
}

STDMETHODIMP CCoMFPipeTest::PipePeek( /*[in]*/ BSTR _bsChannel, /*[in]*/ int _nIndex, /*[out]*/ IUnknown** _ppMFrameOrPacket, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsHints)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeMessagePut(
	/*[in]*/ BSTR _bsChannel,
	/*[in]*/ BSTR _bsEventName,
	/*[in]*/ BSTR _bsEventParam,
	/*[in]*/ IUnknown* _pEventObject,
	/*[in]*/ REFERENCE_TIME _rtMaxWait)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeMessageGet(
	/*[in]*/ BSTR _bsChannel,
	/*[out]*/ BSTR* _pbsEventName,
	/*[out]*/ BSTR* _pbsEventParam,
	/*[in]*/ IUnknown** _ppEventObject,
	/*[in]*/ REFERENCE_TIME _rtMaxWait)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeFlush( /*[in]*/ BSTR _bsChannel, /*[in]*/ eMFFlashFlags _eFlashFlags)
{
	return E_NOTIMPL;
}

STDMETHODIMP CCoMFPipeTest::PipeClose()
{
	return E_NOTIMPL;
}