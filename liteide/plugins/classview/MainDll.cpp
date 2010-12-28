// plugin_classview.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include <liteide/AppImpl.h>
#include "ClassViewFrame.h"

CAppModule	_Module;

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(NULL, hInstance);

        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
	{
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

		IWorkspace * pWorkspace = m_pApplication->GetWorkspace();
		if (!pWorkspace)
		{
			return false;
		}
		m_frame.m_pApplication = m_pApplication;
		if (!m_frame.Create(pWorkspace->GetHwnd(),CWindow::rcDefault,NULL,WS_CHILD | WS_VISIBLE,WS_EX_STATICEDGE))
		{
			return false;
		}
		pWorkspace->AddPage(m_frame.m_hWnd,_T("ClassView"));

		m_pApplication->SinkProjectEvent(&m_frame);

		return true;
	}
	//
	// 每当卸载Plugin时发生。
	//
	virtual void __stdcall OnDisconnection(void* data)
	{
	}
	
	IApplication	* m_pApplication;
	CClassViewFrame	  m_frame;
};

extern "C"
__declspec(dllexport) 
IPlugin* DllGetPlugin()
{
	return new CPlugin;
}


