//
// Created by qazzer on 26/10/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT3_THREAD_SAFE_QUEUE_H
#define MODERNCPPCONCURENCY_UNIT3_THREAD_SAFE_QUEUE_H


#include <mutex>
#include <condition_variable>
#include <queue>

template <class T>
class thread_safe_queue
{
    std::mutex m;
    std::condition_variable cv;
    std::queue<std::shared_ptr<T>> queue;
public:
    thread_safe_queue() = default;
    thread_safe_queue(thread_safe_queue const& otherQueue) = default;

    void push(T value)
    {
        std::lock_guard<std::mutex> lk(m);
        queue.push(std::make_shared<T>(value));
        cv.notify_one();
    }

    std::shared_ptr<T> pop()
    {
        std::lock_guard<std::mutex> lk(m);
        if (queue.empty())
        {
            return std::shared_ptr<T>();
        }
        else
        {
            std::shared_ptr<T> ref(queue.front());
            queue.pop();
            return ref;
        }
    }

    bool empty()
    {
        std::lock_guard<std::mutex> lk(m);
        return queue.empty();
    }

    size_t size()
    {
        std::lock_guard<std::mutex> lk(m);
        return queue.size();
    }

    std::shared_ptr<T> wait_pop()
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this](){ return !queue.empty();}); // Waiting for condition variable signal to continue execution
        std::shared_ptr<T> ref = queue.front();
        queue.pop();
        return ref;
    }
};



#endif //MODERNCPPCONCURENCY_UNIT3_THREAD_SAFE_QUEUE_H
