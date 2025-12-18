#include "pch.h"

#include "EmailSendUnitInterFace.h"

//  接口定义============================================================
typedef CEmailSendUnitInterFace*	(*B_ENDLL_CreateEmailNotifyUnit)(char*, char*, char*, char*, EN_CallBackNotify, uint32_t);
typedef void						(*B_ENDLL_FreeEmailNotifyUnit)(CEmailSendUnitInterFace*);

//  接口声明============================================================
B_ENDLL_CreateEmailNotifyUnit	T_ENDLL_CreateEmailNotifyUnit = NULL;
B_ENDLL_FreeEmailNotifyUnit		T_ENDLL_FreeEmailNotifyUnit = NULL;

HMODULE g_hDllCurl = NULL;
bool	g_bIsDllCurlDone = false;
bool ENDLL_IsLoadDone() 
{ 
	return g_bIsDllCurlDone;
}

bool ENDLL_Load(char* pszDllPath) 
{ 
	CString strDllPath;
	if (pszDllPath)
	{
		strDllPath = pszDllPath;
	}

	g_bIsDllCurlDone = false;

	CString strDllDir;
	CString strDllName;
	if (strDllPath.IsEmpty())
	{
		char szPath[MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szPath, MAX_PATH);
		strDllDir = szPath;
		strDllDir = strDllDir.Mid(0, strDllDir.ReverseFind('\\'));
		strDllName = "EmailNotify.dll";
	}
	else
	{
		int nFind = strDllPath.ReverseFind('\\');
		if (nFind <= 0)
		{
			AfxMessageBox("初始化ENDLL失败,DLL路径不合法");
			return false;
		}

		strDllDir = strDllPath.Mid(0, nFind);
		strDllName = strDllPath.Mid(nFind + 1, strDllPath.GetLength());
	}

	SetDllDirectory(strDllDir);
	g_hDllCurl = LoadLibrary(strDllName);

	if (g_hDllCurl == NULL)
	{
		CString strLog;
		strLog.Format("初始化ENDLL失败, 加载DLL失败. %u", ::GetLastError());
		AfxMessageBox(strLog.GetBuffer());
		SetDllDirectory(NULL);
		return false;
	}

	T_ENDLL_CreateEmailNotifyUnit	= (B_ENDLL_CreateEmailNotifyUnit)	GetProcAddress(g_hDllCurl, "ENDLL_CreateEmailNotifyUnit");
	T_ENDLL_FreeEmailNotifyUnit		= (B_ENDLL_FreeEmailNotifyUnit)		GetProcAddress(g_hDllCurl, "ENDLL_FreeEmailNotifyUnit");


	if (!T_ENDLL_CreateEmailNotifyUnit || !T_ENDLL_FreeEmailNotifyUnit)
	{
		AfxMessageBox("初始化ENDLL失败,DLL接口不全");
		return false;
	}

	SetDllDirectory(NULL);
	g_bIsDllCurlDone = true;
	return true;
}

bool ENDLL_Free() 
{ 
	if (g_hDllCurl == NULL)
	{
		return true;
	}
	return FreeLibrary(g_hDllCurl);
}

CEmailSendUnitInterFace* ENDLL_CreateEmailNotifyUnit(
	char* pszEmName,
	char* pszEmPass,
	char* pszServerHost,
	char* pszSendEmail,
	EN_CallBackNotify CallBackFunc,
	uint32_t nSendThreadCount)
{
	if (!g_bIsDllCurlDone)
	{
		return nullptr;
	}

	if (!pszEmName || strlen(pszEmName) > 128 || strlen(pszEmName) == 0 ||
		!pszEmPass || strlen(pszEmPass) > 128 || strlen(pszEmPass) == 0 ||
		!pszServerHost || strlen(pszServerHost) > 128 || strlen(pszServerHost) == 0 ||
		!pszSendEmail || strlen(pszSendEmail) > 128 || strlen(pszSendEmail) == 0)
	{
		return nullptr;
	}

	return T_ENDLL_CreateEmailNotifyUnit(pszEmName, pszEmPass, pszServerHost, pszSendEmail, CallBackFunc, nSendThreadCount);
}

void ENDLL_FreeEmailNotifyUnit(CEmailSendUnitInterFace* pUnit)
{
	if (!g_bIsDllCurlDone)
	{
		return;
	}

	if (!pUnit)
	{
		return;
	}
	T_ENDLL_FreeEmailNotifyUnit(pUnit);
}