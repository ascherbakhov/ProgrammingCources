//
// Created by qazzer on 08/11/2019.
//

#include <atomic>
#include <thread>
#include <cassert>
#include "unit5_memory_order_acquire_release.h"

std::atomic<bool> moar_x, moar_y;
std::atomic<int> moar_z;
/*
 * Suitable tagged store value synchronized with suitable tagged
 * load value. And despite the fact x is tagged relaxed,
 * y is still acquire-release, and since store x is before store y,
 * after while loop ended not only y, but all store variables before
 * in the same thread(moacqrel_write_x_then_y) would be also synchronized.
 * That's how acquire-release works. So assert will never fire.
 */
void moacqrel_write_x_then_y()
{
    moar_x.store(10, std::memory_order_relaxed);
    moar_y.store(10, std::memory_order_release);
}

void moacqrel_read_y_then_x()
{
    while (!moar_y.load(std::memory_order_acquire));
    if (moar_x.load(std::memory_order_relaxed))
    {
        moar_z++;
    }
}


void unit5_memory_order_acquire_release()
{
    moar_x = false;
    moar_y = false;
    moar_z = 0;

    std::thread thread1(moacqrel_write_x_then_y);
    std::thread thread2(moacqrel_read_y_then_x);

    thread1.join();
    thread2.join();

    assert(moar_z != 0);
}
//////////////////////////////////////////////////////////////

//////////////////////////////////////
std::atomic<bool> moar_a, moar_b;
std::atomic<int>moar_c;

/* Though all memory orders in this threads are acquire-release
 * assert still can fire. It is because of the fact, that
 * moar_a and moar_b is synchronized only with particular thread.
 * Acquire-release do the visibility for all the shared states
 * appeared before the release operation from one thread to another.
 * So between thread1 and thread3 moar_a can be synchronized, but moar_b
 * might not. The same is for thread2 and thread4. So assert can be fired
 *
*/
void write_a()
{
    moar_a.store(true, std::memory_order_release);
}

void write_b()
{
    moar_b.store(true, std::memory_order_release);
}

void read_a_then_b()
{
    while(!moar_a.load(std::memory_order_acquire));

    if(moar_b.load(std::memory_order_acquire))
    {
        moar_c++;
    }
}

void read_b_then_a()
{
    while(!moar_b.load(std::memory_order_acquire));

    if(moar_a.load(std::memory_order_acquire))
    {
        moar_c++;
    }
}

void unit5_memory_order_acquire_release_negative()
{

    moar_a = false;
    moar_b = false;
    moar_c = 0;

    std::thread thread1(write_a);
    std::thread thread2(write_b);
    std::thread thread3(read_a_then_b);
    std::thread thread4(read_b_then_a);

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();

    assert(moar_c != 0);
}