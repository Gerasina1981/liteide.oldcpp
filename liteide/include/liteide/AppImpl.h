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
	// 返回编辑器窗口句柄
	virtual HWND __stdcall GetHwnd() { return NULL; }

	// 请求关闭文档
	virtual bool __stdcall QueryClose(bool bSave) { return true; }

	// 保存文档
	virtual void __stdcall Save() {}

	// 返回当前编辑的文档
	virtual LPCWSTR __stdcall GetFullPath() { return NULL; }

	// 返回是否修改过
	virtual bool __stdcall GetModify() { return false; }

	// 返回可编辑文本支持
	virtual ITextSelection* __stdcall GetTextSelection() { return NULL; }
};

struct ITextSelectionImpl : public ObjectImplT<ITextSelection>
{
	virtual void __stdcall GoToLine(long line) {}
	virtual void __stdcall MoveTo(long line, long column) {}
};

struct IProjectEventImpl : public ObjectImplT<IProjectEvent>
{
	// 关闭解决方案
	virtual void __stdcall SolutionClose() {}

	// 打开解决方案
	virtual void __stdcall SolutionOpen()   {}

	// 添加了一个新项目
	virtual void __stdcall ProjectAdd(IProject* proj) {}

	// 删除了一个项目
	virtual void __stdcall ProjectRemove(IProject* proj) {}

	// 批量更新项目文件
	virtual void __stdcall ProjectUpdate(IProject* proj) {}

	// 设置当前项目
	virtual void __stdcall ProjectActivate(IProject* proj) {}

	// 添加了新文件
	virtual void __stdcall ProjectItemAdd(IProject* proj, IProjectItem* item ) {}

	// 删除了文件
	virtual void __stdcall ProjectItemRemove(IProject* proj, IProjectItem* item ) {}
};



#endif //_CPPVIEW_APPIMPL_H_