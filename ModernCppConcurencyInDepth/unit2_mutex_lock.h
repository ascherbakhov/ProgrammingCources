//
// Created by qazzer on 23/10/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT2_MUTEX_LOCK_H
#define MODERNCPPCONCURENCY_UNIT2_MUTEX_LOCK_H

#include <stack>
#include <mutex>
#include <memory>

void wrong_mutex_use();
void correct_mutex_use();


template<class T>
class naive_thread_safe_stack
{
    std::stack<T> stack;
    std::mutex m;
public:
    void push(T element)
    {
        std::lock_guard<std::mutex> l(m);
        stack.push(element);
    }
    void pop()
    {
        std::lock_guard<std::mutex> l(m);
        stack.pop();
    }

    T& top()
    {
        std::lock_guard<std::mutex> l(m);
        return stack.top();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> l(m);
        return stack.empty();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> l(m);
        return stack.size();
    }
};

void naive_stack_race_condition(naive_thread_safe_stack<int> stack);

template <class T>
class less_naive_thread_safe_stack
{
    std::stack<T> stack;
    std::mutex m;
public:
    void push(T element)
    {
        std::lock_guard<std::mutex> l(m);
        stack.push(std::make_shared<T>(element));
    }

    //Rece condition eliminated by combining top and pop
    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> l(m);
        if (stack.empty())
        {
            throw std::runtime_error("stack is empty");
        }
        std::shared_ptr<T> res{stack.top()};
        stack.pop();
        return res;
    }

    void  pop(T& value)
    {
        std::lock_guard<std::mutex> l(m);
        if (stack.empty())
        {
            throw std::runtime_error("stack is empty");
        }
        value = std::dynamic_pointer_cast(stack.top().get());
        stack.pop();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> l(m);
        return stack.empty();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> l(m);
        return stack.size();
    }
};

#endif //MODERNCPPCONCURENCY_UNIT2_MUTEX_LOCK_H
