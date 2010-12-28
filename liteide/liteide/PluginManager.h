#ifndef _PLUGINMANAGER_H_
#define _PLUGINMANAGER_H_

#include <liteide/Plugin.h>
#include <vector>

class CDllPlugin
{
public:
	bool load(LPCTSTR lpszModule)
	{
		hLib = ::LoadLibrary(lpszModule);
		if (hLib)
		{
			FnDllGetPlugin pfn = NULL;
			pfn = (FnDllGetPlugin)::GetProcAddress(hLib,"DllGetPlugin");
			if (pfn)
			{
				m_pPlugin = pfn();
				return true;
			}
		}
		return false;
	}
	CDllPlugin() : hLib(NULL)
	{
	}
	~CDllPlugin()
	{
		if (hLib)
		{
			::FreeLibrary(hLib);
		}
	}
public:
	IPlugin	* m_pPlugin;
	HMODULE	  hLib;
};

class CPluginManager
{
public:
	void LoadPlugins(const CString & pluginPath)
	{
		CFindFile find;
		BOOL ret = find.FindFile(pluginPath+_T("\\*.dll"));
		while (ret)
		{
			CDllPlugin * pDll = new CDllPlugin();
			if (!pDll->load(find.GetFilePath()))
			{
				delete pDll;
			}
			else
			{
				m_PluginArray.push_back(pDll);
			}
			ret = find.FindNextFile();
		}
	}
	~CPluginManager()
	{
		clear();
	}
	void clear()
	{
		for (size_t i = 0; i < m_PluginArray.size(); i++)
		{
			CDllPlugin * pDll = m_PluginArray[i];
			if (pDll)
			{
				pDll->m_pPlugin->Release();
			}
			delete m_PluginArray[i];
		}
		m_PluginArray.clear();
	}

	void ConnectionAll(IObject* app, void* data)
	{	
		for (size_t i = 0; i < m_PluginArray.size(); i++)
		{
			CDllPlugin * pDll = m_PluginArray[i];
			if (pDll)
			{
				pDll->m_pPlugin->OnConnection(app,data);
			}
		}
	}
	void DisconnectionAll(void* data)
	{
		for (size_t i = 0; i < m_PluginArray.size(); i++)
		{
			CDllPlugin * pDll = m_PluginArray[i];
			if (pDll)
			{
				pDll->m_pPlugin->OnDisconnection(data);
			}
		}
	}
public:
	std::vector<CDllPlugin*>	m_PluginArray;
};


#endif //_PLUGINMANAGER_H_
