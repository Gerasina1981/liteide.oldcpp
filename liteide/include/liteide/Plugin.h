#ifndef _LITE_PLUGIN_H_
#define _LITE_PLUGIN_H_

//
// LiteIDE Plguin Specification
//

interface IObject
{
	// �л�����Ľӿڡ�
	virtual bool __stdcall QueryInterface(const char* iid, void** ppv) = 0;
};

interface IPlugin : IObject
{
	// �ͷŸ��������
	virtual void __stdcall Release() = 0;
	
	// ÿ������Pluginʱ�������������false����Ϊ���ظò��ʧ�ܡ�
	virtual bool __stdcall OnConnection(IObject* app, void* data) = 0;

	// ÿ��ж��Pluginʱ������
	virtual void __stdcall OnDisconnection(void* data) = 0;
};

//
// ���DLL����������
// 	EXTERN_C IPlugin* __stdcall DllGetPlugin();
//
typedef IPlugin* (__stdcall * FnDllGetPlugin)();

#endif // _LITE_PLUGIN_H_
