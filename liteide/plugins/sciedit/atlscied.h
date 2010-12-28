#ifndef _ATLSCIED_H_
#define _ATLSCIED_H_

/*
Update:
Filename: atlscied.h
Data: 4/1/2009
by visualfc@gmail.com
*/

/*
Filename: AtlScintilla.h
Description: Defines an easy wrapper for the Scintilla control, to be used with ATL/WTL projects.
Date: 12/15/2005

Copyright (c) 2005 by Gilad Novik.
Copyright (c) 2006 by Reece Dunn.

License Agreement (zlib license)
-------------------------
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

* If you use this code in a product, I would appreciate a small email from.

Gilad Novik (Web: http://gilad.gsetup.com, Email: gilad@gsetup.com)
*/

#pragma once
#include "AtlScintilla.h"
#include <public/wtlfile.h>

#pragma comment(lib, "Imm32.lib")

template <typename TBase>
class CSciEditCtrlT : public CScintillaWindowT<TBase>
{
public:
    using CScintillaWindowT<TBase>::m_hWnd;
// Constructors
	CSciEditCtrlT(HWND hWnd = NULL)
	{
		m_hWnd = hWnd;
	}

	CSciEditCtrlT< TBase >& operator =(HWND hWnd)
	{
		m_hWnd = hWnd;
		return *this;
	}

	HWND Create(HWND hWndParent, ATL::_U_RECT rect = NULL, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			ATL::_U_MENUorID MenuOrID = 0U, LPVOID lpCreateParam = NULL)
	{
		return TBase::Create(GetWndClassName(), hWndParent, rect.m_lpRect, szWindowName, dwStyle, dwExStyle, MenuOrID.m_hMenu, lpCreateParam);
	}

// Attributes
	static LPCTSTR GetWndClassName()
	{
		return _T("Scintilla");
	}

	static LPCTSTR GetLibraryName()
	{
		return _T("SciLexer.DLL");
	}

	BOOL LoadFile(LPCTSTR szFilename)
	{
		CFile File;
		if (!File.Open(szFilename,CFile::modeRead | CFile::shareDenyRead,NULL))
			return FALSE;

		SetUndoCollection(false);
		static BYTE UTF8[]={0xEF,0xBB,0xBF};
		char szBuffer[8192];
		DWORD dwRead;
		if ( (dwRead = File.Read(szBuffer,sizeof(UTF8)) ) && dwRead>0)
		{
            if (dwRead!=sizeof(UTF8) || memcmp(szBuffer,UTF8,sizeof(UTF8)))
			{
				SetCodePage(GetACP());
				AddText(szBuffer,(int)dwRead);
			}
			else
				SetCodePage(SC_CP_UTF8);
		}
		for (;( dwRead = File.Read(szBuffer,sizeof(szBuffer))) && dwRead>0;AddText(szBuffer,(int)dwRead));
		SetSel(0,0);
		SetUndoCollection(true);
		SetSavePoint();
		SetReadOnly((GetFileAttributes(szFilename) & FILE_ATTRIBUTE_READONLY) ? true : false);
		return TRUE;
	}

    BOOL SaveFile(LPCTSTR szFilename)
	{
		CFile File;
		if (!File.Open(szFilename,CFile::modeCreate | CFile::modeWrite,NULL))
			return FALSE;
		char szBuffer[8192] = {0};
		TextRange trRange;
		trRange.lpstrText=szBuffer;
		DWORD dwWritten;
		if (GetCodePage()==SC_CP_UTF8)
		{
			static BYTE UTF8[]={0xEF,0xBB,0xBF};
			dwWritten = File.WriteN(UTF8,sizeof(UTF8));
			if (dwWritten != sizeof(UTF8))
				return FALSE;
		}
		for (int nIndex=0,nLength=GetLength();nIndex<nLength;nIndex+=(int)dwWritten)
		{
			dwWritten=(DWORD)(nLength-nIndex)>sizeof(szBuffer) ? sizeof(szBuffer) : (DWORD)(nLength-nIndex);
			trRange.chrg.cpMin=nIndex;
			trRange.chrg.cpMax=nIndex+(long)dwWritten;
			GetTextRange(trRange);
			if (File.WriteN(szBuffer,dwWritten) != dwWritten)
				return FALSE;
		}
		SetSavePoint();
		return TRUE;
	}
};

typedef CSciEditCtrlT<CWindow>	CSciEditCtrl;

#endif //_ATLSCIEDI_H_
