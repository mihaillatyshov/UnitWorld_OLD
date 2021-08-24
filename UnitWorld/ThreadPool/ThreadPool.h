#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>


class ThreadPool
{
public:
	using Task = std::function<void()>;

	explicit ThreadPool(size_t numThreads)
	{
		Start(numThreads);
		m_MainId = std::this_thread::get_id();
	}

	~ThreadPool()
	{
		Stop();
	}

	template<class T>
	auto Enqueue(T task)->std::future<decltype(task())>
	{
		auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
	
		{
			std::unique_lock lock(m_EventMutex);
			if ((std::this_thread::get_id() != m_MainId) && (m_Tasks.size() + m_UsingThreads >= m_Threads.size()))
			{
				(*wrapper)();
				return wrapper->get_future();
			}

			m_Tasks.emplace([=]
			{
					(*wrapper)();
			});
		}
	
		m_EventVar.notify_one();
		return wrapper->get_future();
	}
	
	//void Enqueue(Task task)
	//{
	//	{
	//		std::unique_lock lock(m_EventMutex);
	//		m_Tasks.emplace(std::move(task));
	//	}
	//
	//	m_EventVar.notify_one();
	//}

private:

	void Start(size_t numThreads);

	void Stop() noexcept;

private:
	std::vector<std::thread> m_Threads;
	std::condition_variable m_EventVar;
	std::mutex m_EventMutex;
	bool m_Stopping = false;

	std::queue<Task> m_Tasks;

	std::thread::id m_MainId;

	std::atomic<int> m_UsingThreads = 0;

};