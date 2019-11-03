//
// Created by qazzer on 04/11/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT5_ATOMIC_POINTERS_CUSTOM_H
#define MODERNCPPCONCURENCY_UNIT5_ATOMIC_POINTERS_CUSTOM_H

#include <iostream>
#include <atomic>

// Must not have virtual functions
// Every non static member should have trivial copy assigment operator
// Not a descendent of virtual base class
// Bitwise equality comparable
struct IntPair {
    int a, b;
    IntPair(int a, int b): a(a), b(b) {};
};

typedef std::atomic<IntPair> AtomicIntPair;

void unit5_atomic_pointer();
void unit5_atomic_custom();

#endif //MODERNCPPCONCURENCY_UNIT5_ATOMIC_POINTERS_CUSTOM_H
