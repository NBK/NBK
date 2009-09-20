// TargetDlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dxwndhost.h"
#include "TargetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTargetDlg ダイアログ


CTargetDlg::CTargetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTargetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTargetDlg)
	m_DXVersion = -1;
	m_EmulatePal = FALSE;
	m_HookDI = FALSE;
	m_ModifyMouse = FALSE;
	m_OutTrace = FALSE;
	m_UnNotify = FALSE;
	m_FilePath = _T("");
	m_SaveLoad = FALSE;
	m_InitX = 0;
	m_InitY = 0;
	m_MaxX = 0;
	m_MaxY = 0;
	m_MinX = 0;
	m_MinY = 0;
	//}}AFX_DATA_INIT
}


void CTargetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetDlg)
	DDX_Control(pDX, IDC_FILE, m_File);
	DDX_Radio(pDX, IDC_AUTO, m_DXVersion);
	DDX_Check(pDX, IDC_EMULATEPAL, m_EmulatePal);
	DDX_Check(pDX, IDC_HOOKDI, m_HookDI);
	DDX_Check(pDX, IDC_MODIFYMOUSE, m_ModifyMouse);
	DDX_Check(pDX, IDC_OUTTRACE, m_OutTrace);
	DDX_Check(pDX, IDC_UNNOTIFY, m_UnNotify);
	DDX_Text(pDX, IDC_FILE, m_FilePath);
	DDX_Check(pDX, IDC_SAVELOAD, m_SaveLoad);
	DDX_Text(pDX, IDC_INITX, m_InitX);
	DDX_Text(pDX, IDC_INITY, m_InitY);
	DDX_Text(pDX, IDC_MAXX, m_MaxX);
	DDX_Text(pDX, IDC_MAXY, m_MaxY);
	DDX_Text(pDX, IDC_MINX, m_MinX);
	DDX_Text(pDX, IDC_MINY, m_MinY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetDlg, CDialog)
	//{{AFX_MSG_MAP(CTargetDlg)
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetDlg メッセージ ハンドラ

void CTargetDlg::OnOpen() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
    char path[MAX_PATH];
	m_File.GetWindowText(path, MAX_PATH);
	CFileDialog dlg( TRUE, "*.*", path, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        "Program (*.exe)|*.exe|All Files (*.*)|*.*||",  this);
    if( dlg.DoModal() == IDOK) m_File.SetWindowText(dlg.GetPathName());
}
