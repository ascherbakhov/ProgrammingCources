//
// Created by qazzer on 04/11/2019.
//

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "unit5_memory_ordering.h"

std::atomic<bool> ready_flag;
std::vector<int> data_vector;

void reader()
{
    while(!ready_flag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    // 2. "Synchronization" between pushing back an element to vector and printing its element to console
    // even non-atomic operation of pushing back from writer thread is guarantied to be visible to reader thread

    // 3. Pushing back element has "Inter-thread-happened-before" relationship with printing it to console
    std::cout << data_vector[0] << std::endl;
}

void writer()
{
    // 1.Pushing to data_vector "Happened before" storing flag
    data_vector.push_back(5);
    ready_flag.store(true);
}


void unit5_important_relationships()
{
    std::thread reader_thread(reader);
    std::thread writer_thread(writer);

    reader_thread.join();
    writer_thread.join();
}