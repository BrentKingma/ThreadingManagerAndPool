#include "pch.h"
#include "ThreadPool.h"

std::vector<std::shared_ptr<std::thread>> MyEngine::ThreadPool::m_threadPool;
std::queue<MyEngine::ThreadPool::Task> MyEngine::ThreadPool::m_tasks;
std::condition_variable MyEngine::ThreadPool::m_eventVar;
std::mutex MyEngine::ThreadPool::m_eventMutex;
int MyEngine::ThreadPool::m_numThreads;
bool MyEngine::ThreadPool::m_stop;

void MyEngine::ThreadPool::init()
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
void MyEngine::ThreadPool::init(int num)
{
	for (int i = 0; i < num; i++)
	{
		m_threadPool.push_back(std::make_shared<std::thread>(std::thread()));
	}
	m_numThreads = num;
}
void MyEngine::ThreadPool::kill()
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

int MyEngine::ThreadPool::getNumThreads()
{
	return m_numThreads;
}

