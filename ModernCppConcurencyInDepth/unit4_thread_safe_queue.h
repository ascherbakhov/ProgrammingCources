//
// Created by qazzer on 30/10/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT4_THREAD_SAFE_QUEUE_H
#define MODERNCPPCONCURENCY_UNIT4_THREAD_SAFE_QUEUE_H


#include <memory>
#include <mutex>
#include <condition_variable>

template <class T>
class unit4_thread_safe_queue
{
    struct node {
        std::unique_ptr<node> next;
        std::shared_ptr<T> value;
        node(T _data): value(std::move(_data)) {}
        node(){}
    };

    std::unique_ptr<node> head;
    node* tail;
    std::mutex head_mutex;
    std::mutex tail_mutex;
    std::condition_variable cv;

    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> lk(head_mutex);

        // Waiting for condition variable signal to continue execution
        cv.wait(lk, [this]()
        {
            return (head.get() != get_tail());
        });
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head);
        return old_head;
    }
public:
    unit4_thread_safe_queue() : head(new node), tail(head.get())  {} // creating dummy node at constructor
    unit4_thread_safe_queue(unit4_thread_safe_queue const&) = delete;
    void operator=(unit4_thread_safe_queue const &) = delete;

    void push(T value)
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(value)));
        std::unique_ptr<node> new_node(new node);
        node * const new_tail = new_node.get();
        {
            std::lock_guard<std::mutex> lgt(tail_mutex);
            tail->value = new_data;
            tail->next = std::move(new_node);
            tail = new_tail;
        }
        cv.notify_one();

        //add new dummy node to the tail


        tail->next = std::move(new_node);
        tail = new_tail;
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lgh(head_mutex);

        if (head.get() == get_tail())
        {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(head->data);
        std::unique_ptr<node> const old_head = std::move(head);
        head = std::move(old_head->next);
        return res;
    }

    node* get_tail()
    {
        std::lock_guard<std::mutex> tlg(tail_mutex);
        return tail;
    }

    std::shared_ptr<T> wait_pop()
    {
        std::unique_ptr<node> old_head = wait_pop_head();
        return old_head ? old_head->value : std::shared_ptr<T>();
    }
};


void unit4_test_queue();

#endif //MODERNCPPCONCURENCY_UNIT4_THREAD_SAFE_QUEUE_H
