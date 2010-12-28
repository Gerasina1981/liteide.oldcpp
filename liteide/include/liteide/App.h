#ifndef _LITE_APP_H_
#define _LITE_APP_H_

#ifndef _LITE_PLUGIN_H
#include "Plugin.h"
#endif //_LITE_PLUGIN_H

//
// LiteIDE Application API
//

// 文档
interface IDocument;
// 文档集
interface IDocuments;
// 文档事件
interface IDocumentEvent;
// 文档编辑器
interface IEditor;
// 文本文档编辑支持
interface ITextSelection;

// 解决方案
interface ISolution;
// 项目
interface IProject;
// 项目文件
interface IProjectItem;
// 项目事件
interface IProjectEvent;

// 功能区
interface IWorkspace;
// 应用程序
interface IApplication;



interface IWorkspace : IObject
{
	// 取得Workspace的窗口句柄
	virtual HWND __stdcall GetHwnd() = 0;

	// 向Workspace添加窗口
	virtual void __stdcall AddPage(HWND hWndPage, LPCWSTR lpcwTitle) = 0;
};

interface IOutputTabview : IObject
{
	// 取得输出窗口的句柄
	virtual HWND __stdcall GetHwnd() = 0;
	//向输出窗口添加窗口
	virtual void __stdcall AddPage(HWND hWndPage, LPCWSTR lpcwTitle) = 0;
};

interface IDocument : IObject
{
	// 取得文档的全路径。
	virtual LPCWSTR __stdcall GetFullPath() = 0;

	// 取得文档的当前编辑器
	virtual IEditor* __stdcall GetEditor() = 0;
};

enum liSaveChanges
{
    liSaveChangesYes = 1,
    liSaveChangesNo = 2,
    liSaveChangesPrompt = 3
};

interface IDocuments : IObject
{
	// 获取打开的文档数目
	virtual long __stdcall GetCount() = 0;

	// 根据索引获得文档
	virtual IDocument* __stdcall GetItem(int index) = 0;

	// 根据文件名获得文档
	virtual IDocument* __stdcall FindItem(LPCWSTR lpszPath) = 0;

	// 设置当前文档
	virtual void __stdcall Activate(IDocument* doc) = 0;

	// 关闭文档
	virtual bool __stdcall Close(IDocument* doc, enum liSaveChanges lisc) = 0;

	// 取得当前显示文档
	virtual IDocument* __stdcall GetActivate() = 0;

	// 关闭所有文档
	virtual bool __stdcall CloseAll(enum liSaveChanges lisc) = 0;

	// 打开一个文档(自动加入文档集中)
	virtual IDocument* __stdcall Open(LPCWSTR lpcwFilePath, bool readonly) = 0;
};

interface ITextSelection : IObject
{
	// 移动到行
	virtual void __stdcall GoToLine(long line) = 0;

	// 移动到行/列
	virtual void __stdcall MoveTo(long line, long column) = 0;
};

interface IEditor : IObject
{
	// 返回编辑器窗口句柄
	virtual HWND __stdcall GetHwnd() = 0;

	// 请求关闭文档
	virtual bool __stdcall Close(enum liSaveChanges lisc) = 0;

	// 保存文档
	virtual void __stdcall Save() = 0;

	// 返回当前编辑的文档
	virtual LPCWSTR __stdcall GetFullPath() = 0;

	// 返回是否修改过
	virtual bool __stdcall GetModify() = 0;

	// 返回可编辑文本支持
	virtual ITextSelection* __stdcall GetTextSelection() = 0;
};

interface IEditorService : IObject
{
	// 根据文档返回编辑器
	virtual IEditor* __stdcall  CreateEditor(HWND hWnd, LPCWSTR lpcwPath) = 0;

	// 返回编辑器信息
	virtual LPCWSTR	__stdcall	GetEditorInfo() = 0;
};

interface IProjectItem
{
	// 获取项目文件的路径
	virtual size_t __stdcall GetFullPath(wchar_t * path, int cch) = 0;
};

interface IProject : IObject
{
	// 获取项目文件本身路径
	virtual size_t __stdcall GetFullPath(wchar_t* path, size_t cch) = 0;

	// 获取项目名称
	virtual size_t __stdcall GetName(wchar_t* name, size_t cch) = 0;	

	// 获取项目文件数
	virtual long __stdcall GetCount() = 0;

	// 获取文件
	virtual IProjectItem* __stdcall GetItem(long index) = 0;

	// 添加文件
	virtual IProjectItem* __stdcall AddFile(const wchar_t* path) = 0;

	// 删除文件
	virtual void __stdcall RemoveFile(const wchar_t* path) = 0;
};

//由解决方案进行维护项目的增加/删除
interface ISolution : IObject
{
	//获取解决方案路径
	virtual size_t __stdcall GetFullPath(wchar_t * path, size_t cch) = 0;

	//获取解决方案名称
	virtual size_t __stdcall GetName(wchar_t * name, size_t cch) = 0;

	//获取项目个数
	virtual long __stdcall GetCount() = 0;

	//获取项目
	virtual IProject* __stdcall GetItem(long index) = 0;

	//获取当前项目
	virtual IProject* __stdcall GetActivate() = 0;

	//设置当前项目
	virtual void __stdcall SetActivate(IProject* proj) = 0;

	//添加原有项目
	virtual IProject* __stdcall AddProject(const wchar_t * path) = 0;

	//建立新项目
	virtual IProject* __stdcall CreateProject(const wchar_t * name) = 0;

	//删除项目
	virtual void __stdcall RemoveProject(IProject* proj) = 0;

	//打开解决方案
	virtual bool __stdcall Open(const wchar_t * path) = 0;

	//关闭解决方案
	virtual void __stdcall Close() = 0;

	//新建解决方案
	virtual bool __stdcall Create(const wchar_t * name) = 0;

	//保存解决方案
	virtual bool __stdcall SaveAs(const wchar_t * path) = 0;
};

interface IApplication : IObject
{
	// 获取工作区窗口
	virtual IWorkspace* __stdcall GetWorkspace() = 0;

	// 获取解决方案
	virtual ISolution* __stdcall GetSolution() = 0;

	// 获取全部打开的文档
	virtual IDocuments* __stdcall GetDocuments() = 0;

	// 接收项目事件
	virtual void __stdcall SinkProjectEvent(IProjectEvent* Event) = 0;

	// 接收文档事件
	virtual void __stdcall SinkDocumentEvent(IDocumentEvent* Event) = 0;

	// 注册文档编辑器
	virtual void __stdcall RegisterEditorService(IEditorService* editService) = 0;

	// Get AppModule *
	virtual void* __stdcall GetAppModule() = 0;
};

interface IDocumentEvent : IObject
{
	// 文档集更新
	virtual void __stdcall DocumentsUpdate() = 0;

	// 打开（或新建）一个文档后，发送该消息。
	virtual void __stdcall DocumentOpen(IDocument* doc) = 0;

	// 保存一个文档后，发送该消息。
	virtual void __stdcall DocumentSave(IDocument* doc) = 0;

	// 切换当前文档时，发送该消息。注意docDeact可能是NULL（之前如果没有当前文档的话）。
	virtual void __stdcall DocumentActivate(IDocument* docAct, IDocument* docDeact) = 0;

	// 关闭一个文档前，发送该消息。
	virtual void __stdcall DocumentClose(IDocument* doc) = 0;
};

interface IProjectEvent : IObject
{
	// 关闭解决方案
	virtual void __stdcall SolutionClose() = 0;

	// 打开解决方案
	virtual void __stdcall SolutionOpen()   = 0;

	// 添加了一个新项目
	virtual void __stdcall ProjectAdd(IProject* proj) = 0;

	// 删除了一个项目
	virtual void __stdcall ProjectRemove(IProject* proj) = 0;

	// 批量更新项目文件
	virtual void __stdcall ProjectUpdate(IProject* proj) = 0;

	// 设置当前项目
	virtual void __stdcall ProjectActivate(IProject* proj) = 0;

	// 添加了新文件
	virtual void __stdcall ProjectItemAdd(IProject* proj, IProjectItem* item ) = 0;

	// 删除了文件
	virtual void __stdcall ProjectItemRemove(IProject* proj, IProjectItem* item ) = 0;
};
#endif // _LITE_APP_H_
