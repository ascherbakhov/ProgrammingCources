//
// Created by qazzer on 11/11/2019.
//

#include <queue>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include "unit5_release_sequence.h"

/*
 * Release sequence
 * If some atomic is store-released and several other threads perform read-modify-write operations on that atomic,
 * a "release sequence" is formed: all threads that perform the read-modify-writes to the same atomic synchronize
 * with the first thread and each other even if they have no memory_order_release semantics.
 * This makes single producer - multiple consumers situations possible without imposing unnecessary
 * synchronization between individual consumer threads.
 */

std::queue<int> rqueue;
std::atomic<int> count;
std::mutex m_rs;

void process(int i)
{

    std::lock_guard<std::mutex> lock(m_rs);
    std::cout << "id " << std::this_thread::get_id() << ": " << i << std::endl;
}

void writer_thread()
{
    for (int i = 0; i < 20; i++)
        rqueue.push(i);

    count.store(20, std::memory_order_release);
}

void reader_thread()
{
    int ind = 0;
    while(true)
    {
        if (!(ind = count.fetch_sub(1, std::memory_order_relaxed) <= 0))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        process(ind);
    }
}

void unit5_release_sequence()
{
    std::thread thread1(writer_thread);
    std::thread thread2(reader_thread);
    std::thread thread3(reader_thread);

    thread1.join();
    thread2.join();
    thread3.join();
}