#include "ThreadPool.h"





ThreadPool::~ThreadPool()
{
	m_RunningStatus = false;
	m_cv.notify_all();
	for (auto& thread : m_Threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}

void ThreadPool::initalize()
{
	for (int i = 0; i < m_Threads.size(); i++) {
		auto worker = [this, i]() {
			while (m_RunningStatus) {
				TaskType task;
				bool isSuccess = false;
				{
					std::unique_lock<std::mutex> locker(m_mutex);
					if (this->m_TaskQueue.empty()) {
						this->m_cv.wait(locker);
					}
					isSuccess = this->m_TaskQueue.pop(task);
				}
				if (isSuccess) {
					//std::cout << i << std::endl;
					task();
				}
			}
		};
		m_Threads[i] = std::thread(worker);
	}
}
