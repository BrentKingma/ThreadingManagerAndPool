#pragma once
#include <condition_variable>
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <future>

namespace MyEngine
{
	class ThreadPool
	{
	public:
		using Task = std::function<void()>;

		static void init();
		static void init(int num);
		static void kill();
		template<class T>
		static auto enqueue(T a_task) -> std::future<decltype(a_task())>
		{
			auto wrapper = std::make_shared<std::packaged_task<decltype(a_task()) ()>>(std::move(a_task));
			{
				std::unique_lock<std::mutex> lock{ m_eventMutex };
				m_tasks.emplace([=] {
					(*wrapper)();
				});
			}

			m_eventVar.notify_one();
			return wrapper->get_future();
		}

		static int getNumThreads();
	private:
		static std::vector<std::shared_ptr<std::thread>> m_threadPool;
		static std::queue<Task> m_tasks;
		static std::condition_variable m_eventVar;
		static std::mutex m_eventMutex;
		static int m_numThreads;
		static bool m_stop;
	};
}

