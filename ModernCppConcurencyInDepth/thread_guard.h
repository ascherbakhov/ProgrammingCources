//
// Created by qazzer on 22/10/2019.
//

#ifndef MODERNCPPCONCURENCYINDEPTH_THREAD_GUARD_H
#define MODERNCPPCONCURENCYINDEPTH_THREAD_GUARD_H

#include <thread>

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
#endif //MODERNCPPCONCURENCYINDEPTH_THREAD_GUARD_H
