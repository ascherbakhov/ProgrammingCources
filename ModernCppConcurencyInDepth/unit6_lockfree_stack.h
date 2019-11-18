//
// Created by qazzer on 18/11/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT6_LOCKFREE_STACK_H
#define MODERNCPPCONCURENCY_UNIT6_LOCKFREE_STACK_H


#include <atomic>
#include <memory>

template <class T>
class unit6_lockfree_stack
{
    struct node{
        node* next;
        std::shared_ptr<T> value;

        explicit node(T const & value): value(std::make_shared<T>(value)){};
    };
    std::atomic<node*> head;
public:
    void push(T const & value)
    {
        node* const new_node = new node(value);
        new_node->next = head.load();
        /*
         * Atomically compares the object of head with new_node->next, and if those are bitwise-equal,
         * replaces the former with new_node. Otherwise, loads the head value into new_node->next.
         */
        while(!head.compare_exchange_weak(new_node->next, new_node));
    };
    std::shared_ptr<T> pop()
    {
        node* old_head = head.load();
        //Infinite cycle waiting for new value
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next));
        return old_head ? old_head->value : std::shared_ptr<T>();
        //delete old_head; // Deal with memory leak later
    };
};



#endif //MODERNCPPCONCURENCY_UNIT6_LOCKFREE_STACK_H
