// sciedit.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <liteide/app.h>
#include <public/wtlutil.h>
#include "scieditor.h"

CAppModule	_Module;

HMODULE hInitSci;

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(NULL, hInstance);
		hInitSci = ::LoadLibrary(GetModulePath(_Module.m_hInst)+_T("\\SciLexer.dll"));
		if (hInitSci == NULL)
		{
			MessageBox(NULL,_T("Not Load SciLexer.dll"),_T("Error"),MB_OK | MB_ICONERROR);
		}
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
		if (hInitSci)
  			::FreeLibrary(hInitSci);
		_Module.Term();
	}
    return TRUE; 
}

class CPlugin : public IPlugin
{
public:
	virtual bool __stdcall QueryInterface(const char* iid, void** ppv)
	{
		return false;
	}

	virtual void __stdcall Release()
	{
		delete this;
	}
	//
	// 每当加载Plugin时发生。如果返回false则认为加载该插件失败。
	//
	virtual bool __stdcall OnConnection(IObject* app, void* data)
	{
		m_pApplication = NULL;
		app->QueryInterface("IApplication",(void**)&m_pApplication);
		if (!m_pApplication)
		{
			return false;
		}
		m_EditorService.m_pApplication = m_pApplication;
		m_pApplication->RegisterEditorService(static_cast<IEditorService*>(&m_EditorService));
	
		return true;
	}
	//
	// 每当卸载Plugin时发生。
	//
	virtual void __stdcall OnDisconnection(void* data)
	{
	}
	
	IApplication	* m_pApplication;
	CEditorService    m_EditorService;
};

extern "C"
__declspec(dllexport) 
IPlugin* DllGetPlugin()
{
	return new CPlugin;
}
