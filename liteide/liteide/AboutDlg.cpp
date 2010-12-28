// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
char proj_info[] =
"Simple C++ IDE\r\n"	\
"Version 0.1\r\n"	\
"    " __DATE__ "  " __TIME__"\r\n\r\n"	\
"http://code.google.com/liteide\r\n"\
"visualfc@gmail.com\r\n"	\
"xushiweizh@gmail.com\r\n\r\n"	\
"Links\r\n"\
"http://www.sourceforge.net/projects/wtl\r\n"\
"http://www.scintilla.org\r\n"\
"http://code.google.com/p/stdext\r\n"\
"http://code.google.com/p/tpl\r\n"\
"http://code.google.com/p/visualfc\r\n"\
"";

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	
	CEdit info = GetDlgItem(IDC_INFO_EDIT);
	USES_CONVERSION;
	info.SetWindowText(A2T(proj_info));

	DlgFont_Init();

	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
