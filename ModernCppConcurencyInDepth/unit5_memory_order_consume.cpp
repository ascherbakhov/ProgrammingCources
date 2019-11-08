//
// Created by qazzer on 08/11/2019.
//

#include <string>
#include <atomic>
#include <cassert>
#include <thread>
#include "unit5_memory_order_consume.h"

/*
 * Carries-a-dependency-to: if result of op A is used at operation B then A c-d-t B
 * Transitive synchronization: if one thread is synchronized with the second,
 * and second synchronized with  third, than 1st synchronized with third too.
 */

struct X{
    int i;
    std::string s;
};
 std::atomic<X*> p;
 std::atomic<int> a;

 void create_x()
 {
     X* x = new X();
     x->i = 42;
     x->s = "wfsdsdf";

     a.store(10, std::memory_order_relaxed);
     p.store(x, std::memory_order_release);
 }

 void use_x()
 {
     X*x;
     while(!(x = p.load(std::memory_order_consume)));
     assert(x->i == 42);
     assert(x->s == "wfsdsdf");
     // Can fire or not because consume does not synchronize  previous store operations on the create_x thread.
     // Consume will be limited only for direct dependencies, but a is not a dependency for p.store
     assert(a.load(std::memory_order_relaxed) == 10);
 }

 void unit5_memory_order_consume()
 {
     std::thread thread1(create_x);
     std::thread thread2(use_x);

     thread1.join();
     thread2.join();
 }