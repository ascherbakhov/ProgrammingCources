//
// Created by qazzer on 30/10/2019.
//

#include <thread>
#include <iostream>
#include "unit4_thread_safe_queue.h"

unit4_thread_safe_queue<int> queue;
std::mutex print_mutex;

void producer()
{
    int i = 100;
    while (i > 0)
    {
        queue.push(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        i--;
    };
}


void consumer()
{
    int i = 100;
    while (i > 0)
    {
        auto res = queue.wait_pop();
        if (res != nullptr)
        {
            std::lock_guard<std::mutex> lg(print_mutex);
            std::cout << "Result is " << *res.get() << std::endl;

        }
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
        i--;
    };
}

void unit4_test_queue()
{
    std::thread thread_producer_1(producer);
    std::thread thread_producer_2(producer);
    std::thread thread_producer_3(producer);
    std::thread thread_producer_4(producer);

    std::thread thread_consumer_1(consumer);
    std::thread thread_consumer_2(consumer);

    thread_producer_1.join();
    thread_producer_2.join();
    thread_producer_3.join();
    thread_producer_4.join();

    thread_consumer_1.join();
    thread_consumer_1.join();
}