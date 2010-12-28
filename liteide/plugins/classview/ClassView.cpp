// ClassView.cpp: implementation of the CClassView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClassView.h"
#include "resource.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClassView::CClassView()
{
}

CClassView::~CClassView()
{

}

LRESULT CClassView::OnNmDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL &bHandled)
{
	bHandled = TRUE;
	PostMessage(WM_COMMAND,ID_GOTO_DEFINE,0);
	return -1;
}

LRESULT CClassView::OnNmRclickTree(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	bHandled = FALSE;
	CPoint pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	UINT uFlags;
	HTREEITEM item = HitTest(pt, &uFlags);
	if ((item != NULL) && (TVHT_ONITEM & uFlags))
	{
		SelectItem(item);

		CMenu menu;
		menu.LoadMenu(IDR_POPUPMENU);
		CMenuHandle popup = menu.GetSubMenu(0);
	
		ClientToScreen(&pt);
		popup.TrackPopupMenu(TPM_LEFTALIGN ,pt.x,pt.y,m_hWnd);
	}
	return -1;
}

LRESULT CClassView::OnGotoDefine(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM item = GetSelectedItem();
	if (item)
	{
		vfc_token * tk = GetToken(item);
		if (tk)
		{
			GotoFile(tk,true);
		}
	}
	return 0;
}

LRESULT CClassView::OnGotoDeclare(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	HTREEITEM item = GetSelectedItem();
	vfc_token * tk = NULL;
	if (item && (tk = GetToken(item)) != NULL)
	{
		GotoFile(tk,false);
	}

	return 0;
}

//DEL void __stdcall CClassView::WorkspaceClose()
//DEL {
//DEL 	DeleteAllItems();
//DEL 	m_cb.clear();
//DEL }

//DEL void __stdcall CClassView::WorkspaceNew(LPCWSTR lpcwName)
//DEL {
//DEL 	DeleteAllItems();
//DEL 	m_workName = lpcwName;
//DEL 	m_cb.clear();
//DEL }

//DEL void __stdcall CClassView::DocumentsUpdate()
//DEL {
//DEL 	IDocuments * pDocuments = m_pApplication->GetDocuments();
//DEL 	std::vector<CString> ar;
//DEL 	for (int i = 0; i < pDocuments->GetCount(); i++)
//DEL 	{
//DEL 		ar.push_back(pDocuments->Item(i)->GetFullPath());
//DEL 	}
//DEL 	m_cb.clear();
//DEL 	m_cb.load_sources(ar);
//DEL 	RefreshTree();
//DEL }

//DEL void __stdcall CClassView::DocumentSave(IDocument* doc)
//DEL {
//DEL 	DocumentsUpdate();
//DEL }

void CClassView::RefreshTree()
{
	vfc_token_tree * tree = m_cb.get_tree();

	SetRedraw(FALSE);

	DeleteAllItems();
	m_itemRoot	= InsertItem(m_workName,0,0,TVI_ROOT,NULL);
	m_itemGlobalFunction = InsertItem(_T("Globals"),2,2,m_itemRoot,NULL);
	m_itemGlobalTypedef = InsertItem(_T("Globals Typedef"),14,14,m_itemRoot,NULL);
	m_itemGlobalDefine = InsertItem(_T("Globals Define"),12,12,m_itemRoot,NULL);

	vfc_token_array tks;
	if (!tree->get_token_array(&tks))
		return;
	for (size_t i = 0; i < tks.size(); i++)
	{
		vfc_token * tk = tks.at(i);
		if (tk && tk->get_parent() == NULL)
		{
			InsertToken(tk,m_itemRoot);
		}
	}

	Expand(m_itemRoot);

	SetRedraw(TRUE);
}

static CString get_string(vfc_token * tk, int vt)
{
	CString text;
	tk->get_string(vt,text.GetBufferSetLength(MAX_PATH),MAX_PATH);
	return text;
}

void CClassView::InsertToken(vfc_token *tk, HTREEITEM root)
{
	int kind = tk->get_uint(vt_TokenKind);
	int scope = tk->get_uint(vt_Scope);
	int image = 2;
	CString text = get_string(tk,vt_Name);
	text.ReleaseBuffer(-1);
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

	if (tk->get_parent() != NULL ||
		kind & vk_Class || kind & vk_Namespace || kind & vk_Enum)
	{
		root = root;
	}
	else if (kind & vk_Function || kind & vk_Variable)
	{
		root = m_itemGlobalFunction;
	}
	else if (kind & vk_Typedef)
	{
		root = m_itemGlobalTypedef;
	}
	else
		root = m_itemGlobalDefine;

	HTREEITEM item = InsertItem(text,image,image,root,NULL);
	SetItemData(item,tk->get_uint(vt_Index)+1);

	vfc_int_array vti;
	if (tk->get_token_idx(vt_Children,&vti))
	{
		for (size_t i = 0; i < vti.size(); i++)
		{
			vfc_token * child = tk->get_tree()->get_token(vti.at(i));
			InsertToken(child,item);
		}
	}
}

void CClassView::InitImageList()
{
	CImageList il;
	il.CreateFromImage(IDB_PROJIMAGE, 16, 1, RGB(255, 0, 255), IMAGE_BITMAP, LR_CREATEDIBSECTION);
	SetImageList(il);
}

void CClassView::GotoFile(vfc_token *tk, bool bImpl)
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

	/*
	IDocument* pDoc = m_pApplication->GetDocuments()->GetDocument(filename);
	if (pDoc)
	{
		m_pApplication->GetDocuments()->Activate(pDoc);
		IEditor* pEditor = m_pApplication->GetEditspace()->GetEditor(pDoc);
		if (pEditor)
		{
			ITextSelection* pTextSel = pEditor->GetTextSelection();
			if (pTextSel)
			{
				pTextSel->GoToLine(line);
			}
		}
	}
	*/
}

vfc_token * CClassView::GetToken(HTREEITEM item)
{
	if (item)
	{
		int id = GetItemData(item);
		if (id == 0)
			return NULL;
		id--;
		return m_cb.get_tree()->get_token(id);
	}
	return NULL;
}

void __stdcall CClassView::ProjectAdd(IProject *proj)
{
	TCHAR path[MAX_PATH];
	proj->GetFullPath(path,MAX_PATH);
	InsertItem(path,TVI_ROOT,NULL);
}
