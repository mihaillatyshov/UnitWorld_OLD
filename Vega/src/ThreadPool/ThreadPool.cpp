#include "ThreadPool.h"

//template<class T>
//auto ThreadPool::Enqueue(T task)->std::future<decltype(task())>
//{
//	auto wrapper = std::make_shared<std::packaged_task<decltype(task())()>>(std::move(task));
//
//	{
//		std::unique_lock lock(m_EventMutex);
//		m_Tasks.emplace([=]
//		{
//				(*wrapper)();
//		});
//	}
//
//	m_EventVar.notify_one();
//	return wrapper->get_future();
//}


void ThreadPool::Start(size_t numThreads)
{
	for (int i = 0; i < numThreads; i++)
	{
		m_Threads.emplace_back([=] {
			while (true)
			{
				Task task;

				{
					std::unique_lock lock(m_EventMutex);

					m_EventVar.wait(lock, [=] { return m_Stopping || !m_Tasks.empty(); });

					if (m_Stopping && m_Tasks.empty())
						break;

					task = std::move(m_Tasks.front());
					m_Tasks.pop();
					//m_UsingThreads += 1;
				}

				task();
				//m_UsingThreads -= 1;
			}
		});
	}
}


void ThreadPool::Stop() noexcept
{
	{
		std::unique_lock lock(m_EventMutex);
		m_Stopping = true;
	}

	m_EventVar.notify_all();

	for (auto& thread : m_Threads)
	{
		thread.join();
	}
}
