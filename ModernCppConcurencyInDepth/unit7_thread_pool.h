//
// Created by qazzer on 25/11/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT7_THREAD_POOL_H
#define MODERNCPPCONCURENCY_UNIT7_THREAD_POOL_H

#include <atomic>
#include <functional>
#include <thread>
#include "unit3_thread_safe_queue.h"
#include "common.h"

class simple_thread_pool
{
    std::atomic<bool> is_done;
    thread_safe_queue<std::function<void()>> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread()
    {
        while(is_done)
        {
            std::shared_ptr<std::function<void()>> task;
            task = work_queue.pop();
            if (task)
            {
               *task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }

public:simple_thread_pool(): is_done(false), joiner(threads)
{
    try
    {
        int const thread_count = std::thread::hardware_concurrency();
        for (int i=0; i<thread_count; i++)
        {
            threads.emplace_back
                    (std::thread(&simple_thread_pool::worker_thread, this)
            );
        }
    }
    catch(...)
    {
        is_done = true;
        throw;
    }
}

    ~simple_thread_pool()
    {
        is_done = true;
    }

    template <typename Function_type>
    void submit(Function_type f)
    {
        work_queue.push(std::function<void()>(f));
    }
};

#endif //MODERNCPPCONCURENCY_UNIT7_THREAD_POOL_H
