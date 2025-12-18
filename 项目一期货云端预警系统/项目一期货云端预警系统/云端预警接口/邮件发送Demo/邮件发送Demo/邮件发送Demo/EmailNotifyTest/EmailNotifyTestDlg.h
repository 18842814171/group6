
// EmailNotifyTestDlg.h: 头文件
//

#pragma once

#include "DllLoad/EmailSendUnitInterFace.h"

struct SDCfg
{
	CString strName;
	CString strPsw;
	CString strSmtpServer;
	CString strEmailAddr;
	int nSendThreadCount = 1;

	SDCfg& operator=(const SDCfg& Other)
	{
		strName = Other.strName;
		strPsw = Other.strPsw;
		strSmtpServer = Other.strSmtpServer;
		strEmailAddr = Other.strEmailAddr;
		nSendThreadCount = Other.nSendThreadCount;
		return *this;
	}
};

// CEmailNotifyTestDlg 对话框
class CEmailNotifyTestDlg : public CDialogEx
{
// 构造
public:
	CEmailNotifyTestDlg(CWnd* pParent = nullptr);	// 标准构造函数

private:

	//邮件发送结果回调通知
	void SendEmailCallBack(uint32_t nNotifyType, const char* pszInfo, size_t nInfoLen, uint64_t dwParam);

	//邮件发送单元
	CEmailSendUnitInterFace* m_pEmSender = nullptr;

	//SendEmailCallBack回调的c++11 bind
	EN_CallBackNotify m_funcCallBack = nullptr;

private:
	CEdit m_editResult;
	CEdit m_editSubject;
	CEdit m_editRecvs;
	CEdit m_editSDThreadCount;
	CEdit m_editSDEmlAddr;
	CEdit m_editSDServer;
	CEdit m_editSDPsw;
	CEdit m_editSDUser;

	CString m_strSendResult;

	bool SetOrGetCtrl(bool bIsGet,SDCfg& sdcfg, EMSend& ems, bool bIsCheck);

	//读写测试配置
	void ReadConfig(SDCfg& sdcfg, EMSend &ems);
	void WriteConfig(SDCfg& sdcfg, EMSend& ems);

	//配置文件路径
	CString GetConfigPath();

public:
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EMAILNOTIFYTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnStart();
	CEdit m_editInfo;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
