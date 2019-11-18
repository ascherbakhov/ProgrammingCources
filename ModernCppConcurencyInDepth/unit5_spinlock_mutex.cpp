//
// Created by qazzer on 18/11/2019.
//

#include "unit5_spinlock_mutex.h"

void unit5_spinlock_mutex::lock()
{
    // Returns false if not set, so while loop continues
    while(flag.test_and_set(std::memory_order_acquire));
}

void  unit5_spinlock_mutex::unlock()
{
    flag.clear(std::memory_order_release);
}