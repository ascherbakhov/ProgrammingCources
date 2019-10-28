//
// Created by qazzer on 28/10/2019.
//

#include <iostream>
#include <future>
#include "unit3_futures_asyncs.h"


int getSomMagicNumber()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 42;
}

void doSomeCalculation()
{
    std::cout << "Some other calculations" << std::endl;
}

void unit3_run_futures_code_example()
{
    //std::launch::async or std::launch::defer
    // first runs async task at a separate thread, second runs it in the same thread
    std::future<int> answer_future = std::async(getSomMagicNumber); //std::launch::async or std::launch::defer
    doSomeCalculation();
    std::cout << "Results of async is " << answer_future.get() << std::endl;

}


void printing()
{
    std::cout<<"This thread is " << std::this_thread::get_id() << std::endl;
}

int addition(int x, int y)
{
    std::cout<<"Addition runs on " << std::this_thread::get_id() << std::endl;
    return x + y;
}

int substraction(int x, int y)
{
    std::cout<<"Substruction runs on " << std::this_thread::get_id() << std::endl;
    return x - y;
}

void unit3_run_async_code()
{
    std::cout<<"Main thread is " << std::this_thread::get_id() << std::endl;

    int x = 100;
    int y = 50;

    std::future<void> f1 = std::async(std::launch::async, printing);
    std::future<int> f2 = std::async(std::launch::deferred, addition, x, y);
    std::future<int> f3 = std::async(std::launch::async | std::launch::deferred, substraction, x, y);

    f1.get();
    std::cout<<"Value recived from f2 future is  " << f2.get() << std::endl;
    std::cout<<"Value recived from f3 future is  " << f3.get() << std::endl;

}