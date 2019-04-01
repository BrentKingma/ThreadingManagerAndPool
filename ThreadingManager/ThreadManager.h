#pragma once
#include <unordered_map>
#include <thread>
#include <string>

namespace MyThreading
{
	class ThreadManager
	{
	public:
		ThreadManager() {};
		~ThreadManager() {};

		inline int GetNumThreads() const { return m_numThreads; }

		void AddThread(std::string a_threadName, std::thread a_thread);
		void KillThread(std::string a_threadName);
		void ReturnThread(std::string a_threadName, std::thread& a_myTheard);

	private:
		std::unordered_map <std::string, std::thread> m_managingThreads;
		int m_numThreads;
	};
}

