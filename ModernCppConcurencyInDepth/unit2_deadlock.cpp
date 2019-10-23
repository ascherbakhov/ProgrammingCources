//
// Created by qazzer on 24/10/2019.
//

#include "unit2_deadlock.h"
#include <mutex>
#include <thread>

std::mutex m1;
std::mutex m2;

void deadlock_f1()
{
    std::lock_guard<std::mutex>l1(m1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::lock_guard<std::mutex>l2(m2);
}

void deadlock_f2()
{
    std::lock_guard<std::mutex>l1(m2);
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    std::lock_guard<std::mutex>l2(m1);
}

void run_code_deadlock()
{
    std::thread thread1(deadlock_f1);
    std::thread thread2(deadlock_f2);

    thread1.join();
    thread2.join();
}