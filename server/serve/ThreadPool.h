#pragma once
#include <future>
#include <memory>
#include <mutex>
#include <atomic>
#include <vector>
#include <condition_variable>
#include <queue>
#include <iostream>
#include <functional>
/*
此类为线程池类
模板类需要直接写在.h文件
*/





/* 安全队列 */
template<typename T>
class SafeQueue {     // 安全队列:即添加了互斥量的队列，其他方法与队列类似
public:
	SafeQueue() = default;
	~SafeQueue() = default;
	bool empty()
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		return m_Queue.empty();
	}

	int size()
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		return m_Queue.size();
	}

	void push(T& value)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_Queue.emplace(value);
	}

	void push(T&& value)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_Queue.emplace(std::move(value));
	}

	bool pop(T& value)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		if (m_Queue.empty()) {
			return false;
		}
		else {
			value = std::move(m_Queue.front());
			m_Queue.pop();
			return true;
		}
	}
private:
	std::queue<T> m_Queue;
	std::mutex m_mutex;
};




/* 线程池 */
class ThreadPool {
public:
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(const ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&&) = delete;
	ThreadPool() :m_Threads(3), m_RunningStatus(true) { initalize(); } // std::thread::hardware_concurrency()
	ThreadPool(int threadNum) :m_Threads(threadNum), m_RunningStatus(true) { initalize(); }
	/* 用于提交要进行的任务 */
	template<typename Func, typename ... Args>
	void submitTask(Func&& func, Args... args)
	{
		std::function<void()> taskWrapper1 = std::bind(std::forward<Func>(func), std::forward<Args>(args)...); // 打包成不带参数的task
		auto taskWrapper2 = std::make_shared<std::packaged_task<void()>>(taskWrapper1); // 处理返回值
		TaskType wrapperFunction = [taskWrapper2]() {
			(*taskWrapper2)();
		};
		m_TaskQueue.push(wrapperFunction);
		m_cv.notify_one();		// 随机唤醒一个线程
	}

	~ThreadPool();

private:
	using TaskType = std::function<void()>;
	SafeQueue<TaskType> m_TaskQueue;	// 安全队列用于保存要进行的任务
	std::vector<std::thread> m_Threads;		// 线程向量
	std::condition_variable m_cv;
	std::mutex m_mutex;			// 互斥锁
	std::atomic<bool> m_RunningStatus;	// 运行状态
	void initalize();		// 初始化线程池函数
};
