#include "pch.h"
#include "ThreadManager.h"

namespace MyThreading
{
	void ThreadManager::AddThread(std::string a_threadName, std::thread a_thread)
	{
		m_managingThreads[a_threadName] = std::move(a_thread);
		m_numThreads++;
	}

	void ThreadManager::KillThread(std::string a_threadName)
	{
		m_managingThreads[a_threadName].join();
		m_managingThreads.erase(a_threadName);
		m_numThreads--;
	}

	void ThreadManager::ReturnThread(std::string a_threadName, std::thread & a_myTheard)
	{
		a_myTheard = std::move(m_managingThreads[a_threadName]);
		m_managingThreads.erase(a_threadName);
		m_numThreads--;
	}
}
