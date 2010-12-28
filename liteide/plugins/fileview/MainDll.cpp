// plugin_fileview.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#include "FileView.h"

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
		
		if (!m_fileview.Create(pWorkspace->GetHwnd(),CWindow::rcDefault,NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE))
		{
			return false;
		}
		pWorkspace->AddPage(m_fileview.m_hWnd,_T("FileView"));

		m_pApplication->SinkProjectEvent(&m_fileview);

		m_fileview.m_pApplication = m_pApplication;

		return true;
	}
	//
	// 每当卸载Plugin时发生。
	//
	virtual void __stdcall OnDisconnection(void* data)
	{
	}
	
	IApplication	* m_pApplication;
	CFileView		  m_fileview;
};

extern "C"
__declspec(dllexport) 
IPlugin* DllGetPlugin()
{
	return new CPlugin;
}
