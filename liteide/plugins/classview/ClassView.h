// ClassView.h: interface for the CClassView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEW_H__D5D0E784_B8A8_4B01_8D6C_3D9A62E466DA__INCLUDED_)
#define AFX_CLASSVIEW_H__D5D0E784_B8A8_4B01_8D6C_3D9A62E466DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <liteide/AppImpl.h>
#include "cbparser.h"
#include "resource.h"

struct vfc_token;
struct vfc_token_tree;

class CClassView : public CWindowImpl<CClassView, CTreeViewCtrl>, 
				   public IProjectEventImpl
{
public:
	cbparser	  m_cb;
	CString		  m_workName;

	CClassView();
	virtual ~CClassView();

public:
	DECLARE_WND_SUPERCLASS(NULL, CTreeViewCtrl::GetWndClassName())
	HTREEITEM	m_itemRoot;
	HTREEITEM	m_itemGlobalFunction;
	HTREEITEM	m_itemGlobalTypedef;
	HTREEITEM	m_itemGlobalDefine;
	CImageList	m_images;
	IApplication * m_pApplication;
public:
	BEGIN_MSG_MAP(CClassView)
		COMMAND_ID_HANDLER(ID_GOTO_DEFINE, OnGotoDefine)
		COMMAND_ID_HANDLER(ID_GOTO_DECLARE, OnGotoDeclare)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_DBLCLK,OnNmDblclkTree)
		REFLECTED_NOTIFY_CODE_HANDLER(NM_RCLICK,OnNmRclickTree)
	END_MSG_MAP()
public:
	void __stdcall ProjectAdd(IProject * proj);
	vfc_token * GetToken(HTREEITEM item);
	void GotoFile(vfc_token * tk, bool bImpl);
	void InitImageList();
	void InsertToken(vfc_token *tk, HTREEITEM root);
	void RefreshTree();
	LRESULT OnNmDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnNmRclickTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnGotoDefine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnGotoDeclare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // !defined(AFX_CLASSVIEW_H__D5D0E784_B8A8_4B01_8D6C_3D9A62E466DA__INCLUDED_)
