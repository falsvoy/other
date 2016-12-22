//
// Created by Надежда on 22.12.16.
//

#ifndef STRINGCALCULATOR_THREAD_POOL_H
#define STRINGCALCULATOR_THREAD_POOL_H

#include <future>
#include <thread>
#include <queue>
#include <functional>
#include <atomic>


class thread_pool
{
public:

	thread_pool()
	{
		_start();
	}

	thread_pool(size_t num) :
		_num(num)
	{
		_start();
	}

	~thread_pool()
	{
		std::unique_lock<std::mutex> lk(_m);
		_stop = true;
		lk.unlock();

		cv.notify_all();
		for (auto & t : _threads)
			t.join();
	}

	template<class F, class ... Args>
	std::future<std::result_of_t<F(Args...)>> submit(F && func, Args && ... args)
	{
		using result_type = std::result_of_t<F(Args...)>;

		auto ptr_task = std::make_shared<std::packaged_task<result_type()>>(
			std::bind(std::forward<F>(func), std::forward<Args>(args)... ));

		auto result = ptr_task->get_future();

		std::lock_guard<std::mutex> lk(_m);
		_tasks.emplace([ptr_task]() -> void
		{
			(*ptr_task)();
		});

		cv.notify_one();
		return result;
	}

private:
	size_t _num = std::thread::hardware_concurrency();
	std::queue<std::function<void(void)>> _tasks;
	std::vector<std::thread> _threads;
	std::mutex _m;
	std::condition_variable cv;
	bool _stop;

	void _working_thread_func()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lk(_m);
			cv.wait(lk, [this]()
			{
				return _stop || !this->_tasks.empty();
			});

			if (_stop)
				return;

			auto task = _tasks.front();
			_tasks.pop();
			lk.unlock();

			task();
		}
	}

	void _start()
	{
		_stop = false;
		for (size_t i = 0; i < _num; i++)
			_threads.push_back(std::thread(&thread_pool::_working_thread_func, this));
	}
};

namespace sharik
{
	class ThreadPool {
	public:
		ThreadPool(size_t);
		template<class F, class... Args>
		auto enqueue(F&& f, Args&&... args)
				-> std::future<typename std::result_of<F(Args...)>::type>;
		~ThreadPool();
	private:
		// need to keep track of threads so we can join them
		std::vector< std::thread > workers;
		// the task queue
		std::queue< std::function<void()> > tasks;

		// synchronization
		std::mutex queue_mutex;
		std::condition_variable condition;
		bool stop;
	};

// the constructor just launches some amount of workers
	inline ThreadPool::ThreadPool(size_t threads)
			:   stop(false)
	{
		for(size_t i = 0;i<threads;++i)
			workers.emplace_back(
					[this]
					{
						for(;;)
						{
							std::function<void()> task;

							{
								std::unique_lock<std::mutex> lock(this->queue_mutex);
								this->condition.wait(lock,
													 [this]{ return this->stop || !this->tasks.empty(); });
								if(this->stop && this->tasks.empty())
									return;
								task = std::move(this->tasks.front());
								this->tasks.pop();
							}

							task();
						}
					}
			);
	}

// add new work item to the pool
	template<class F, class... Args>
	auto ThreadPool::enqueue(F&& f, Args&&... args)
	-> std::future<typename std::result_of<F(Args...)>::type>
	{
		using return_type = typename std::result_of<F(Args...)>::type;

		auto task = std::make_shared< std::packaged_task<return_type()> >(
				std::bind(std::forward<F>(f), std::forward<Args>(args)...)
		);

		std::future<return_type> res = task->get_future();
		{
			std::unique_lock<std::mutex> lock(queue_mutex);

			// don't allow enqueueing after stopping the pool
			if(stop)
				throw std::runtime_error("enqueue on stopped ThreadPool");

			tasks.emplace([task](){ (*task)(); });
		}
		condition.notify_one();
		return res;
	}

// the destructor joins all threads
	inline ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(queue_mutex);
			stop = true;
		}
		condition.notify_all();
		for(std::thread &worker: workers)
			worker.join();
	}
}

#endif //STRINGCALCULATOR_THREAD_POOL_H
