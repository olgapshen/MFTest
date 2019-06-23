// dllmain.h : Declaration of module class.

class CMFPipeTestModule : public ATL::CAtlDllModuleT< CMFPipeTestModule >
{
public :
	DECLARE_LIBID(LIBID_MFPipeTestLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MFPIPETEST, "{6e988471-3743-48d5-876a-07bfcdfacd17}")
};

extern class CMFPipeTestModule _AtlModule;
