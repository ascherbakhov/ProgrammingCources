//
// Created by qazzer on 29/10/2019.
//

#include <chrono>
#include <thread>
#include <iostream>
#include <future>
#include "unit3_package_task.h"


std::mutex mtx_task;

int add(int x, int y)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    {
        std::lock_guard<std::mutex> lk(mtx_task);
        std::cout << "Add function runs in " << std::this_thread::get_id() << std::endl;
    }
    return x + y;
}

void normal_task()
{
    std::packaged_task<int(int,int)> task(add);
    std::future<int> fut = task.get_future();
    task(7,8);
    auto res = fut.get();
    {
        std::lock_guard<std::mutex> lk(mtx_task);
        std::cout << "Normal task execution thread" << std::this_thread::get_id() << " , result " << res
                  << std::endl;
    }
}

void thread_task()
{
    std::packaged_task<int(int,int)> task(add);
    std::future<int> fut = task.get_future();

    std::thread thread(std::move(task), 5, 6);
    thread.detach();
    auto  res = fut.get();
    {
        std::lock_guard<std::mutex> lk(mtx_task);
        std::cout<<"Thread task execution thread " << std::this_thread::get_id()<<" , result "<<res<<std::endl;
    }

}
void unit3_run_package_task()
{
    thread_task();
    normal_task();
}
