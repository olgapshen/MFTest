// CoMFPipeTest.h : Declaration of the CCoMFPipeTest

#pragma once
#include <memory>
#include "resource.h"       // main symbols
#include "MFPipeTest_i.h"
#include "SenderBase.h"
#include "SenderBase.h"
#include "ReceiverBase.h"

#ifdef _WIN32_WCE
#error "Neutral-threaded COM objects are not supported on Windows CE."
#endif

using namespace ATL;

// CCoMFPipeTest

class ATL_NO_VTABLE CCoMFPipeTest :
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CCoMFPipeTest, &CLSID_CoMFPipeTest>,
	public IMFPipe
{
private:
	std::unique_ptr<SenderBase> *sender;
	std::unique_ptr<ReceiverBase> *receiver;

public:
	CCoMFPipeTest()
	{
		m_pUnkMarshaler = nullptr;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COMFPIPETEST)

BEGIN_COM_MAP(CCoMFPipeTest)
	COM_INTERFACE_ENTRY(IMFPipe)
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;

public:

	STDMETHOD(PipeInfoGet)(BSTR* _pbsPipeName, BSTR _bsChannel, MF_PIPE_INFO* _pPipeInfo);
	STDMETHOD(PipeConnectedGetByIndex)( /*[in]*/ int _nIndex, /*[out]*/ IMFPipe** _ppPipe);
	STDMETHOD(PipeCreate)( /*[in]*/ BSTR _bsPipeID, /*[in]*/ BSTR _bsHints);
	STDMETHOD(PipeOpenDirect)( /*[in]*/ IMFPipe* _pSourcePipeObj, /*[in]*/ int _nMaxBuffers, /*[in]*/ BSTR _bsHints);
	STDMETHOD(PipeOpen)( /*[in]*/ BSTR _bsPipeID, /*[in]*/ int _nMaxBuffers, /*[in]*/ BSTR _bsHints);
	STDMETHOD(PipePut)( /*[in]*/ BSTR _bsChannel, /*[in]*/ IUnknown* _pMFrameOrPacket, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsHints);
	STDMETHOD(PipeGet)( /*[in]*/ BSTR _bsChannel, /*[out]*/ IUnknown** _ppMFrameOrPacket, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsHints);
	STDMETHOD(PipePeek)( /*[in]*/ BSTR _bsChannel, /*[in]*/ int _nIndex, /*[out]*/ IUnknown** _ppMFrameOrPacket, /*[in]*/ REFERENCE_TIME _rtMaxWait, /*[in]*/ BSTR _bsHints);
	STDMETHOD(PipeMessagePut)(
		/*[in]*/ BSTR _bsChannel,
		/*[in]*/ BSTR _bsEventName,
		/*[in]*/ BSTR _bsEventParam,
		/*[in]*/ IUnknown* _pEventObject,
		/*[in]*/ REFERENCE_TIME _rtMaxWait);
	STDMETHOD(PipeMessageGet)(
		/*[in]*/ BSTR _bsChannel,
		/*[out]*/ BSTR* _pbsEventName,
		/*[out]*/ BSTR* _pbsEventParam,
		/*[in]*/ IUnknown** _ppEventObject,
		/*[in]*/ REFERENCE_TIME _rtMaxWait);
	STDMETHOD(PipeFlush)( /*[in]*/ BSTR _bsChannel, /*[in]*/ eMFFlashFlags _eFlashFlags);
	STDMETHOD(PipeClose)();

};

OBJECT_ENTRY_AUTO(__uuidof(CoMFPipeTest), CCoMFPipeTest)
