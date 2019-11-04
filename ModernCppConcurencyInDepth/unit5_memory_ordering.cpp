//
// Created by qazzer on 04/11/2019.
//

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <assert.h>
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

void unit5_memory_ordering_common()
{
    // Just examples of using memory order
    // std::memory_order_seq_cst is default memory order option
    std::atomic<int> x;

    x.store(4, std::memory_order_seq_cst);
    x.load(std::memory_order_seq_cst);

    int y = 11;
    x.compare_exchange_weak(y, std::memory_order_release, std::memory_order_relaxed);
}


///////////////////////////////////////
//Just a divider
//////////////////////////////////////
std::atomic<bool> mo_x, mo_y;
std::atomic<int>mo_z;

void write_x()
{
    mo_y.store(true, std::memory_order_seq_cst);
}

void write_fl()
{
    mo_x.store(true, std::memory_order_seq_cst);
}

void read_x_then_y()
{
    while(!mo_x.load(std::memory_order_seq_cst));

    if(mo_y.load(std::memory_order_seq_cst))
    {
        mo_z++;
    }
}

void read_y_then_x()
{
    while(!mo_y.load(std::memory_order_seq_cst));

    if(mo_x.load(std::memory_order_seq_cst))
    {
        mo_z++;
    }
}

void unit5_memory_order_seq_cst()
{
    // Memory order is about core cashes, I suppose.
    // If threads get on different cores, then due to
    // cache update policy it can store different values
    // at a time for one variable. Memory_order_seq_cst
    // guarantees that cache would be consistent for
    // all threads
    mo_x = false;
    mo_y = false;
    mo_z = 0;

    std::thread thread1(write_x);
    std::thread thread2(write_fl);
    std::thread thread3(read_x_then_y);
    std::thread thread4(read_y_then_x);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    assert(mo_z != 0);
}