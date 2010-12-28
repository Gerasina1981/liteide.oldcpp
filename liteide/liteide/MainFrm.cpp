// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"
#include <public/wtlutil.h>
#include <public/dspparser.h>

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_Documents.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	m_Documents.UIUpdateTitle();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	CreateSimpleStatusBar();

	m_hWndClient = m_splitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pane.SetPaneContainerExtendedStyle(PANECNT_NOBORDER);
	m_pane.Create(m_splitter, _T("Workspace"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_Workspace.Create(m_pane, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN , WS_EX_CLIENTEDGE);
	m_Workspace.SetFont(AtlGetDefaultGuiFont());
	m_pane.SetClient(m_Workspace);
/*
	m_Editspace.Create(m_splitter, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_Editspace.SetFont(AtlGetDefaultGuiFont());
	m_Editspace.SetTitleBarWindow(m_hWnd);

	m_splitter.SetSplitterPanes(m_pane, m_Editspace);
*/
	m_Documents.Create(m_splitter, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_Documents.SetFont(AtlGetDefaultGuiFont());
	m_Documents.SetTitleBarWindow(m_hWnd);

	m_splitter.SetSplitterPanes(m_pane, m_Documents);

	UpdateLayout();
	m_splitter.SetSplitterPosPct(25);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);
	UISetCheck(ID_VIEW_TREEPANE, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	CMenuHandle menuMain = m_CmdBar.GetMenu();
//	m_Editspace.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));
	m_Documents.SetWindowMenu(menuMain.GetSubMenu(WINDOW_MENU_POSITION));

	//drag files
	DragAcceptFiles(TRUE);


	m_PluginManager.LoadPlugins(GetModulePath(_Module.m_hInst)+_T("\\plugins"));
	m_PluginManager.ConnectionAll(this,NULL);
	
//	FireWorkspaceNew(_T("Undefine Workspace"));
//	this->m_Solution.Open(_T("Undefine"));
//	this->m_Solution.m_projs.AddProject(_T("Undefine"));
	this->m_Solution.Create(_T("Undefine"));

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
//	FireWorkspaceClose();
	m_PluginManager.DisconnectionAll(NULL);
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document
/*
	if (!m_Editspace.QueryCloseAll())
		return 0;

	m_Documents.CloseAll();
//	FireWorkspaceNew(_T("Undefine Workspace"));
*/
	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnViewTreePane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	bool bShow = (m_splitter.GetSinglePaneMode() != SPLIT_PANE_NONE);
	m_splitter.SetSinglePaneMode(bShow ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);
	UISetCheck(ID_VIEW_TREEPANE, bShow);

	return 0;
}

LRESULT CMainFrame::OnTreePaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
{
	if(hWndCtl == m_pane.m_hWnd)
	{
		m_splitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);
		UISetCheck(ID_VIEW_TREEPANE, 0);
	}

	return 0;
}

LRESULT CMainFrame::OnWindowClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	IDocument * pDoc = m_Documents.GetActivate();
	if (pDoc)
	{
		m_Documents.Close(pDoc,liSaveChangesPrompt);
	}
	return 0;
}

LRESULT CMainFrame::OnWindowCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_Documents.CloseAll(liSaveChangesPrompt);
	return 0;
}

LRESULT CMainFrame::OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nPage = wID - ID_WINDOW_TABFIRST;
	m_Documents.SetActivePage(nPage);

	return 0;
}

LRESULT CMainFrame::OnFileOpen(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	CMultiFileDialog dlg(NULL,NULL,OFN_HIDEREADONLY ,
		_T("C++ Files (*.h;*hpp;*.hxx;*.c;*.cpp;*.cxx)\0*.h;*.hpp;*.hxx;*.c;*.cxx;*.cpp;\0\
			Import VC Projects (*.dsp;*.vcproj)\0*.dsp;*.vcproj;\0\
			All Files (*.*)\0*.*\0"));
	if (dlg.DoModal() == IDOK)
	{
		CString path;
		bool ret = dlg.GetFirstPathName(path);
		if (!ret)
			return 0;
		//test is vc project file
		if (LoadVCProject(path))
			return 0;
		
		IProject * proj = this->GetSolution()->GetActivate();
		if (proj == NULL)
		{
			proj = this->GetSolution()->CreateProject(_T("Undefine"));
		}
		while (ret)
		{
			proj->AddFile(path);
			ret = dlg.GetNextPathName(path);
		}
		this->m_Solution.FireProjectUpdate(proj);
	}
	return 0;
}

LRESULT CMainFrame::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDROP hDropInfo = (HDROP)wParam;
	TCHAR szPath[MAX_PATH] = {0};
	int nFiles = ::DragQueryFile(hDropInfo,-1,NULL,0);
	for (int i = 0; i < nFiles; i++)
	{
		DragQueryFile(hDropInfo,i,szPath,MAX_PATH);

		if (!::PathFileExists(szPath) || ::PathIsDirectory(szPath))
			continue;

		if (nFiles == 1 && LoadVCProject(szPath))
		{
			return 0;
		}
	}
	::DragFinish(hDropInfo);

	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
	IDocument* pDoc = m_Documents.GetActivate();
	if (pDoc)
	{
		pDoc->GetEditor()->Save();
	}
	return 0;
}

LRESULT CMainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_Documents.CloseAll(liSaveChangesPrompt))
	{
		bHandled = FALSE;
	}
	return 0;
}

BOOL CMainFrame::LoadVCProject(LPCTSTR lpszFileName)
{
	CString ext = PathFindExtension(lpszFileName);
	ext.MakeLower();
	//import vs6 projects
	if (ext == _T(".dsp") || ext == _T(".vcproj"))
	{
		dspparser dsp;
		if ( (ext == _T(".dsp") && dsp.load_dsp(lpszFileName) ) ||
			(ext == _T(".vcproj") && dsp.load_vcproj(lpszFileName) ) )
		{
//			FireWorkspaceNew(PathFindFileName(lpszFileName));
			for (int i = 0; i < dsp.source.GetSize(); i++)
			{
				if (::PathFileExists(dsp.source[i]))
				{

				}
			}
			return TRUE;
		}
	}
	return FALSE;
}
