#include "pch.h"
#include "ThreadManager.h"


ThreadManager::ThreadManager()
{
}


ThreadManager::~ThreadManager()
{
}

void ThreadManager::AddThread(std::string a_threadName, std::thread* a_thread)
{
	m_managingThreads[a_threadName] = a_thread;
}
