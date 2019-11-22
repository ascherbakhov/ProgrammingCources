//
// Created by qazzer on 22/11/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT6_LOCK_FREE_STACK_REF_COUNT_H
#define MODERNCPPCONCURENCY_UNIT6_LOCK_FREE_STACK_REF_COUNT_H

#include <memory>
#include <atomic>

template <class T>
class lock_free_stack_ref_count{
    struct node;

    struct node_wrapper{
        //Increase when read it
        int external_count;
        node* pointer;
    };

    struct node{
        std::shared_ptr<T> value;
        node_wrapper next;
        //Decrease it when reader finishes with node
        std::atomic<int> internal_count;

        node(T const& _data): value(std::make_shared<T>(_data)), internal_count(0){}
    };

    std::atomic<node_wrapper> head;

public:
    void push(T const& data)
    {
        node_wrapper new_node;
        new_node.pointer = new node();
        //Set external count to 1 since now stack has a link to a new node
        new_node.external_count = 1;
        new_node.pointer->next = head.load();
        while(!head.compare_exchange_weak(new_node.pointer->next, new_node));
    }

    void increment_head_ref_count(node_wrapper& old_counter)
    {
        node_wrapper new_counter;
        //Other thread can also try to increment external count for this node
        //So we need to retry incrementing  while counter on node is updating
        //Or new node added at previous iteration
        do {
            new_counter = old_counter;
            ++new_counter.external_count;
        }//strong guarantees that if head==old_head, then no further iterations will happen
        while(!head.compare_exchange_strong(old_counter, new_counter));

        old_counter.external_count = new_counter.external_count;
    }

    std::shared_ptr<T> pop()
    {
        node_wrapper old_head = head.load();

        for (;;)
        {
            increment_head_ref_count(old_head);

            node *const pointer = old_head.pointer;
            if (!pointer)
                return std::shared_ptr<T>();

            //Try to detach node from stack, if it fails, we need to try again in for loop
            if (head.compare_exchange_strong(old_head, pointer->next))
            {
                std::shared_ptr<T> res;
                res.swap(pointer->value);

                //Incrementing in this pop function + stack reference = 2
                int const current_external_count = old_head.external_count - 2;

                if (pointer->internal_count.fetch_add(current_external_count) == 0)
                {
                    delete pointer;
                }
                return res;
            } // If 'if'statement below failed, then we dont have reference to that node,
            //so we have to decrease internal count and delete if needed
            else if (pointer->internal_count.fetch_sub(1) == 0)
            {
                delete pointer;
            }
        }
    }
};

#endif //MODERNCPPCONCURENCY_UNIT6_LOCK_FREE_STACK_REF_COUNT_H
