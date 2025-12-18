#include "pch.h"
#include "../Export/EmailSendUnitInterFace.h"
#include "EmailSendUnit.h"

#include <iostream>
#include <sstream>

bool ENDLL_IsLoadDone() { return false; }
bool ENDLL_Load(char* pszDllPath) { return false; }
bool ENDLL_Free() { return false; }

CEmailSendUnitInterFace* ENDLL_CreateEmailNotifyUnit(char* pszEmName, char* pszEmPass, char* pszServerHost, char* pszSendEmail, bool  bIsUserSSL,
	EN_CallBackNotify CallBackFunc, uint32_t nSendThreadCount)
{
	if (!pszEmName		|| strlen(pszEmName) > 128		|| strlen(pszEmName) == 0		||
		!pszEmPass		|| strlen(pszEmPass) > 128		|| strlen(pszEmPass) == 0		||
		!pszServerHost	|| strlen(pszServerHost) > 128	|| strlen(pszServerHost) == 0	||
		!pszSendEmail	|| strlen(pszSendEmail) > 128	|| strlen(pszSendEmail) == 0)
	{
		return nullptr;
	}

	CEmailSendUnit* pUnit = new CEmailSendUnit(pszEmName, pszEmPass, pszServerHost, pszSendEmail, bIsUserSSL, CallBackFunc, nSendThreadCount);
	return pUnit;
}

//释放一个邮件发送单元
void ENDLL_FreeEmailNotifyUnit(CEmailSendUnitInterFace* pUnit)
{
	if (!pUnit)
	{
		return;
	}
	CEmailSendUnit* pSUnit = (CEmailSendUnit*)pUnit;
	delete pSUnit;
}
