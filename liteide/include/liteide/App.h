#ifndef _LITE_APP_H_
#define _LITE_APP_H_

#ifndef _LITE_PLUGIN_H
#include "Plugin.h"
#endif //_LITE_PLUGIN_H

//
// LiteIDE Application API
//

// �ĵ�
interface IDocument;
// �ĵ���
interface IDocuments;
// �ĵ��¼�
interface IDocumentEvent;
// �ĵ��༭��
interface IEditor;
// �ı��ĵ��༭֧��
interface ITextSelection;

// �������
interface ISolution;
// ��Ŀ
interface IProject;
// ��Ŀ�ļ�
interface IProjectItem;
// ��Ŀ�¼�
interface IProjectEvent;

// ������
interface IWorkspace;
// Ӧ�ó���
interface IApplication;



interface IWorkspace : IObject
{
	// ȡ��Workspace�Ĵ��ھ��
	virtual HWND __stdcall GetHwnd() = 0;

	// ��Workspace��Ӵ���
	virtual void __stdcall AddPage(HWND hWndPage, LPCWSTR lpcwTitle) = 0;
};

interface IOutputTabview : IObject
{
	// ȡ��������ڵľ��
	virtual HWND __stdcall GetHwnd() = 0;
	//�����������Ӵ���
	virtual void __stdcall AddPage(HWND hWndPage, LPCWSTR lpcwTitle) = 0;
};

interface IDocument : IObject
{
	// ȡ���ĵ���ȫ·����
	virtual LPCWSTR __stdcall GetFullPath() = 0;

	// ȡ���ĵ��ĵ�ǰ�༭��
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
	// ��ȡ�򿪵��ĵ���Ŀ
	virtual long __stdcall GetCount() = 0;

	// ������������ĵ�
	virtual IDocument* __stdcall GetItem(int index) = 0;

	// �����ļ�������ĵ�
	virtual IDocument* __stdcall FindItem(LPCWSTR lpszPath) = 0;

	// ���õ�ǰ�ĵ�
	virtual void __stdcall Activate(IDocument* doc) = 0;

	// �ر��ĵ�
	virtual bool __stdcall Close(IDocument* doc, enum liSaveChanges lisc) = 0;

	// ȡ�õ�ǰ��ʾ�ĵ�
	virtual IDocument* __stdcall GetActivate() = 0;

	// �ر������ĵ�
	virtual bool __stdcall CloseAll(enum liSaveChanges lisc) = 0;

	// ��һ���ĵ�(�Զ������ĵ�����)
	virtual IDocument* __stdcall Open(LPCWSTR lpcwFilePath, bool readonly) = 0;
};

interface ITextSelection : IObject
{
	// �ƶ�����
	virtual void __stdcall GoToLine(long line) = 0;

	// �ƶ�����/��
	virtual void __stdcall MoveTo(long line, long column) = 0;
};

interface IEditor : IObject
{
	// ���ر༭�����ھ��
	virtual HWND __stdcall GetHwnd() = 0;

	// ����ر��ĵ�
	virtual bool __stdcall Close(enum liSaveChanges lisc) = 0;

	// �����ĵ�
	virtual void __stdcall Save() = 0;

	// ���ص�ǰ�༭���ĵ�
	virtual LPCWSTR __stdcall GetFullPath() = 0;

	// �����Ƿ��޸Ĺ�
	virtual bool __stdcall GetModify() = 0;

	// ���ؿɱ༭�ı�֧��
	virtual ITextSelection* __stdcall GetTextSelection() = 0;
};

interface IEditorService : IObject
{
	// �����ĵ����ر༭��
	virtual IEditor* __stdcall  CreateEditor(HWND hWnd, LPCWSTR lpcwPath) = 0;

	// ���ر༭����Ϣ
	virtual LPCWSTR	__stdcall	GetEditorInfo() = 0;
};

interface IProjectItem
{
	// ��ȡ��Ŀ�ļ���·��
	virtual size_t __stdcall GetFullPath(wchar_t * path, int cch) = 0;
};

interface IProject : IObject
{
	// ��ȡ��Ŀ�ļ�����·��
	virtual size_t __stdcall GetFullPath(wchar_t* path, size_t cch) = 0;

	// ��ȡ��Ŀ����
	virtual size_t __stdcall GetName(wchar_t* name, size_t cch) = 0;	

	// ��ȡ��Ŀ�ļ���
	virtual long __stdcall GetCount() = 0;

	// ��ȡ�ļ�
	virtual IProjectItem* __stdcall GetItem(long index) = 0;

	// ����ļ�
	virtual IProjectItem* __stdcall AddFile(const wchar_t* path) = 0;

	// ɾ���ļ�
	virtual void __stdcall RemoveFile(const wchar_t* path) = 0;
};

//�ɽ����������ά����Ŀ������/ɾ��
interface ISolution : IObject
{
	//��ȡ�������·��
	virtual size_t __stdcall GetFullPath(wchar_t * path, size_t cch) = 0;

	//��ȡ�����������
	virtual size_t __stdcall GetName(wchar_t * name, size_t cch) = 0;

	//��ȡ��Ŀ����
	virtual long __stdcall GetCount() = 0;

	//��ȡ��Ŀ
	virtual IProject* __stdcall GetItem(long index) = 0;

	//��ȡ��ǰ��Ŀ
	virtual IProject* __stdcall GetActivate() = 0;

	//���õ�ǰ��Ŀ
	virtual void __stdcall SetActivate(IProject* proj) = 0;

	//���ԭ����Ŀ
	virtual IProject* __stdcall AddProject(const wchar_t * path) = 0;

	//��������Ŀ
	virtual IProject* __stdcall CreateProject(const wchar_t * name) = 0;

	//ɾ����Ŀ
	virtual void __stdcall RemoveProject(IProject* proj) = 0;

	//�򿪽������
	virtual bool __stdcall Open(const wchar_t * path) = 0;

	//�رս������
	virtual void __stdcall Close() = 0;

	//�½��������
	virtual bool __stdcall Create(const wchar_t * name) = 0;

	//����������
	virtual bool __stdcall SaveAs(const wchar_t * path) = 0;
};

interface IApplication : IObject
{
	// ��ȡ����������
	virtual IWorkspace* __stdcall GetWorkspace() = 0;

	// ��ȡ�������
	virtual ISolution* __stdcall GetSolution() = 0;

	// ��ȡȫ���򿪵��ĵ�
	virtual IDocuments* __stdcall GetDocuments() = 0;

	// ������Ŀ�¼�
	virtual void __stdcall SinkProjectEvent(IProjectEvent* Event) = 0;

	// �����ĵ��¼�
	virtual void __stdcall SinkDocumentEvent(IDocumentEvent* Event) = 0;

	// ע���ĵ��༭��
	virtual void __stdcall RegisterEditorService(IEditorService* editService) = 0;

	// Get AppModule *
	virtual void* __stdcall GetAppModule() = 0;
};

interface IDocumentEvent : IObject
{
	// �ĵ�������
	virtual void __stdcall DocumentsUpdate() = 0;

	// �򿪣����½���һ���ĵ��󣬷��͸���Ϣ��
	virtual void __stdcall DocumentOpen(IDocument* doc) = 0;

	// ����һ���ĵ��󣬷��͸���Ϣ��
	virtual void __stdcall DocumentSave(IDocument* doc) = 0;

	// �л���ǰ�ĵ�ʱ�����͸���Ϣ��ע��docDeact������NULL��֮ǰ���û�е�ǰ�ĵ��Ļ�����
	virtual void __stdcall DocumentActivate(IDocument* docAct, IDocument* docDeact) = 0;

	// �ر�һ���ĵ�ǰ�����͸���Ϣ��
	virtual void __stdcall DocumentClose(IDocument* doc) = 0;
};

interface IProjectEvent : IObject
{
	// �رս������
	virtual void __stdcall SolutionClose() = 0;

	// �򿪽������
	virtual void __stdcall SolutionOpen()   = 0;

	// �����һ������Ŀ
	virtual void __stdcall ProjectAdd(IProject* proj) = 0;

	// ɾ����һ����Ŀ
	virtual void __stdcall ProjectRemove(IProject* proj) = 0;

	// ����������Ŀ�ļ�
	virtual void __stdcall ProjectUpdate(IProject* proj) = 0;

	// ���õ�ǰ��Ŀ
	virtual void __stdcall ProjectActivate(IProject* proj) = 0;

	// ��������ļ�
	virtual void __stdcall ProjectItemAdd(IProject* proj, IProjectItem* item ) = 0;

	// ɾ�����ļ�
	virtual void __stdcall ProjectItemRemove(IProject* proj, IProjectItem* item ) = 0;
};
#endif // _LITE_APP_H_
