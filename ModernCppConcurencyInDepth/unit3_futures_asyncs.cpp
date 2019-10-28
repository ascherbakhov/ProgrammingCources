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

void run_futures_code_example()
{
    std::future<int> answer_future = std::async(getSomMagicNumber);
    doSomeCalculation();
    std::cout << "Results of async is " << answer_future.get() << std::endl;

}