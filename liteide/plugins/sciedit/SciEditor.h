// SciEditor.h: interface for the CSciEditor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCIEDITOR_H__FE8D35E0_F4E6_4F24_A38D_A71CFE797D73__INCLUDED_)
#define AFX_SCIEDITOR_H__FE8D35E0_F4E6_4F24_A38D_A71CFE797D73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <liteide/AppImpl.h>
#include <public/wtlfile.h>
#include "atlscied.h"

class CSciEditor : public IEditorImpl, public ITextSelectionImpl, 
				   public CWindowImpl<CSciEditor, CSciEditCtrl> 
{
public:
	typedef CWindowImpl<CSciEditor, CSciEditCtrl> _base;
	virtual HWND __stdcall GetHwnd()
	{
		return m_hWnd;
	}

	virtual LPCWSTR __stdcall GetFullPath()
	{
		return m_path;
	}
	virtual bool __stdcall Close(enum liSaveChanges lisc)
	{
		if (lisc == liSaveChangesNo)
		{
		}
		else if (lisc == liSaveChangesYes)
		{
			Save();
		}
		else if (lisc == liSaveChangesPrompt && GetModify())
		{
			CString text = _T("Save change to ");
			text += m_path;
			UINT ret = MessageBox(text,_T("SciEditor"),MB_YESNOCANCEL);
			if (ret == IDCANCEL)
			{
				return false;
			}
			else if (ret == IDYES)
			{
				Save();
			}
		}
		return true;
	}
	virtual void __stdcall Save()
	{
		SaveFile(m_path);
	}
	virtual bool __stdcall GetModify()
	{
		return _base::GetModify();
	}

	virtual ITextSelection* __stdcall GetTextSelection()
	{
		return static_cast<ITextSelection*>(this);
	}

	// ITextSelection
	virtual void __stdcall GoToLine(long line)
	{
		int first = GetFirstVisibleLine();
		line = line-1;
		SendMessage(m_hWnd,SCI_ENSUREVISIBLEENFORCEPOLICY, line,0);
		SendMessage(m_hWnd,SCI_GOTOLINE, line,0);
	}
	virtual void __stdcall MoveTo(long line, long column)
	{
	}

public:
	DECLARE_WND_SUPERCLASS(NULL, CSciEditCtrl::GetWndClassName())

public:
	CString m_path;

	CSciEditor();
	virtual ~CSciEditor();

	BEGIN_MSG_MAP(CSciEditor)
	END_MSG_MAP()

	virtual void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	}
};

class CEditorService : public ObjectImplT<IEditorService>
{
public:
	virtual IEditor* __stdcall	CreateEditor(HWND hWnd, LPCWSTR lpcwPath)
	{
		CSciEditor * pEdit = new CSciEditor();
		if (pEdit->Create(hWnd,CWindow::rcDefault,NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | 
			WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL , 
			WS_EX_CLIENTEDGE) && pEdit->LoadFile(lpcwPath) )
		{
			pEdit->m_path = lpcwPath;
			pEdit->SetLexerLanguage("cpp");
			return pEdit;
		}

		delete pEdit;
		return NULL;
	} 

	virtual LPCWSTR __stdcall GetEditorInfo()
	{
		return L"Sci Editor";
	}
	
	IApplication	* m_pApplication;
};

#endif // !defined(AFX_SCIEDITOR_H__FE8D35E0_F4E6_4F24_A38D_A71CFE797D73__INCLUDED_)

