// dxwndhostView.cpp : CDxwndhostView クラスの動作の定義を行います。
//


#include "stdafx.h"
#include "shlwapi.h"

#include "dxwndhost.h"

#include "dxwndhostDoc.h"
#include "dxwndhostView.h"
#include "TargetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDxwndhostView

IMPLEMENT_DYNCREATE(CDxwndhostView, CListView)

BEGIN_MESSAGE_MAP(CDxwndhostView, CListView)
	//{{AFX_MSG_MAP(CDxwndhostView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_ADD, OnAdd)
	ON_COMMAND(ID_MODIFY, OnModify)
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_WM_RBUTTONDOWN()
	ON_WM_DESTROY()
	ON_COMMAND(ID_RUN, OnRun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxwndhostView クラスの構築/消滅

CDxwndhostView::CDxwndhostView()
{
}

CDxwndhostView::~CDxwndhostView()
{
	int i;
	char key[32], val[32];
	EndHook();
	for(i = 0; i < MAXTARGETS; i ++){
		if(!TargetMaps[i].path[0]) break;
		sprintf(key, "path%i", i);
		WritePrivateProfileString("target", key, TargetMaps[i].path, InitPath);
		sprintf(key, "ver%i", i);
		sprintf(val, "%i", TargetMaps[i].dxversion);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "flag%i", i);
		sprintf(val, "%i", TargetMaps[i].flags);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "initx%i", i);
		sprintf(val, "%i", TargetMaps[i].initx);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "inity%i", i);
		sprintf(val, "%i", TargetMaps[i].inity);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "minx%i", i);
		sprintf(val, "%i", TargetMaps[i].minx);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "miny%i", i);
		sprintf(val, "%i", TargetMaps[i].miny);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "maxx%i", i);
		sprintf(val, "%i", TargetMaps[i].maxx);
		WritePrivateProfileString("target", key, val, InitPath);
		sprintf(key, "maxy%i", i);
		sprintf(val, "%i", TargetMaps[i].maxy);
		WritePrivateProfileString("target", key, val, InitPath);
	}
	for(; i < MAXTARGETS; i ++){
		sprintf(key, "path%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "ver%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "flag%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "initx%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "inity%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "minx%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "miny%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "maxx%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
		sprintf(key, "maxy%i", i);
		WritePrivateProfileString("target", key, 0, InitPath);
	}	
}

BOOL CDxwndhostView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER;
	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDxwndhostView クラスの描画

void CDxwndhostView::OnDraw(CDC* pDC)
{
	CDxwndhostDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

void CDxwndhostView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();
	CListCtrl& listctrl = GetListCtrl();
	LV_COLUMN listcol;
	LV_ITEM listitem;
	int i;
	char key[32];

	listcol.mask = LVCF_WIDTH;
	listcol.cx = 100;
	
	listctrl.InsertColumn(0, &listcol);
	GetCurrentDirectory(MAX_PATH, InitPath);
	strcat(InitPath, "\\dxwnd.ini");
	for(i = 0; i < MAXTARGETS; i ++){
		sprintf(key, "path%i", i);
		GetPrivateProfileString("target", key, "", TargetMaps[i].path, MAX_PATH, InitPath);
		if(!TargetMaps[i].path[0]) break;
		sprintf(key, "ver%i", i);
		TargetMaps[i].dxversion = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "flag%i", i);
		TargetMaps[i].flags = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "initx%i", i);
		TargetMaps[i].initx = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "inity%i", i);
		TargetMaps[i].inity = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "minx%i", i);
		TargetMaps[i].minx = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "miny%i", i);
		TargetMaps[i].miny = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "maxx%i", i);
		TargetMaps[i].maxx = GetPrivateProfileInt("target", key, 0, InitPath);
		sprintf(key, "maxy%i", i);
		TargetMaps[i].maxy = GetPrivateProfileInt("target", key, 0, InitPath);
		listitem.mask = LVIF_TEXT;
		listitem.iItem = i;
		listitem.iSubItem = 0;
		listitem.pszText = TargetMaps[i].path;
		listctrl.InsertItem(&listitem);
	}
	for(; i < MAXTARGETS; i ++) TargetMaps[i].path[0] = 0;
	Resize();
	SetTarget(TargetMaps);
	StartHook();
}

/////////////////////////////////////////////////////////////////////////////
// CDxwndhostView クラスの診断

#ifdef _DEBUG
void CDxwndhostView::AssertValid() const
{
	CListView::AssertValid();
}

void CDxwndhostView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CDxwndhostDoc* CDxwndhostView::GetDocument() // 非デバッグ バージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDxwndhostDoc)));
	return (CDxwndhostDoc*)m_pDocument;
}
#endif //_DEBUG


void CDxwndhostView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	OnRun();
	*pResult = 0;
}

void CDxwndhostView::OnModify() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	int i;
	CTargetDlg dlg;
	POSITION pos;
	LV_ITEM listitem;
	CListCtrl& listctrl = GetListCtrl();

	if(!listctrl.GetSelectedCount()) return;
	pos = listctrl.GetFirstSelectedItemPosition();
	i = listctrl.GetNextSelectedItem(pos);
	dlg.m_DXVersion = TargetMaps[i].dxversion;
	if(dlg.m_DXVersion > 6) dlg.m_DXVersion -= 5;
	dlg.m_FilePath = TargetMaps[i].path;
	dlg.m_UnNotify = TargetMaps[i].flags & UNNOTIFY ? 1 : 0;
	dlg.m_EmulatePal = TargetMaps[i].flags & EMULATEPAL ? 1 : 0;
	dlg.m_HookDI = TargetMaps[i].flags & HOOKDI ? 1 : 0;
	dlg.m_ModifyMouse = TargetMaps[i].flags & MODIFYMOUSE ? 1 : 0;
	dlg.m_OutTrace = TargetMaps[i].flags & OUTTRACE ? 1 : 0;
	dlg.m_SaveLoad = TargetMaps[i].flags & SAVELOAD ? 1 : 0;
	dlg.m_InitX = TargetMaps[i].initx;
	dlg.m_InitY = TargetMaps[i].inity;
	dlg.m_MinX = TargetMaps[i].minx;
	dlg.m_MinY = TargetMaps[i].miny;
	dlg.m_MaxX = TargetMaps[i].maxx;
	dlg.m_MaxY = TargetMaps[i].maxy;
	if(dlg.DoModal() == IDOK && dlg.m_FilePath.GetLength()){
		strcpy(TargetMaps[i].path, dlg.m_FilePath);
		if(dlg.m_DXVersion > 1) dlg.m_DXVersion += 5;
		TargetMaps[i].dxversion = dlg.m_DXVersion;
		TargetMaps[i].flags = 0;
		if(dlg.m_UnNotify) TargetMaps[i].flags |= UNNOTIFY;
		if(dlg.m_EmulatePal) TargetMaps[i].flags |= EMULATEPAL;
		if(dlg.m_HookDI) TargetMaps[i].flags |= HOOKDI;
		if(dlg.m_ModifyMouse) TargetMaps[i].flags |= MODIFYMOUSE;
		if(dlg.m_OutTrace) TargetMaps[i].flags |= OUTTRACE;
		if(dlg.m_SaveLoad) TargetMaps[i].flags |= SAVELOAD;
		TargetMaps[i].initx = dlg.m_InitX;
		TargetMaps[i].inity = dlg.m_InitY;
		TargetMaps[i].minx = dlg.m_MinX;
		TargetMaps[i].miny = dlg.m_MinY;
		TargetMaps[i].maxx = dlg.m_MaxX;
		TargetMaps[i].maxy = dlg.m_MaxY;
		CListCtrl& listctrl = GetListCtrl();
		listitem.mask = LVIF_TEXT;
		listitem.iItem = i;
		listitem.iSubItem = 0;
		listitem.pszText = TargetMaps[i].path;
		listctrl.SetItem(&listitem);
		Resize();
		SetTarget(TargetMaps);		
	}
}

void CDxwndhostView::OnAdd() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	int i;
	CTargetDlg dlg;
	LV_ITEM listitem;

	dlg.m_DXVersion = 0;
	dlg.m_MaxX = 639;
	dlg.m_MaxY = 479;
	for(i = 0; i < MAXTARGETS; i ++) if(!TargetMaps[i].path[0]) break;
	if(dlg.DoModal() == IDOK && dlg.m_FilePath.GetLength()){
		strcpy(TargetMaps[i].path, dlg.m_FilePath);
		if(dlg.m_DXVersion > 1) dlg.m_DXVersion += 5;
		TargetMaps[i].dxversion = dlg.m_DXVersion;
		TargetMaps[i].flags = 0;
		if(dlg.m_UnNotify) TargetMaps[i].flags |= UNNOTIFY;
		if(dlg.m_EmulatePal) TargetMaps[i].flags |= EMULATEPAL;
		if(dlg.m_HookDI) TargetMaps[i].flags |= HOOKDI;
		if(dlg.m_ModifyMouse) TargetMaps[i].flags |= MODIFYMOUSE;
		if(dlg.m_OutTrace) TargetMaps[i].flags |= OUTTRACE;
		if(dlg.m_SaveLoad) TargetMaps[i].flags |= SAVELOAD;
		TargetMaps[i].initx = dlg.m_InitX;
		TargetMaps[i].inity = dlg.m_InitY;
		TargetMaps[i].minx = dlg.m_MinX;
		TargetMaps[i].miny = dlg.m_MinY;
		TargetMaps[i].maxx = dlg.m_MaxX;
		TargetMaps[i].maxy = dlg.m_MaxY;
		CListCtrl& listctrl = GetListCtrl();
		listitem.mask = LVIF_TEXT;
		listitem.iItem = i;
		listitem.iSubItem = 0;
		listitem.pszText = TargetMaps[i].path;
		listctrl.InsertItem(&listitem);
		Resize();
		SetTarget(TargetMaps);	
	}
}

void CDxwndhostView::OnDelete() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	int i;
	POSITION pos;
	CListCtrl& listctrl = GetListCtrl();

	if(!listctrl.GetSelectedCount()) return ;
	pos = listctrl.GetFirstSelectedItemPosition();
	i = listctrl.GetNextSelectedItem(pos);
	listctrl.DeleteItem(i);
	for(; i < MAXTARGETS  - 1; i ++) TargetMaps[i] = TargetMaps[i + 1];
	Resize();
	SetTarget(TargetMaps);
}

void CDxwndhostView::Resize()
{
	CListCtrl& listctrl = GetListCtrl();
	LV_COLUMN listcol;
	int i, tmp, size = 0;
	
	for(i = 0; i < MAXTARGETS; i ++){
		tmp = listctrl.GetStringWidth(TargetMaps[i].path);
		if(size < tmp) size = tmp;
	}
	
	listcol.mask = LVCF_WIDTH;
	listcol.cx = size + 10;
	listctrl.SetColumn(0, &listcol);
}



void CDxwndhostView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CMenu popup;
	int res;
	
	ClientToScreen(&point);
	popup.LoadMenu(IDR_MENU_POPUP);
	res = popup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD, point.x, point.y, this);
	switch(res){
	case ID_PRUN:
		OnRun();
		break;
	case ID_PMODIFY:
		OnModify();
		break;
	case ID_PDELETE:
		OnDelete();
		break;
	case ID_PADD:
		OnAdd();
		break;
	}
	CListView::OnRButtonDown(nFlags, point);
}

void CDxwndhostView::OnRun() 
{
	// TODO: この位置にコマンド ハンドラ用のコードを追加してください
	CListCtrl& listctrl = GetListCtrl();
	POSITION pos;
	int i;
	STARTUPINFO sinfo;
	PROCESS_INFORMATION pinfo;
	char path[MAX_PATH];

	if(!listctrl.GetSelectedCount()) return;
	pos = listctrl.GetFirstSelectedItemPosition();
	i = listctrl.GetNextSelectedItem(pos);
	ZeroMemory(&sinfo, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	strcpy(path, TargetMaps[i].path);
	PathRemoveFileSpec(path);
	CreateProcess(0, TargetMaps[i].path, 0, 0, 0, CREATE_DEFAULT_ERROR_MODE, 0, path, &sinfo, &pinfo);
}
