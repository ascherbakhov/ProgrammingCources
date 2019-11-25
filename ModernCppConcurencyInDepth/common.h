//
// Created by qazzer on 22/10/2019.
//

#ifndef MODERNCPPCONCURENCYINDEPTH_THREAD_GUARD_H
#define MODERNCPPCONCURENCYINDEPTH_THREAD_GUARD_H

#include <thread>
#include <vector>

class thread_guard {
    std::thread &t;

public:
    explicit thread_guard(std::thread& t) : t(t) {}
    thread_guard(thread_guard&) = delete;
    thread_guard& operator=(thread_guard&) = delete;

    ~thread_guard()
    {
        if (t.joinable())
        {
            t.join();
        }
    }

};

class join_threads {
    std::vector<std::thread>& threads;

public:
    explicit join_threads(std::vector<std::thread> & _threads) :
            threads(_threads)
    {}

    ~join_threads()
    {
        for (long i = 0; i < threads.size(); i++)
        {
            if (threads[i].joinable())
                threads[i].join();
        }
    }

};
#endif //MODERNCPPCONCURENCYINDEPTH_THREAD_GUARD_H
