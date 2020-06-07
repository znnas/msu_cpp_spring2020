#include <iostream>
#include <chrono>
#include <queue>
#include <vector>
#include <string>
#include <cstdlib>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <functional>

using namespace std;

class ThreadPool
{
private:
    vector<thread> threads_;
    queue<function<void()>> tasks;

    mutex queue_mutex;
    condition_variable condition;
    bool stop;

public:
    explicit ThreadPool(size_t poolSize) : stop(false)
    {
        for (size_t i=0; i<poolSize; i++)
            threads_.emplace_back(
            [this]{
				while(true) {
					function<void()> task;
					{
						unique_lock<mutex> lock(this->queue_mutex);
						this->condition.wait(lock,
								[this] {return this->stop || !this->tasks.empty();});
						if (this->stop && this->tasks.empty())
							return;

						task = move(this->tasks.front());
						this->tasks.pop();
					}
					task();
				}
        	}
        );
    }

    template <class Func, class... Args>
    auto exec(Func func, Args... args)->future<decltype(func(args...))>
    {
        using return_type = decltype(func(args...));
        auto task = make_shared<packaged_task<return_type()>>(
            bind(forward<Func>(func), forward<Args>(args)...)
            );
        future<return_type> res = task->get_future();
        {
            unique_lock<mutex> lock(queue_mutex);
            if (stop) {
                throw runtime_error("ThreadPool is stopped");
            }
            tasks.emplace([task]() { (*task)( ); });
        }
        condition.notify_one();
        return res;
    }

    ~ThreadPool()
    {
        {
            unique_lock<mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for (thread& thread_: threads_) {
            thread_.join();
        }
    }
};


int main()
{
    ThreadPool pool(4);
    vector<future<string>> results;

    for (int i=0; i<19; i++) {
        results.emplace_back(
            pool.exec([i] {
        	cout << "start task N " << i << ", thread id=" << this_thread::get_id() << +"\n";

            int rd = rand() % 100;
            this_thread::sleep_for(chrono::milliseconds(rd));

            cout << " end  task N " << i << ", thread id=" << this_thread::get_id() << +"\n";
            return string("\n");
        })
        );
    }

    for (auto&& result: results)
        cout << result.get();
    cout << endl;

    return 0;
}
