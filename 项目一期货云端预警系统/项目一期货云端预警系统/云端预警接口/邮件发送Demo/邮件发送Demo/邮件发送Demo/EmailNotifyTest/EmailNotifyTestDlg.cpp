
// EmailNotifyTestDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EmailNotifyTest.h"
#include "EmailNotifyTestDlg.h"
#include "afxdialogex.h"

#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEmailNotifyTestDlg 对话框

#define WM_SendEmailResult	WM_USER + 1

CEmailNotifyTestDlg::CEmailNotifyTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EMAILNOTIFYTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEmailNotifyTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Result, m_editResult);
	DDX_Control(pDX, IDC_EDIT_EMINFO, m_editInfo);
	DDX_Control(pDX, IDC_EDIT_RecvEml, m_editRecvs);
	DDX_Control(pDX, IDC_EDIT_SDTHCOUNT, m_editSDThreadCount);
	DDX_Control(pDX, IDC_EDIT_SDEmailAddr, m_editSDEmlAddr);
	DDX_Control(pDX, IDC_EDIT_SDServer, m_editSDServer);
	DDX_Control(pDX, IDC_EDIT_SDPASS, m_editSDPsw);
	DDX_Control(pDX, IDC_EDIT_SDUSER, m_editSDUser);
	DDX_Control(pDX, IDC_EDIT_Subject, m_editSubject);
}

BEGIN_MESSAGE_MAP(CEmailNotifyTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_Start, &CEmailNotifyTestDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CEmailNotifyTestDlg 消息处理程序

BOOL CEmailNotifyTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_funcCallBack = std::bind(&CEmailNotifyTestDlg::SendEmailCallBack, this,
		std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	SDCfg sdcfg;
	EMSend ems;
	ReadConfig(sdcfg, ems);
	SetOrGetCtrl(false, sdcfg, ems, false);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEmailNotifyTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEmailNotifyTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEmailNotifyTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEmailNotifyTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pEmSender)
	{
		ENDLL_FreeEmailNotifyUnit(m_pEmSender);
	}

	m_pEmSender = nullptr;
	m_funcCallBack = nullptr;
}

void CEmailNotifyTestDlg::SendEmailCallBack(uint32_t nNotifyType, const char* pszInfo, size_t nInfoLen, uint64_t dwParam)
{
	DWORD dwThID = GetCurrentThreadId();
	CString *pstrMsg = new CString;
	pstrMsg->Format("邮件标识: %llu, 处理线程: %u, 发送结果:[%d]%s\r\n", dwParam, dwThID, nNotifyType, pszInfo);

	//此回调函数调用于邮件库内部的发送线程, 更新UI应在主线程进行.
	
	//将结果字符串,发送到主线程处理
	PostMessage(WM_SendEmailResult, (WPARAM)pstrMsg, 0);
}

bool CEmailNotifyTestDlg::SetOrGetCtrl(bool bIsGet, SDCfg& sdcfg, EMSend& ems, bool bIsCheck)
{
	if (bIsGet)
	{
		SDCfg sdTmp;

		CString strTmp, strTmp1, strTmp2;

		m_editSDUser.GetWindowText(sdTmp.strName);
		m_editSDPsw.GetWindowText(sdTmp.strPsw);
		m_editSDServer.GetWindowText(sdTmp.strSmtpServer);
		m_editSDEmlAddr.GetWindowText(sdTmp.strEmailAddr);
		
		m_editSDThreadCount.GetWindowText(strTmp);
		sdTmp.nSendThreadCount = atoi(strTmp.GetString());

		if (bIsCheck && 
			(sdTmp.strName.IsEmpty() || sdTmp.strPsw.IsEmpty() || 
		     sdTmp.strSmtpServer.IsEmpty() || sdTmp.strEmailAddr.IsEmpty() || strTmp.IsEmpty()))
		{
			AfxMessageBox("请将测试参数填写全", MB_OK);
			return false;
		}

		EMSend emsTmp;
		m_editRecvs.GetWindowText(strTmp);
		emsTmp.dRecvEmails = strTmp.GetBuffer();

		m_editSubject.GetWindowText(strTmp1);
		emsTmp.dSubject = strTmp1.GetBuffer();

		m_editInfo.GetWindowText(strTmp2);
		emsTmp.dInfo = strTmp2.GetBuffer();

		if (bIsCheck &&
			(strTmp.IsEmpty() || strTmp1.IsEmpty() || strTmp2.IsEmpty()))
		{
			AfxMessageBox("请将测试参数填写全", MB_OK);
			return false;
		}

		sdcfg = sdTmp;
		ems = emsTmp;
	}
	else
	{
		m_editSDUser.SetWindowText(sdcfg.strName);
		m_editSDPsw.SetWindowText(sdcfg.strPsw);
		m_editSDServer.SetWindowText(sdcfg.strSmtpServer);
		m_editSDEmlAddr.SetWindowText(sdcfg.strEmailAddr);

		CString strTmp;
		strTmp.Format("%d", sdcfg.nSendThreadCount);
		m_editSDThreadCount.SetWindowText(strTmp);

		m_editRecvs.SetWindowText(ems.dRecvEmails.m_pszData);
		m_editSubject.SetWindowText(ems.dSubject.m_pszData);
		m_editInfo.SetWindowText(ems.dInfo.m_pszData);
	}

	return true;
}

void CEmailNotifyTestDlg::ReadConfig(SDCfg& sdcfg, EMSend& ems)
{
	CString strIniPath = GetConfigPath();

	char szTmp[512];
	memset(szTmp, 0, 512);
	::GetPrivateProfileString("SDCFG", "USER", "", szTmp, 512, strIniPath);
	sdcfg.strName = szTmp;

	memset(szTmp, 0, 512);
	::GetPrivateProfileString("SDCFG", "PSW", "", szTmp, 512, strIniPath);
	sdcfg.strPsw = szTmp;

	memset(szTmp, 0, 512);
	::GetPrivateProfileString("SDCFG", "SMTP", "", szTmp, 512, strIniPath);
	sdcfg.strSmtpServer = szTmp;

	memset(szTmp, 0, 512);
	::GetPrivateProfileString("SDCFG", "EMAIL", "", szTmp, 512, strIniPath);
	sdcfg.strEmailAddr = szTmp;

	sdcfg.nSendThreadCount = ::GetPrivateProfileInt("SDCFG", "THCOUNT", 1, strIniPath);

	memset(szTmp, 0, 512);
	::GetPrivateProfileString("EMCFG", "RECVUSER", "", szTmp, 512, strIniPath);
	ems.dRecvEmails = szTmp;

	memset(szTmp, 0, 512);
	::GetPrivateProfileString("EMCFG", "SUBJECT", "", szTmp, 512, strIniPath);
	ems.dSubject = szTmp;

	memset(szTmp, 0, 512);
	::GetPrivateProfileString("EMCFG", "INFO", "", szTmp, 512, strIniPath);
	ems.dInfo = szTmp;
}
void CEmailNotifyTestDlg::WriteConfig(SDCfg& sdcfg, EMSend& ems)
{
	CString strIniPath = GetConfigPath();

	::WritePrivateProfileString("SDCFG", "USER", sdcfg.strName, strIniPath);
	::WritePrivateProfileString("SDCFG", "PSW", sdcfg.strPsw, strIniPath);
	::WritePrivateProfileString("SDCFG", "SMTP", sdcfg.strSmtpServer, strIniPath);
	::WritePrivateProfileString("SDCFG", "EMAIL", sdcfg.strEmailAddr, strIniPath);

	CString strTmp;
	strTmp.Format("%d", sdcfg.nSendThreadCount);
	::WritePrivateProfileString("SDCFG", "THCOUNT", strTmp, strIniPath);

	::WritePrivateProfileString("EMCFG", "RECVUSER", ems.dRecvEmails.m_pszData, strIniPath);
	::WritePrivateProfileString("EMCFG", "SUBJECT", ems.dSubject.m_pszData, strIniPath);
	::WritePrivateProfileString("EMCFG", "INFO", ems.dInfo.m_pszData, strIniPath);
}

CString CEmailNotifyTestDlg::GetConfigPath()
{
	char szPath[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strIniPath = szPath;
	strIniPath = strIniPath.Mid(0, strIniPath.ReverseFind('\\'));
	strIniPath += "\\TestConfig.ini";
	return strIniPath;
}

void CEmailNotifyTestDlg::OnBnClickedBtnStart()
{
	SDCfg sdfg;
	EMSend ems;
	if (!SetOrGetCtrl(TRUE,sdfg,ems,true))
	{
		return;
	}

	//记录测试选项
	WriteConfig(sdfg, ems);

	//如果之前有未发送完的邮件, 析构时停止发送
	if (m_pEmSender)
	{
		ENDLL_FreeEmailNotifyUnit(m_pEmSender);
		m_pEmSender = nullptr;
	}

	m_strSendResult = "";
	m_editResult.SetWindowText(m_strSendResult);

	
	m_pEmSender = ENDLL_CreateEmailNotifyUnit(
		sdfg.strName.GetBuffer(),			//发送者邮箱账号
		sdfg.strPsw.GetBuffer(),			//发送者邮箱密码
		sdfg.strSmtpServer.GetBuffer(),		//发送者邮箱服务器
		sdfg.strEmailAddr.GetBuffer(),		//发送者邮箱地址
		m_funcCallBack,						//邮件发送结果回调
		sdfg.nSendThreadCount);				//处理发送邮件线程数; 1~10

	if (m_pEmSender)
	{
		//测试, 一次发10封邮件
		for (int i = 0; i < 2/*10*/; i++)
		{
			ems.dwCallBackFuncParam = i;		//邮件的自定义参数, 可以任意写数字,有效指针地址等, 由callfun回调带回

			//发送邮件, 异步非阻塞
			m_pEmSender->SendEmail(ems);
		}
	}
}


LRESULT CEmailNotifyTestDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SendEmailResult)
	{
		CString* pstrResult = (CString*)wParam;
		if (pstrResult)
		{
			m_strSendResult += *pstrResult;
			m_editResult.SetWindowText(m_strSendResult);
			m_editResult.LineScroll(m_editResult.GetLineCount());
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
