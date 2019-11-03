//
// Created by qazzer on 04/11/2019.
//

#include "unit5_atomic_pointers_custom.h"

void unit5_atomic_pointer()
{
    int values[20];
    for (int i=0; i<20; i++)
        values[i] = i+1;

    std::atomic<int*> x_pointer(values);
    int* y_pointer = x_pointer + 3;

    std::cout << "Is lock free: " << x_pointer.is_lock_free() << std::endl;
    std::cout << "X_pointer is : " << *x_pointer << ", y_pointer is : " << * y_pointer << std::endl;

    x_pointer.store(y_pointer);
    std::cout << "X_pointer is : " << *(x_pointer.load()) << std::endl;

    bool retval = x_pointer.compare_exchange_weak(y_pointer, values + 10);
    std::cout << "Store operation successfull: " << (retval ? "yes" : "no") << std::endl;
    std::cout << "New value ponter: " << *x_pointer << std::endl;

    int* prev = x_pointer.fetch_sub(4);
    std::cout << "fetch_sub - X_pointer: " << *x_pointer << " Prev: "<< *prev << std::endl;
    prev = x_pointer.fetch_add(4);
    std::cout << "fetch_add - X_pointer: " << *x_pointer << " Prev: "<< *prev << std::endl;
    x_pointer++;
    std::cout << "++ - X_pointer: " << *x_pointer << std::endl;
    x_pointer--;
    std::cout << "-- - X_pointer: " << *x_pointer  << std::endl;
}


void unit5_atomic_custom()
{
    IntPair pair1(1, 2);
    AtomicIntPair atomic_pair(pair1);
}