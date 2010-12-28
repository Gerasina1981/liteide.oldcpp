#ifndef _CPPVIEW_APPIMPL_H_
#define _CPPVIEW_APPIMPL_H_

#include "App.h"

template <typename T> 
class ObjectImplT : public T
{
public:
	virtual ~ObjectImplT() { Release(); }
	virtual bool __stdcall QueryInterface(const char* iid, void** ppv)
	{
		return false;
	}

	virtual void __stdcall Release()
	{
	}
};

struct IEditorImpl : ObjectImplT<IEditor>
{
	// ���ر༭�����ھ��
	virtual HWND __stdcall GetHwnd() { return NULL; }

	// ����ر��ĵ�
	virtual bool __stdcall QueryClose(bool bSave) { return true; }

	// �����ĵ�
	virtual void __stdcall Save() {}

	// ���ص�ǰ�༭���ĵ�
	virtual LPCWSTR __stdcall GetFullPath() { return NULL; }

	// �����Ƿ��޸Ĺ�
	virtual bool __stdcall GetModify() { return false; }

	// ���ؿɱ༭�ı�֧��
	virtual ITextSelection* __stdcall GetTextSelection() { return NULL; }
};

struct ITextSelectionImpl : public ObjectImplT<ITextSelection>
{
	virtual void __stdcall GoToLine(long line) {}
	virtual void __stdcall MoveTo(long line, long column) {}
};

struct IProjectEventImpl : public ObjectImplT<IProjectEvent>
{
	// �رս������
	virtual void __stdcall SolutionClose() {}

	// �򿪽������
	virtual void __stdcall SolutionOpen()   {}

	// �����һ������Ŀ
	virtual void __stdcall ProjectAdd(IProject* proj) {}

	// ɾ����һ����Ŀ
	virtual void __stdcall ProjectRemove(IProject* proj) {}

	// ����������Ŀ�ļ�
	virtual void __stdcall ProjectUpdate(IProject* proj) {}

	// ���õ�ǰ��Ŀ
	virtual void __stdcall ProjectActivate(IProject* proj) {}

	// ��������ļ�
	virtual void __stdcall ProjectItemAdd(IProject* proj, IProjectItem* item ) {}

	// ɾ�����ļ�
	virtual void __stdcall ProjectItemRemove(IProject* proj, IProjectItem* item ) {}
};



#endif //_CPPVIEW_APPIMPL_H_