// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(VFC_ABOUTDLG_H__BAAA3F80_260C_4dcb_B6AC_1FF95CF8BAB6__INCLUDED_)
#define VFC_ABOUTDLG_H__BAAA3F80_260C_4dcb_B6AC_1FF95CF8BAB6__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <public/vfcfonts.h>


class CAboutDlg : public CDialogImpl<CAboutDlg>, public CDialogFont<CAboutDlg>
{
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	BEGIN_DLGFONT_MAP(CAboutDlg)
		DLGFONT_CONTROL_EX(IDC_INFO_EDIT, -14,0,0,0,400,1,0,0,0,3,2,1,34,_T("Microsoft Sans Serif"))
	END_DLGFONT_MAP()
	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// VisualFC AppWizard will insert additional declarations immediately before the previous line.

#endif // !defined(VFC_ABOUTDLG_H__BAAA3F80_260C_4dcb_B6AC_1FF95CF8BAB6__INCLUDED_)
