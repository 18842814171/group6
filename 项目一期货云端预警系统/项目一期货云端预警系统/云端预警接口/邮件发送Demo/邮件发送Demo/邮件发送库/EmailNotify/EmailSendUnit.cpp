#include "pch.h"
#include "EmailSendUnit.h"

#include <iostream>  
#include <sstream>  
#include <fstream> 

class CManager
{
public:
	CManager() { curl_global_init(CURL_GLOBAL_ALL); }
	~CManager() { curl_global_cleanup(); }
};
CManager g_Mgr;

// string传统格式化
template<typename ... Args>
static string Strformat(const std::string& format, Args ... args)
{
	auto size_buf = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buf(new(std::nothrow) char[size_buf]);

	if (!buf)
		return string("");

	std::snprintf(buf.get(), size_buf, format.c_str(), args ...);
	return string(buf.get(), buf.get() + size_buf - 1);
}

//根据拆分标记对源串进行拆分
deque<string> StrSplit(string strSource, const string strFlag)
{
	deque<string> dqResult;

	//参数检查
	if (strSource == strFlag)
	{
		dqResult.push_back(strSource);
		return dqResult;
	}
	else if (strSource.length() <= strFlag.length())
	{
		return dqResult;
	}

	//首期处理
	if (strSource.substr(strSource.length() - strFlag.length()) != strFlag)
	{
		strSource += strFlag;
	}

	string strTmp;
	size_t Pos = strSource.find(strFlag);

	while (Pos != -1)
	{
		dqResult.push_back(strSource.substr(0, Pos));
		strSource.erase(0, Pos + strFlag.length());
		Pos = strSource.find(strFlag);
	}

	return dqResult;
}

CEmailSendUnit::CEmailSendUnit(string strName, string strPass, string strServer, string strSendEmail, bool bIsUserSSL,
	EN_CallBackNotify CallBackFunc, uint32_t nSendThreadCount)
{
	m_strName = strName;
	m_strPass = strPass;
	m_strSendEmail = strSendEmail;

	m_bIsUserSSL = bIsUserSSL;

	transform(strServer.begin(), strServer.end(), strServer.begin(), ::tolower);

	if (bIsUserSSL)
	{
		size_t nPos = strServer.find("smtps://");
		if (nPos == string::npos)
		{
			m_strServer = "smtps://" + strServer;
		}
		else
		{
			m_strServer = strServer;
		}
	}
	else
	{
		size_t nPos = strServer.find("smtp://");
		if (nPos == string::npos)
		{
			m_strServer = "smtp://" + strServer;
		}
		else
		{
			m_strServer = strServer;
		}
	}
	
	m_CallBackFunc = CallBackFunc;

	if (nSendThreadCount <= 0)
	{
		nSendThreadCount = 1;
	}
	if (nSendThreadCount > 10)
	{
		nSendThreadCount = 10;
	}
	m_nThreadCount = nSendThreadCount;

	m_bStop = false;
	m_thSend = new thread * [m_nThreadCount];
	for (uint32_t i = 0; i < m_nThreadCount; i++)
	{
		m_thSend[i] = new thread(&CEmailSendUnit::SendEmailTh, this);
	}
}

CEmailSendUnit::~CEmailSendUnit()
{
	m_bStop = true;
	for (uint32_t i = 0; i < m_nThreadCount; i++)
	{
		m_thSend[i]->join();
		delete m_thSend[i];
	}
	delete[] m_thSend;
	m_EmsQueue.clear();

}

void CEmailSendUnit::SendEmail(const EMSend& ems)
{
	EMSend* pItem = new EMSend(ems);
	m_EmsQueue.push(pItem);
}

void CEmailSendUnit::SendEmailTh()
{
	//在当前线上上创建CURL对象,进行邮件发送
	CURL* pCurl = CreateCurlAndOption();
	if (!pCurl)
	{
		return;
	}

	//线程轮询
	while (!m_bStop)
	{
		//发送缓冲队列中无要发送的邮件时, 休眠100毫秒
		if (m_EmsQueue.size() == 0)
		{
			this_thread::sleep_for(chrono::milliseconds(100));
			continue;
		}

		//从发送缓冲中获取一份要发送的邮件
		EMSend* pItem = nullptr;
		m_EmsQueue.poll(pItem);
		if (!pItem)
		{
			continue;
		}

		//设置收件人
		deque<string> strRecvs = StrSplit(pItem->dRecvEmails.m_pszData, ",");
		struct curl_slist* recipients = NULL;
		for (size_t i = 0; i < strRecvs.size(); i++) {

			recipients = curl_slist_append(recipients, strRecvs[i].c_str());
		}
		curl_easy_setopt(pCurl, CURLOPT_MAIL_RCPT, recipients);

// 		//设置MIME串(格式化好的发件收件及主题正文串)
// 		string strMime = Strformat(m_strFormatMsg, pItem->dRecvEmails.m_pszData, pItem->dSubject.m_pszData, pItem->dInfo.m_pszData);
// 		std::stringstream stream;
// 		stream.str(strMime.c_str());
// 		stream.flush();
// 		curl_easy_setopt(pCurl, CURLOPT_READDATA, (void*)&stream);
// 		curl_easy_setopt(pCurl, CURLOPT_UPLOAD, 1L);
// 
		
		//组织发送头

		string toHeader = "To: ";
		toHeader += "测试用户";
		string fromHeader = "From: ";
		fromHeader += m_strSendEmail;
		string subjectHeader = "Subject: ";
		subjectHeader += pItem->dSubject.m_pszData;

		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, fromHeader.c_str());
		headers = curl_slist_append(headers, toHeader.c_str());
		headers = curl_slist_append(headers, subjectHeader.c_str());
		curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);

		curl_mime* mime = curl_mime_init(pCurl);
		curl_mimepart* part = curl_mime_addpart(mime);

		string text = pItem->dInfo.m_pszData;
		text += "\r\n";
		curl_mime_data(part, text.c_str(), CURL_ZERO_TERMINATED);
		curl_mime_type(part, "text/plain");
		curl_mime_encoder(part, "quoted-printable");
		curl_easy_setopt(pCurl, CURLOPT_MIMEPOST, mime);

		//发送
		CURLcode res = curl_easy_perform(pCurl);

		//释放收件人列表
		curl_slist_free_all(recipients);
		curl_slist_free_all(headers);
		curl_mime_free(mime);

		//通过回调通知发送结果
		ErrorCallBack(res, curl_easy_strerror(res), pItem->dwCallBackFuncParam);

		delete pItem;
	}

	//线程退出,回收该线程上的CURL对象
	curl_easy_cleanup(pCurl);
}

CURL* CEmailSendUnit::CreateCurlAndOption()
{
	CURL * pCurl = curl_easy_init();
	if (!pCurl)
	{
		return nullptr;
	}

	//发送的账号,密码,服务器,发送邮箱
	curl_easy_setopt(pCurl, CURLOPT_USERNAME, m_strName.c_str());
	curl_easy_setopt(pCurl, CURLOPT_PASSWORD, m_strPass.c_str());
	curl_easy_setopt(pCurl, CURLOPT_URL, m_strServer.c_str());
	curl_easy_setopt(pCurl, CURLOPT_MAIL_FROM, m_strSendEmail.c_str());

	curl_easy_setopt(pCurl, CURLOPT_VERBOSE, 0L);			//0关闭调试输出
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);	//不检查证书颁发机构
	curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);	//检查证书里的域名. 0,不检查; 1,只检查是否有域名; 2,检查域名与主机名(IP)是否匹配

	curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, &CEmailSendUnit::payload_source); //设置邮件发送过程中回调

	return pCurl;
}

size_t CEmailSendUnit::payload_source(void* ptr, size_t size, size_t nmemb, void* stream)
{
	size_t num_bytes = size * nmemb;
	char* data = (char*)ptr;
	std::stringstream* strstream = (std::stringstream*)stream;

	strstream->read(data, num_bytes);

	return strstream->gcount();
}

void CEmailSendUnit::ErrorCallBack(uint32_t nError, string strError, uint64_t dwCustomParam)
{
	if (!m_CallBackFunc)
	{
		return;
	}

	m_CallBackFunc(nError, strError.c_str(), strError.size(), dwCustomParam);
}