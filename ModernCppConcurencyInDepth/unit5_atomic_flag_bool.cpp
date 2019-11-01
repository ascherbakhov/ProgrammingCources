//
// Created by qazzer on 01/11/2019.
//

#include <atomic>
#include <iostream>
#include "unit5_atomic_flag_bool.h"


void unit5_atomic_flag()
{
    //guarantied to be lock free
    //std::atomic_flag flag1 = true; incorrect
    std::atomic_flag flag2 = ATOMIC_FLAG_INIT;

    std::cout <<"1. Previous flag value " << flag2.test_and_set() << std::endl;
    std::cout <<"2. Previous flag value " << flag2.test_and_set() << std::endl;


    flag2.clear();
    std::cout <<"3. Previous flag value " << flag2.test_and_set() << std::endl;
}


// Not copy constructable

void unit5_atomic_bool()
{
    std::atomic<bool> flag1(false);
    std::cout << "flag1 = " << flag1 << std::endl;


    // std::atomic<bool> flag2(flag1);

    // Not copy assignable
    // std::atomic<bool> flag3 = flag1;

    // Cannot invoke operator= for non atomic too
    // bool just_bool_2 = true;
    // std::atomic<bool> flag5 = just_bool;

    bool just_bool = true;
    std::atomic<bool> flag4(just_bool);
    std::cout << "Atomic boolean is lock free -" << (flag4.is_lock_free() ? "yes" : "no") << std::endl;
    std::cout << "flag4 = " << flag4 << std::endl;

    // Store examples
    flag4.store(false);
    std::cout << "after storing flag4 = " << flag4 << std::endl;

    // Load examples
    bool loaded = flag4.load();
    std::cout << "Printing loaded value " << loaded << std::endl;

    //Exchange example
    bool exchange_res = flag4.exchange(true);
    std::cout << "Previous=" << exchange_res << ", current=" << flag4 << std::endl;


}


void unit5_compare_exchange()
{
    std::atomic<int> number(20);

    int expected_value = 20;
    std::cout << "Previous expected value " << expected_value << std::endl;
    // change atomic value to the second argument if current value is equal to expected value
    // returns 1 on success, 0 on fail. It is atomic operation
    // weak doesn't guarantee successfullness even if atomic is equal to expected value
    // on a machines without such instructions
    // used fo spinlock(locks) realization. Also look at ABA probmlem
    //  can be used to implement a more sophisticated spin lock that stores the thread identifier in the lock
    // also for optimistic transactional data structures
    bool return_val = number.compare_exchange_weak(expected_value, 6);

    std::cout << "Operation successful " << (return_val ? "yes" : "no") << std::endl;
    std::cout << "current expected value" << expected_value << std::endl;
    std::cout << "current atomic number " << number.load() << std::endl;

}