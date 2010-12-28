// FileView.cpp: implementation of the CFileView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileView.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileView::CFileView()
{

}

CFileView::~CFileView()
{

}

LRESULT CFileView::OnNmDblclkTree(int idCtrl, LPNMHDR pnmh, BOOL &bHandled)
{
	HTREEITEM hItem = GetSelectedItem();
	if (hItem)
	{
		IProjectItem * item = (IProjectItem*)GetItemData(hItem);
		if (item)
		{
			TCHAR path[MAX_PATH] = {0};
			item->GetFullPath(path,MAX_PATH);
			m_pApplication->GetDocuments()->Open(path,false);
		}
	}
	return -1;
}

void __stdcall CFileView::ProjectAdd(IProject *proj)
{
 	TCHAR name[MAX_PATH];
 	proj->GetName(name,MAX_PATH);
 	m_itemRoot		= InsertItem(name,TVI_ROOT,NULL);
	SetItemData(m_itemRoot,(DWORD)proj);

 	m_itemSource	= InsertItem(_T("Source Files"),m_itemRoot,NULL);
 	m_itemHeader	= InsertItem(_T("Header Files"),m_itemRoot,NULL);
 	m_itemResource	= InsertItem(_T("Resource Files"),m_itemRoot,NULL);
 
 	Expand(m_itemRoot);
}

void __stdcall CFileView::ProjectUpdate(IProject *proj)
{

}

void __stdcall CFileView::ProjectItemAdd(IProject *proj, IProjectItem *item)
{
	HTREEITEM hItem = GetChildItem(TVI_ROOT);
	while(hItem)
	{
		if ((IProject*)GetItemData(hItem) == proj)
		{
			TCHAR path[MAX_PATH] = {0};
			item->GetFullPath(path,MAX_PATH);
			CString ext = PathFindExtension(path);
			ext.MakeLower();
			
			HTREEITEM root = m_itemRoot;
			if (ext == _T(".h") || ext == _T(".hpp"))
			{
				root = m_itemHeader;
			}
			else if (ext == _T(".c") || ext == _T(".cpp"))
			{
				root = m_itemSource;
			}
			
			HTREEITEM hItem = InsertItem(PathFindFileName(path),root,NULL);
			if (hItem)
			{
				SetItemData(hItem,(LONG)item);
			}
		 	Expand(root);
			break;
		}
		hItem = GetNextSiblingItem(hItem);
	}
}
