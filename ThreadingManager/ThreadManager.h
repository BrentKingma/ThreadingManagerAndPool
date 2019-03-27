#pragma once
#include <unordered_map>
#include <thread>
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	void AddThread(std::string a_threadName, std::thread* a_thread);
	
private:
	std::unordered_map <std::string, std::thread*> m_managingThreads;
};

