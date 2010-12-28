// FileView.h: interface for the CFileView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEVIEW_H__E166EFD6_D7D3_4CCE_AEE3_5457768F425C__INCLUDED_)
#define AFX_FILEVIEW_H__E166EFD6_D7D3_4CCE_AEE3_5457768F425C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <liteide/AppImpl.h>

class CFileView : public CWindowImpl<CFileView, CTreeViewCtrl>,
				  public IProjectEventImpl
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())

	CFileView();
	virtual ~CFileView();
public:
	BEGIN_MSG_MAP(CFileTree)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_DBLCLK,OnNmDblclkTree)
	END_MSG_MAP()
public:
	virtual void __stdcall ProjectItemAdd(IProject * proj, IProjectItem* item);
	virtual void __stdcall ProjectUpdate(IProject* proj);
	virtual void __stdcall ProjectAdd(IProject* proj);
	IApplication	* m_pApplication;
	HTREEITEM	m_itemRoot;
	HTREEITEM	m_itemHeader;
	HTREEITEM	m_itemSource;
	HTREEITEM	m_itemResource;
	LRESULT OnNmDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
};

#endif // !defined(AFX_FILEVIEW_H__E166EFD6_D7D3_4CCE_AEE3_5457768F425C__INCLUDED_)
