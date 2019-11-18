//
// Created by qazzer on 18/11/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT5_SPINLOCK_MUTEX_H
#define MODERNCPPCONCURENCY_UNIT5_SPINLOCK_MUTEX_H


#include <atomic>

class unit5_spinlock_mutex
{
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    unit5_spinlock_mutex() = default;

    void lock();
    void unlock();
};


#endif //MODERNCPPCONCURENCY_UNIT5_SPINLOCK_MUTEX_H
