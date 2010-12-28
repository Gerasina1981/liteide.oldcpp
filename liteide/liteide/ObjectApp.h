#ifndef _OBJECTAPP_H_
#define _OBJECTAPP_H_

#include <liteide/App.h>
#include <liteide/AppImpl.h>
#include <vector>
#include <public/wtlutil.h>

class CApplication;

template <typename T>
class EventArrayT
{
public:

#define FIRE_DEFINE0(func)\
	void Fire##func()\
	{\
		for (size_t i = 0; i < m_Event.size(); i++)\
		{\
			m_Event[i]->func();\
		}\
	}

#define FIRE_DEFINE1(func,param1)\
	void Fire##func(param1 a1)\
	{\
		for (size_t i = 0; i < m_Event.size(); i++)\
		{\
			m_Event[i]->func(a1);\
		}\
	}
#define FIRE_DEFINE2(func,param1,param2)\
	void Fire##func(param1 a1,param2 a2)\
	{\
		for (size_t i = 0; i < m_Event.size(); i++)\
		{\
			m_Event[i]->func(a1,a2);\
		}\
	}

	void AddEvent(T * pT)
	{
		m_Event.push_back(pT);
	}
	void RemoveEvent(T * pT)
	{
	}
protected:
	std::vector<T*>	m_Event;
};

class CProjectEventArray : public EventArrayT<IProjectEvent>
{
public:
	FIRE_DEFINE0(SolutionOpen)
	FIRE_DEFINE0(SolutionClose)
	FIRE_DEFINE1(ProjectAdd,IProject*)
	FIRE_DEFINE1(ProjectRemove,IProject*)
	FIRE_DEFINE1(ProjectUpdate,IProject*)
	FIRE_DEFINE1(ProjectActivate,IProject*)
	FIRE_DEFINE2(ProjectItemAdd,IProject*,IProjectItem*)
	FIRE_DEFINE2(ProjectItemRemove,IProject*,IProjectItem*)
};

class CDocumentEventArray : public EventArrayT<IDocumentEvent>
{
public:
	FIRE_DEFINE1(DocumentOpen,IDocument*)
	FIRE_DEFINE1(DocumentClose,IDocument*)
	FIRE_DEFINE1(DocumentSave,IDocument*)
	FIRE_DEFINE2(DocumentActivate,IDocument*,IDocument*)
};

class CWorkspace : public ObjectImplT<IWorkspace>,
					public CTabViewImpl<CWorkspace>
{
public:
	DECLARE_WND_CLASS_EX(_T("CP_Workspace"), 0, COLOR_APPWORKSPACE)
	typedef CTabViewImpl<CWorkspace> _base;
public:

	virtual HWND __stdcall GetHwnd()
	{
		return m_hWnd;
	}
	virtual void __stdcall AddPage(HWND hWndPage, LPCWSTR lpcwTitle)
	{
		_base::AddPage(hWndPage,lpcwTitle);
	}
};

class CDocument : public ObjectImplT<IDocument>
{
public:
	CDocument(IEditor * edit = NULL, LPCTSTR lpszPath = NULL) : m_pEditor(edit), m_path(lpszPath)
	{
	}
	LPCWSTR __stdcall GetFullPath()
	{
		return m_path;
	}
	IEditor* __stdcall GetEditor()
	{
		return m_pEditor;
	}

	virtual void __stdcall Release()
	{
	}

	CString		m_path;
	IEditor*	m_pEditor;
};

class CDocuments : public ObjectImplT<IDocuments>, public CDocumentEventArray,
				   public CTabViewImpl<CDocuments>
{
public:
	DECLARE_WND_CLASS_EX(_T("LiteIDE_Documents"), 0, COLOR_APPWORKSPACE)
	typedef CTabViewImpl<CDocuments> _base;
public:
	CDocuments() : m_pCurrentDoc(NULL)
	{
	}

	virtual long __stdcall GetCount()
	{
		return m_docs.size();
	}

	virtual IDocument* __stdcall GetItem(int index)
	{
		return m_docs[index];
	}

	virtual IDocument* __stdcall FindItem(LPCWSTR lpszPath)
	{
		for (size_t i = 0; i < m_docs.size(); i++)
		{
			if (m_docs[i]->m_path.CompareNoCase(lpszPath) == 0)
				return m_docs[i];
		}
		return NULL;
	}

	virtual IDocument* __stdcall Open(LPCWSTR lpszPath, bool readonly = false)
	{
		IEditor * pEditor = NULL;

		IDocument * pDoc = FindItem(lpszPath);
		if (pDoc != NULL)
		{
			Activate(pDoc);
			return pDoc;
		}

		//查询Editor类工厂
		for (size_t i = 0; i < m_EditorServiceArray.size(); i++)
		{
			pEditor = m_EditorServiceArray[i]->CreateEditor(m_hWnd,lpszPath);
			if (pEditor)
			{
				break;
			}
		}
		if (pEditor && pEditor->GetHwnd() )
		{
			CDocument* pDoc = new CDocument(pEditor,lpszPath);
			m_docs.push_back(pDoc);

			_base::AddPage(pEditor->GetHwnd(),PathFindFileName(lpszPath));
			_base::SetPageData(GetPageCount()-1,pDoc);

			return pDoc;
		}
		return NULL;
	}

	virtual bool __stdcall Close(IDocument* doc, enum liSaveChanges lisc)
	{
		if (doc == NULL || doc->GetEditor() == NULL)
			return false;

		if (!doc->GetEditor()->Close(lisc))
			return false;

		int nPage = FindDocumentPage(doc);
		if (nPage != -1)
		{
			_base::RemovePage(nPage);
		}
		remove_at(&m_docs,(CDocument*)doc);
		return true;
	}

	virtual void __stdcall Activate(IDocument* doc)
	{
		int nPage = FindDocumentPage(doc);
		if (nPage != -1)
		{
			_base::SetActivePage(nPage);
		}
	}

	virtual IDocument* __stdcall GetActivate()
	{
		int nPage = _base::GetActivePage();
		if (nPage == -1)
			return NULL;
		return (CDocument*)_base::GetPageData(nPage);
	}

	virtual void __stdcall Release()
	{
		clear(&m_docs);
	}

	virtual bool __stdcall CloseAll(enum liSaveChanges lisc)
	{
		int count = _base::GetPageCount()-1;
		while (count >= 0)
		{
			CDocument * pDoc = (CDocument*)_base::GetPageData(count);
			if (pDoc)
			{
				if(!pDoc->GetEditor()->Close(lisc))
					return false;
				remove_at(&m_docs,pDoc);
				_base::RemovePage(count);
			}
			count--;
		}
		return true;
	}
public:

	int FindDocumentPage(IDocument* doc)
	{
		for (int i = 0; i < _base::GetPageCount(); i++)
		{
			if ((IDocument*)_base::GetPageData(i) == doc)
			{
				return i;
			}
		}
		return -1;
	}

	void UIUpdateTitle()
	{
		int nPage = GetActivePage();
		if (nPage == -1)
			return;

		IDocument * pDoc = (IDocument*)_base::GetPageData(nPage);
		if (!pDoc)
			return;
		CString title = PathFindFileName(pDoc->GetFullPath());
		if (pDoc->GetEditor()->GetModify())
		{
			title += _T(" *");
		}
		if (_base::GetPageTitle(nPage) != title)
		{
			_base::SetPageTitle(nPage,title);
		}
	}
public:
	std::vector<IEditorService*>	m_EditorServiceArray;
	IDocument*						 m_pCurrentDoc;
	std::vector<CDocument*>		 	 m_docs;
	IApplication*					 m_pApplication;
};

class CProjectItem : public ObjectImplT<IProjectItem>
{
public:
	CProjectItem(LPCWSTR lpszFileName = NULL) : m_path(lpszFileName)
	{
	}
	// 获取项目文件的路径
	virtual size_t __stdcall GetFullPath(wchar_t * path, int cch)
	{
		return copy_string(path,m_path,cch);
	}
protected:
	CString m_path;
};


class CProject : public ObjectImplT<IProject>
{
public:
	CProject(CProjectEventArray * events = NULL) : m_Event(events)
	{
	}
	// 获取项目文件本身路径
	virtual size_t __stdcall GetFullPath(wchar_t* path, size_t cch)
	{
		return copy_string(path,m_path,cch);
	}
	// 获取项目名称
	virtual size_t __stdcall GetName(wchar_t* name, size_t cch)
	{
		return copy_string(name,m_name,cch);
	}

	// 获取项目文件数
	virtual long __stdcall GetCount()
	{
		return m_items.size();
	}

	// 获取文件
	virtual IProjectItem* __stdcall GetItem(long index)
	{
		return m_items[index];
	}

	// 添加文件
	virtual IProjectItem* __stdcall AddFile(const wchar_t* path)
	{
		CProjectItem* item = new CProjectItem(path);
		m_items.push_back(item);

		m_Event->FireProjectItemAdd(this,item);
		return item;
	}

	// 删除文件
	virtual void __stdcall RemoveFile(const wchar_t* path)
	{
		//m_Event->FireProjectItemAdd(item);
	}

public:
	bool LoadFile(const wchar_t * path)
	{
		m_path = path;
		return true;
	}
	bool Create(const wchar_t * name)
	{
		m_name = name;
		return true;
	}
	virtual void __stdcall Release()
	{
		clear(&m_items);
	}
public:
	std::vector<CProjectItem*>	m_items;
	CString						m_path;
	CString						m_name;
	CProjectEventArray *		m_Event;
};

class CSolution : public ObjectImplT<ISolution>, public CProjectEventArray
{
public:
	CSolution() : m_nActive(-1)
	{
	}
	//获取解决方案路径
	virtual size_t __stdcall GetFullPath(wchar_t * path, size_t cch)
	{
		return copy_string(path,m_path,cch);
	}

	//获取解决方案名称
	virtual size_t __stdcall GetName(wchar_t * name, size_t cch)
	{
		return copy_string(name,m_name,cch);
	}

	//获取项目个数
	virtual long __stdcall GetCount()
	{
		return m_projs.size();
	}

	//获取项目
	virtual IProject* __stdcall GetItem(long index)
	{
		return m_projs[index];
	}

	//获取当前项目
	virtual IProject* __stdcall GetActivate()
	{
		if (m_nActive == -1)
			return NULL;
		return m_projs[m_nActive];
	}

	//设置当前项目
	virtual void __stdcall SetActivate(IProject* proj)
	{
		for (size_t i = 0; i < m_projs.size(); i++)
		{
			if (m_projs[i] == proj)
			{
				m_nActive = i;

				FireProjectActivate(proj);

				break;
			}
		}
	}

	//增加项目
	virtual IProject* __stdcall AddProject(const wchar_t * path)
	{
		CProject * proj = new CProject(this);
		if (!proj->LoadFile(path))
		{
			delete proj;
			return NULL;
		}
		m_projs.push_back(proj);
		m_nActive = m_projs.size()-1;

		FireProjectAdd(proj);

		return proj;
	}

	//增加空项目
	virtual IProject* __stdcall CreateProject(const wchar_t * name)
	{
		CProject * proj = new CProject(this);
		if (!proj->Create(name))
		{
			delete proj;
			return NULL;
		}
		m_projs.push_back(proj);
		m_nActive = m_projs.size()-1;

		FireProjectAdd(proj);

		return proj;
	}

	//删除项目
	virtual void __stdcall RemoveProject(IProject* proj)
	{
		remove_at(&m_projs,proj);

		FireProjectRemove(proj);
	}

	//打开解决方案
	virtual bool __stdcall Open(const wchar_t * path)
	{
		m_path = path;
		FireSolutionOpen();
		return true;
	}

	//关闭解决方案
	virtual void __stdcall Close()
	{
		m_path.Empty();
		m_name.Empty();
		clear(&m_projs);
		FireSolutionClose();
	}

	//新建解决方案
	virtual bool __stdcall Create(const wchar_t * name)
	{
		m_name = name;
		FireSolutionOpen();
		return true;
	}

	//保存解决方案
	virtual bool __stdcall SaveAs(const wchar_t * path)
	{
		return false;
	}

	virtual void __stdcall Release()
	{
		clear(&m_projs);
	}

public:
	int						m_nActive;	//当前项目
	std::vector<IProject*>	m_projs;
	CString					m_path;
	CString					m_name;
};

class CApplication : public ObjectImplT<IApplication>
{
public:
	CApplication()
	{
		m_Documents.m_pApplication = this;
//		m_Documents.m_pAppEventArray = static_cast<CApplicationEventArray*>(this);
//		m_Editspace.m_pAppEventArray = static_cast<CApplicationEventArray*>(this);
	}

	virtual bool __stdcall QueryInterface(const char* iid, void** ppv)
	{
		if (strcmp(iid,"IApplication") == 0)
		{
			*ppv = static_cast<IApplication*>(this);
		}
		else
			return false;
		return true;
	}

	virtual void* __stdcall GetAppModule()
	{
		return &_Module;
	}

	virtual ISolution* __stdcall GetSolution()
	{
		return static_cast<ISolution*>(&m_Solution);
	}

	virtual IWorkspace* __stdcall GetWorkspace()
	{
		return static_cast<IWorkspace*>(&m_Workspace);
	}

	virtual IDocuments* __stdcall GetDocuments()
	{
		return static_cast<IDocuments*>(&m_Documents);
	}

	virtual void __stdcall RegisterEditorService(IEditorService* editService)
	{
		m_Documents.m_EditorServiceArray.push_back(editService);
	}

	// 接收项目事件
	virtual void __stdcall SinkProjectEvent(IProjectEvent* Event)
	{
		m_Solution.AddEvent(Event);
	}

	// 接收文档事件
	virtual void __stdcall SinkDocumentEvent(IDocumentEvent* Event)
	{
		m_Documents.AddEvent(Event);
	}
public:
	CWorkspace						m_Workspace;
//	CEditspace						m_Editspace;
	CDocuments						m_Documents;
	CSolution						m_Solution;
};

#endif //_OBJECTAPP_H_
