// ClassViewFrame.h: interface for the CClassViewFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEWFRAME_H__8032CB5F_6731_4577_9853_CFFE1598754B__INCLUDED_)
#define AFX_CLASSVIEWFRAME_H__8032CB5F_6731_4577_9853_CFFE1598754B__INCLUDED_

#include "ClassView.h"	// Added by ClassView
#include "cbparser.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClassViewFrame : public CFrameWindowImpl<CClassViewFrame>,public CUpdateUI<CClassViewFrame>,
						public CMessageFilter, public CIdleHandler,
						public IProjectEventImpl
{
public:
	CClassViewFrame();
	virtual ~CClassViewFrame();

	BEGIN_MSG_MAP(CClassViewFrame)
		COMMAND_ID_HANDLER(ID_SETUP_VIEW_BASE, OnSetupViewBase)
		COMMAND_ID_HANDLER(ID_SETUP_VIEW_EXT, OnSetupViewExt)
		COMMAND_ID_HANDLER(ID_GOTO_DEFINE, OnGotoDefine)
		COMMAND_ID_HANDLER(ID_GOTO_DECLARE, OnGotoDeclare)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		NOTIFY_CODE_HANDLER(TBN_DROPDOWN, OnTBDropDown)
		NOTIFY_CODE_HANDLER(NM_DBLCLK,OnNmDblclkTree)
		CHAIN_MSG_MAP(CUpdateUI<CClassViewFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CClassViewFrame>)
	END_MSG_MAP()
	
public:
	BEGIN_UPDATE_UI_MAP(CClassViewFrame)
		UPDATE_ELEMENT(ID_SETUP_VIEW_BASE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_SETUP_VIEW_EXT, UPDUI_MENUPOPUP)
		//UPDATE_ELEMENT(ID_SETUP_SHOW_PUBLIC, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP();
	
	IApplication	* m_pApplication;
	CHorSplitterWindow	  m_splitter;
	CToolBarCtrl m_wndTB;
	CClassView m_view;
	CTreeViewCtrl	m_wndTreeClass;
	CTreeViewCtrl	m_wndTreeChilds;
	CImageList	m_images;
public:
	void __stdcall ProjectAdd(IProject * proj);
public:
	void RefreshAllView();
	void __stdcall ProjectUpdate(IProject* proj);
	void GotoFile(vfc_token *tk, bool bImpl);
	vfc_token * GetToken(HWND hWndTree,HTREEITEM item);
	LRESULT OnGotoDefine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnGotoDeclare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnNmDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	int GetTokenImage(vfc_token * tk);
	void ClearChild(HTREEITEM hItem);
	void RefreshView(HTREEITEM hItem);
	void InsertToken(HWND hWndTree, vfc_token *tk, 
								  HTREEITEM hRootItem, 
								  HTREEITEM hGlobalFunction,
								  HTREEITEM hGlobalTypedef,
								  HTREEITEM hGlobalDefine);
	BOOL	 m_bViewExt;
	std::vector<cbparser*> m_cbs;
	CString m_workName;
	virtual void __stdcall DocumentSave(IDocument* doc);
	virtual void __stdcall DocumentsUpdate();
	virtual void __stdcall WorkspaceClose();
	virtual void __stdcall WorkspaceNew(LPCWSTR lpcwName);
	virtual BOOL OnIdle();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	LRESULT OnTBDropDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSetupViewBase(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	LRESULT OnSetupViewExt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};

#endif // !defined(AFX_CLASSVIEWFRAME_H__8032CB5F_6731_4577_9853_CFFE1598754B__INCLUDED_)
