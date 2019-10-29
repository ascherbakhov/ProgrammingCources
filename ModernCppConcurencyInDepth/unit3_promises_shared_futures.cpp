//
// Created by qazzer on 29/10/2019.
//

#include <future>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include "unit3_promises_shared_futures.h"

void print_fut_result(std::future<int>& fut)
{
    try
    {
        int x = fut.get();
        std::cout << "Future result is " << x << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << "Future result exception " <<e.what()<< std::endl;
    }

}

void unit3_run_promise_code()
{
    std::promise<int> promise;
    std::future<int> fut = promise.get_future();

    std::thread thread(print_fut_result, std::ref(fut));

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    promise.set_value(10);

    thread.join(); // Watch out, if join before setting the value, deadlock will occur
}

//////////////////////////////////////////////////////////////////////////////////////////

void square_root_promise(std::promise<int>& prom)
{
    int x = 1;
    try
    {
        std::cout << "Enter number to get square" << std::endl;
        std::cin >> x;
        if (x < 0)
        {
            throw(std::exception());
        }
        prom.set_value(std::sqrt(x));

    }
    catch (std::exception&)
    {
        std::cout << "Setting exception from "<<std::this_thread::get_id()<<" thread"<<std::endl;
        prom.set_exception(std::current_exception());
    }
}

void unit3_run_promise_exception()
{
    std::promise<int> promise;
    std::future<int> fut = promise.get_future();

    std::thread print_thread(print_fut_result, std::ref(fut));
    std::thread calculation_thread(square_root_promise, std::ref(promise));


    calculation_thread.join();
    print_thread.join(); // Watch out, if join before setting the value, deadlock will occur
}

///////////////////////////////////////////////

std::mutex unit3_sf_mutex;

void print_shared_future_result(std::shared_future<int>& fut)
{
    std::lock_guard<std::mutex> lk(unit3_sf_mutex);
    std::cout<<"Print shared future result "<<fut.get()<<std::endl;
}

void unit3_shared_future()
{
    std::promise<int> prom;
    std::shared_future<int> fut(prom.get_future());

    std::thread thread1(print_shared_future_result, std::ref(fut));
    std::thread thread2(print_shared_future_result, std::ref(fut));

    prom.set_value(10);

    thread1.join();
    thread2.join();
}