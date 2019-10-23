//
// Created by qazzer on 24/10/2019.
//

#include "unit2_deadlock_uniquelock.h"
#include <mutex>
#include <thread>
#include <iostream>

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

void doSomething_1()
{
    std::cout << "Operation 1" << std::endl;

}

void doSomething_2()
{
    std::cout << "Operation 2" << std::endl;

}


std::unique_lock<std::mutex> get_lock()
{
    std::mutex m;
    std::unique_lock<std::mutex> lk(m);
    doSomething_1();
    return lk;
}

void run_several_mutex_lock()
{
    std::mutex mt1, mt2;
    std::unique_lock<std::mutex> lk1(mt1, std::defer_lock);
    std::unique_lock<std::mutex> lk2(mt2, std::defer_lock);
    std::lock(lk1, lk2);
    // locks several lockable objects using deadlock avoidance algorithm, all or nothing operation
    //also std::scoped_lock, that is similar to std::thread_guard and std::lock together, also recursive_locks
}

void run_uniquelock_code()
{
    //transferring ownership of mutex from one scope into another
    // calling of 1 and 2 function is under protection of same mutex
    //ownership can be transferred for example from one to another instance of class
    std::unique_lock<std::mutex> lk(get_lock());
    doSomething_2();
}