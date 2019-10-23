//
// Created by qazzer on 23/10/2019.
//

#include <mutex>
#include <list>
#include <iostream>
#include "unit2_mutex_lock.h"

std::list<int> simpleList;
std::mutex m;

void pushToList(int x)
{
    simpleList.push_back(x);
    throw std::runtime_error("Example");
}

void wrong_mutex_use()
{
    m.lock();
    try
    {
        pushToList(3);
        m.unlock(); //Need to unlock in all catch scenarios manually
    }
    catch(int a)
    {
        m.unlock(); //Need to unlock in all catch scenarios manually
    }
    catch (std::runtime_error& error)
    {
        m.unlock(); //Need to unlock in all catch scenarios manually
    }

}

void correct_mutex_use()
{
    std::lock_guard<std::mutex> l(m); // Mutex will be unlocked at lock_guard destructor
    try
    {
        pushToList(3);
    }
    catch (std::runtime_error& error)
    {

    }
}


void naive_stack_race_condition(naive_thread_safe_stack<int> stack)
{
    if (!stack.empty())
    {
        int value = stack.top();
        std::cout<<"Value is " << value << std::endl;
        // If this function is called at a several threads, pop can generate an exception when 1 element left
        // when top returns the same value. Or top can return other value, not than one we expected
        stack.pop();
    }
}
