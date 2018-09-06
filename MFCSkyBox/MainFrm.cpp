
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MFCSkyBox.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
	this->m_bFullScreenMode = false;//设置全屏显示为假
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO:  如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	
	//自己添加的，为了全屏显示
	this->m_menuMainWnd.LoadMenu(IDR_MAINFRAME);
	this->SetMenu(&m_menuMainWnd);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::FullScreenModeOn()
{
	m_bToolBarWasVisible = (m_wndToolBar.IsWindowVisible() != 0);//隐藏工具栏之前工具栏的显示状态    
	m_wndToolBar.ShowWindow(SW_HIDE);//隐藏工具栏 


	m_bStatusBarWasVisible = (m_wndStatusBar.IsWindowVisible() != 0);//隐藏状态栏之前状态栏的显示状态 
	m_wndStatusBar.ShowWindow(SW_HIDE);//隐藏状态栏 


	//隐藏菜单栏 
	SetMenu(NULL);


	// 保存以前的位置信息 
	GetWindowRect(&m_mainRect);


	// 去除主窗口的标题 
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	style &= ~WS_CAPTION;

	::SetWindowLong(m_hWnd, GWL_STYLE, style);



	//得到当前系统的分辨率 
	int screenx = GetSystemMetrics(SM_CXSCREEN);

	int screeny = GetSystemMetrics(SM_CYSCREEN);



	// 全屏显示 
	SetWindowPos(NULL, -4, -4, screenx + 8, screeny + 8, SWP_NOZORDER);

	style = ::GetWindowLong(this->m_hWnd, GWL_STYLE);


	//设置全屏显示标志 
	m_bFullScreenMode = true;
}


void CMainFrame::FullScreenModeOff()
{
	//恢复窗口标题 
	LONG style = ::GetWindowLong(m_hWnd, GWL_STYLE);

	style |= WS_CAPTION;

	::SetWindowLong(m_hWnd, GWL_STYLE, style);

	//如果需要，显示工具栏 
	if (m_bToolBarWasVisible)

		m_wndToolBar.ShowWindow(SW_SHOW);

	//如果需要，显示状态栏 
	if (m_bStatusBarWasVisible)

		m_wndStatusBar.ShowWindow(SW_SHOW);

	//恢复窗口以前的大小 
	MoveWindow(&m_mainRect);


	//恢复菜单栏 
	this->SetMenu(&m_menuMainWnd);


	//设置全屏显示标志 
	m_bFullScreenMode = false;
}


void CMainFrame::FullScreenModeSwitch()
{
	if (this->m_bFullScreenMode)
	{
		this->FullScreenModeOff();
	}
	else
	{
		this->FullScreenModeOn();
	}
}