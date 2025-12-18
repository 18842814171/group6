#pragma once
#include "../Export/EmailSendUnitInterFace.h"
#include <curl/curl.h>
#include <mutex>
#include <thread>
#include<queue>
using namespace std;

class CEmsQueue
{
public:
	CEmsQueue() = default;
	~CEmsQueue() { clear(); }

	//添加
	void push(EMSend *pItem)
	{
		unique_lock<mutex>lck(m_mtx);
		m_queue.push(pItem);
	}

	//队列长度
	size_t size()
	{
		unique_lock<mutex>lck(m_mtx);
		return m_queue.size();
	}

	// 轮询消息
	bool poll(EMSend* &pItem)
	{
		unique_lock<mutex>lck(m_mtx);
		if (m_queue.size())
		{
			pItem = m_queue.front();
			m_queue.pop();
			return true;
		}
		return false;
	}

	// 清理.
	void clear()
	{
		EMSend* pItem = nullptr;
		while (poll(pItem))
		{
			if (pItem)
			{
				delete pItem;
				pItem = nullptr;
			}
		}
	}
private:
	queue<EMSend*> m_queue;
	mutex m_mtx;
};

class CEmailSendUnit : public CEmailSendUnitInterFace
{
public:
	CEmailSendUnit(string strName, string strPass, string strServer, string strSendEmail, bool bIsUserSSL,
		EN_CallBackNotify CallBackFunc, uint32_t nSendThreadCount);
	~CEmailSendUnit();

	//重写外部调用的发送接口
	void SendEmail(const EMSend& ems);

//线程相关
private:

	//邮件发送线程
	void SendEmailTh();

	bool		m_bStop;		//线程退出标识
	thread**	m_thSend;		//线程组
	CEmsQueue	m_EmsQueue;		//发送邮件线程交互队列

//curl相关
private:

	//创建并设置CURL对象
	CURL* CreateCurlAndOption();

	//curl发送回调
	static  size_t payload_source(void* ptr, size_t size, size_t nmemb, void* stream);

	//错误回调
	void ErrorCallBack(uint32_t nError, string strError, uint64_t dwCustomParam);

//服务器及发送者
private:
	string				m_strName;
	string				m_strPass;
	string				m_strServer;
	string				m_strSendEmail;

	bool				m_bIsUserSSL = false;

	EN_CallBackNotify	m_CallBackFunc = nullptr;

	uint32_t			m_nThreadCount = 1;
};

