// ClassViewFrame.cpp: implementation of the CClassViewFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClassViewFrame.h"
#include <public/wtlutil.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static CString get_string(vfc_token * tk, int vt)
{
	CString text;
	tk->get_string(vt,text.GetBufferSetLength(MAX_PATH),MAX_PATH);
	return text;
}

CClassViewFrame::CClassViewFrame()
{
	m_bViewExt = TRUE;
}

CClassViewFrame::~CClassViewFrame()
{
	clear(&m_cbs);
}

LRESULT CClassViewFrame::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_wndTB = CreateSimpleToolBarCtrl(m_hWnd, IDR_TOOLBAR, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	// BLOCK: set drop-down style for the window button
	{
		TBBUTTONINFO tbbi;
		tbbi.cbSize = sizeof(tbbi);
		tbbi.dwMask = TBIF_STYLE;
		tbbi.fsStyle = BTNS_BUTTON | BTNS_WHOLEDROPDOWN;
		m_wndTB.SetButtonInfo(ID_VIEW_SETUP, &tbbi);
	}

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(m_wndTB, NULL, TRUE);

	m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_wndTreeClass.Create(m_splitter,CWindow::rcDefault,NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
	m_wndTreeChilds.Create(m_splitter,CWindow::rcDefault,NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);

	m_splitter.SetSplitterPanes(m_wndTreeClass,m_wndTreeChilds);

	UpdateLayout();
	m_splitter.SetSplitterPosPct(50);

//	m_hWndClient = m_view.Create(m_hWnd,CWindow::rcDefault,NULL,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
//	m_view.InitImageList();

	m_bViewExt = TRUE;
	UISetCheck(ID_SETUP_VIEW_EXT, 1);

//	CAppModule * pModule = (CAppModule*)m_pApplication->GetAppModule();

//	CMessageLoop* pLoop = pModule->GetMessageLoop();
//	ATLASSERT(pLoop != NULL);
//	pLoop->AddMessageFilter(this);
//	pLoop->AddIdleHandler(this);

	CImageList il;
	il.CreateFromImage(IDB_PROJIMAGE, 16, 1, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
	m_wndTreeClass.SetImageList(il);
	m_wndTreeChilds.SetImageList(il);

	return 0;
}

LRESULT CClassViewFrame::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
LRESULT CClassViewFrame::OnSetupViewBase(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UISetCheck(ID_SETUP_VIEW_BASE, 1);
	UISetCheck(ID_SETUP_VIEW_EXT, 0);

	m_splitter.SetSinglePaneMode(SPLIT_PANE_TOP);

	if (m_bViewExt == TRUE)
	{
		m_bViewExt = FALSE;
		RefreshAllView();
	}
	return 0;
}
LRESULT CClassViewFrame::OnSetupViewExt(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	UISetCheck(ID_SETUP_VIEW_BASE, 0);
	UISetCheck(ID_SETUP_VIEW_EXT, 1);

	m_splitter.SetSinglePaneMode(SPLIT_PANE_NONE);

	if (m_bViewExt == FALSE)
	{
		m_bViewExt = TRUE;
		RefreshAllView();
	}

	return 0;
}

LRESULT CClassViewFrame::OnTBDropDown(int, LPNMHDR pnmh, BOOL &bHandled)
{
	LPNMTOOLBAR lpTB = (LPNMTOOLBAR)pnmh;
	
	if(lpTB->iItem != ID_VIEW_SETUP)	// something else
	{
		bHandled = FALSE;
		return 1;
	}
	
	CToolBarCtrl wndToolBar = pnmh->hwndFrom;
	int nIndex = wndToolBar.CommandToIndex(ID_VIEW_SETUP);
	RECT rect;
	wndToolBar.GetItemRect(nIndex, &rect);
	wndToolBar.ClientToScreen(&rect);
	
	CMenu menu;
	menu.LoadMenu(IDR_SETUPMENU);

	CMenuHandle popup = menu.GetSubMenu(0);
	popup.TrackPopupMenu(TPM_LEFTALIGN , rect.left, rect.bottom, m_hWnd);
	
//	m_view.BuildWindowMenu(menu);
	
//	m_CmdBar.TrackPopupMenu(menu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rect.left, rect.bottom);
	
	return TBDDRET_DEFAULT;
}

BOOL CClassViewFrame::PreTranslateMessage(MSG *pMsg)
{
	if(CFrameWindowImpl<CClassViewFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return FALSE;
}

BOOL CClassViewFrame::OnIdle()
{
	return FALSE;
}

void __stdcall CClassViewFrame::WorkspaceNew(LPCWSTR lpcwName)
{
//	ClearView();
	m_workName = lpcwName;
}

void __stdcall CClassViewFrame::WorkspaceClose()
{
//	ClearView();
}

void __stdcall CClassViewFrame::DocumentsUpdate()
{
	/*
	IDocuments * pDocuments = m_pApplication->GetDocuments();
	std::vector<CString> ar;
	for (int i = 0; i < pDocuments->GetCount(); i++)
	{
		ar.push_back(pDocuments->Item(i)->GetFullPath());
	}
	m_cb.clear();
	m_cb.load_sources(ar);
	RefreshView();
	*/
}

void __stdcall CClassViewFrame::DocumentSave(IDocument *doc)
{

}

void CClassViewFrame::RefreshView(HTREEITEM hItem)
{
	cbparser * cb = (cbparser*)m_wndTreeClass.GetItemData(hItem);
	if (cb == NULL)
		return;

	vfc_token_tree * tree = cb->get_tree();

	m_wndTreeClass.SetRedraw(FALSE);
	ClearChild(hItem);

	HTREEITEM hGlobalFunction = m_wndTreeClass.InsertItem(_T("Globals"),2,2,hItem,NULL);
	HTREEITEM hGlobalTypedef = m_wndTreeClass.InsertItem(_T("Globals Typedef"),14,14,hItem,NULL);
	HTREEITEM hGlobalDefine = m_wndTreeClass.InsertItem(_T("Globals Define"),12,12,hItem,NULL);

	vfc_token_array tks;
	if (!tree->get_token_array(&tks))
		return;
	for (size_t i = 0; i < tks.size(); i++)
	{
		vfc_token * tk = tks.at(i);
		if (tk && tk->get_parent() == NULL)
		{
			InsertToken(m_wndTreeClass,tk,hItem,hGlobalFunction,hGlobalTypedef,hGlobalDefine);
		}
	}

	m_wndTreeClass.SetRedraw(TRUE);
}

void CClassViewFrame::InsertToken(HWND hWndTree, vfc_token *tk, 
								  HTREEITEM hRootItem, 
								  HTREEITEM hGlobalFunction,
								  HTREEITEM hGlobalTypedef,
								  HTREEITEM hGlobalDefine)
{
	CTreeViewCtrl wndTree = hWndTree;

	int kind = tk->get_uint(vt_TokenKind);
	
	if (m_bViewExt && ( kind != vk_Class && kind != vk_Namespace && kind != vk_Enum ) )
	{
		return;
	}

	int scope = tk->get_uint(vt_Scope);
	int image = GetTokenImage(tk);
	CString text = get_string(tk,vt_Name);

	HTREEITEM root = hRootItem;

	if (tk->get_parent() != NULL ||
		kind & vk_Class || kind & vk_Namespace || kind & vk_Enum)
	{
		root = hRootItem;
	}
	else if (kind & vk_Function || kind & vk_Variable)
	{
		root = hGlobalFunction;
	}
	else if (kind & vk_Typedef)
	{
		root = hGlobalTypedef;
	}
	else
		root = hGlobalDefine;

	HTREEITEM item = wndTree.InsertItem(text,image,image,root,NULL);

	wndTree.SetItemData(item,tk->get_uint(vt_Index)+1);

	vfc_int_array vti;
	if (tk->get_token_idx(vt_Children,&vti))
	{
		for (size_t i = 0; i < vti.size(); i++)
		{
			vfc_token * child = tk->get_tree()->get_token(vti.at(i));
			InsertToken(wndTree,child,item,hGlobalFunction,hGlobalTypedef,hGlobalDefine);
		}
	}
}

void CClassViewFrame::ClearChild(HTREEITEM hRoot)
{
	HTREEITEM hItem = m_wndTreeClass.GetChildItem(hRoot);
	while(hItem)
	{
		m_wndTreeClass.DeleteItem(hItem);
		//hItem = m_wndTreeClass.GetNextSiblingItem(hItem);
		hItem = m_wndTreeClass.GetChildItem(hRoot);
	}
}

int CClassViewFrame::GetTokenImage(vfc_token *tk)
{
	int kind = tk->get_uint(vt_TokenKind);
	int scope = tk->get_uint(vt_Scope);
	int image = 2;
	CString text = get_string(tk,vt_Name);
	if (kind == vk_Class)
	{
		image = 1;
	}
	else if (kind & vk_AnyFunction)
	{
		text += get_string(tk,vt_RealArgs);
		image = 2;
		if (scope == vs_Protected)
			image = 8;
		else if (scope == vs_Private)
			image = 9;
	}
	else if (kind & vk_Variable)
	{
		text += _T(" : ");
		text += get_string(tk,vt_Type);
		image = 3;
		if (scope == vs_Protected)
			image = 10;
		else if (scope == vs_Private)
			image = 11;
	}
	else if (kind & vk_Mapping)
	{
		text += get_string(tk,vt_Args);
		image = 5;
	}
	else if (kind & vk_Enum)
	{
		image = 6;
	}
	else if (kind & vk_Enumerator)
	{
		image = 7;
	}
	else if (kind & vk_Namespace)
	{
		image = 13;
	}
	else if (kind & vk_Typedef)
	{
		image = 14;
	}
	return image;
}

LRESULT CClassViewFrame::OnNmDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	if (pnmh->hwndFrom == m_wndTreeClass)
	{
		bHandled = TRUE;
		PostMessage(WM_COMMAND,ID_GOTO_DEFINE,0);
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CClassViewFrame::OnGotoDefine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM item = NULL;
	if (!m_bViewExt)
		item = m_wndTreeClass.GetSelectedItem();
	if (item)
	{
		vfc_token * tk = GetToken(m_wndTreeClass,item);
		if (tk)
		{
			GotoFile(tk,true);
		}
	}
	return 0;
}

LRESULT CClassViewFrame::OnGotoDeclare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM item = NULL;
	if (!m_bViewExt)
		item = m_wndTreeClass.GetSelectedItem();
	vfc_token * tk = NULL;
	if (item && (tk = GetToken(m_wndTreeClass,item)) != NULL)
	{
		GotoFile(tk,false);
	}

	return 0;
}

vfc_token * CClassViewFrame::GetToken(HWND hWndTree, HTREEITEM hItem)
{
	CTreeViewCtrl wndTree = hWndTree;
	HTREEITEM hRoot = wndTree.GetParentItem(hItem);
	HTREEITEM hParent = hRoot;
	while(hRoot)
	{
		hParent = wndTree.GetParentItem(hRoot);
		if (hParent == TVI_ROOT || hParent == NULL)
			break;
		hRoot = hParent;
	}

	if (hRoot == NULL)
		return NULL;

	cbparser * cb = (cbparser*)wndTree.GetItemData(hRoot);
	int id = wndTree.GetItemData(hItem);
	if (cb && id > 0)
	{
		id--;
		return cb->get_tree()->get_token(id);
	}

	return NULL;
}

void CClassViewFrame::GotoFile(vfc_token *tk, bool bImpl)
{
	int kind = tk->get_uint(vt_TokenKind);
	CString filename;
	int line = 0;

	if (kind == vk_Function && bImpl)
	{
		filename = get_string(tk,vt_ImplFileName);
		line = tk->get_uint(vt_ImplLine);
		if (filename.IsEmpty())
		{
			filename = get_string(tk,vt_FileName);
			line = tk->get_uint(vt_Line);
		}
	}
	else
	{
		filename = get_string(tk,vt_FileName);
		line = tk->get_uint(vt_Line);
	}

	IDocument * pDoc = m_pApplication->GetDocuments()->Open(filename,false);

	if (pDoc)
	{
		m_pApplication->GetDocuments()->Activate(pDoc);
		IEditor* pEditor = pDoc->GetEditor();
		if (pEditor)
		{
			ITextSelection* pTextSel = pEditor->GetTextSelection();
			if (pTextSel)
			{
				pTextSel->GoToLine(line);
			}
		}
	}
}

void __stdcall CClassViewFrame::ProjectAdd(IProject * proj)
{
	TCHAR name[MAX_PATH];
	proj->GetName(name,MAX_PATH);
	HTREEITEM hItem = m_wndTreeClass.InsertItem(name,TVI_ROOT,NULL);
	if (hItem)
	{
		cbparser * cb = new cbparser();
		cb->m_proj = proj;
		m_cbs.push_back(cb);
		m_wndTreeClass.SetItemData(hItem,(DWORD)cb);
	}
}

void __stdcall CClassViewFrame::ProjectUpdate(IProject *proj)
{
	HTREEITEM hItem = NULL;
	cbparser * cb = NULL;
	do
	{
		hItem = m_wndTreeClass.GetChildItem(TVI_ROOT);
		if (hItem)
		{
			cb = (cbparser*)m_wndTreeClass.GetItemData(hItem);
			if (cb && cb->m_proj == proj)
			{
				break;
			}
			hItem = m_wndTreeClass.GetNextSiblingItem(hItem);
		}
	} while (hItem);

	if (hItem == NULL)
	{
		return;
	}

	std::vector<CString> ar;
	TCHAR path[MAX_PATH] = {0};
	for (long i = 0; i < proj->GetCount(); i++)
	{
		proj->GetItem(i)->GetFullPath(path,MAX_PATH);
		ar.push_back(path);
	}
	cb->clear();
	cb->load_sources(ar);
	RefreshView(hItem);
}

void CClassViewFrame::RefreshAllView()
{
	HTREEITEM hItem = m_wndTreeClass.GetChildItem(TVI_ROOT);
	while(hItem)
	{
		RefreshView(hItem);
		hItem = m_wndTreeClass.GetNextSiblingItem(hItem);
	}
}
