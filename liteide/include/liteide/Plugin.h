#ifndef _LITE_PLUGIN_H_
#define _LITE_PLUGIN_H_

//
// LiteIDE Plguin Specification
//

interface IObject
{
	// 切换组件的接口。
	virtual bool __stdcall QueryInterface(const char* iid, void** ppv) = 0;
};

interface IPlugin : IObject
{
	// 释放该组件对象。
	virtual void __stdcall Release() = 0;
	
	// 每当加载Plugin时发生。如果返回false则认为加载该插件失败。
	virtual bool __stdcall OnConnection(IObject* app, void* data) = 0;

	// 每当卸载Plugin时发生。
	virtual void __stdcall OnDisconnection(void* data) = 0;
};

//
// 插件DLL引出函数：
// 	EXTERN_C IPlugin* __stdcall DllGetPlugin();
//
typedef IPlugin* (__stdcall * FnDllGetPlugin)();

#endif // _LITE_PLUGIN_H_
