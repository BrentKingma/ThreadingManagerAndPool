#include "pch.h"
#include "ThreadPool.h"

namespace MyThreading
{
	std::vector<std::shared_ptr<std::thread>> ThreadPool::m_threadPool;
	std::queue<ThreadPool::Task> ThreadPool::m_tasks;
	std::condition_variable ThreadPool::m_eventVar;
	std::mutex ThreadPool::m_eventMutex;
	int ThreadPool::m_numThreads;
	bool ThreadPool::m_stop;

	void ThreadPool::init()
	{
		for (int i = 0; i < 10; i++)
		{
			m_threadPool.push_back(std::make_shared<std::thread>([=] {
				while (true)
				{
					Task task;
					{
						std::unique_lock<std::mutex> lock{ m_eventMutex };
						//Blocking call, passes when conditional variable is notified
						m_eventVar.wait(lock, [=] { return m_stop || !m_tasks.empty(); });

						if (m_stop && m_tasks.empty()) break;

						task = std::move(m_tasks.front());
						m_tasks.pop();
					}

					task();
				}
			}));
		}
		m_numThreads = 10;
	}
	void ThreadPool::init(int num)
	{
		for (int i = 0; i < num; i++)
		{
			m_threadPool.push_back(std::make_shared<std::thread>([=] {
				while (true)
				{
					Task task;
					{
						std::unique_lock<std::mutex> lock{ m_eventMutex };
						//Blocking call, passes when conditional variable is notified
						m_eventVar.wait(lock, [=] { return m_stop || !m_tasks.empty(); });

						if (m_stop && m_tasks.empty()) break;

						task = std::move(m_tasks.front());
						m_tasks.pop();
					}

					task();
				}
			}));
		}
		m_numThreads = num;
	}
	void ThreadPool::kill()
	{
		{
			std::unique_lock<std::mutex> lock{ m_eventMutex };
			m_stop = true;
		}

		m_eventVar.notify_all();
		for (int i = 0; i < m_numThreads; i++)
		{
			m_threadPool[i]->join();
		}
	}

	int ThreadPool::getNumThreads()
	{
		return m_numThreads;
	}
}

