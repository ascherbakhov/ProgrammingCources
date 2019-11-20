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
    std::atomic<int> threads_in_pop;
    std::atomic<node*> to_be_deleted;
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

    void delete_nodes(node* nodes)
    {
        //Other thread cannot access to node, so it's safely delete them
        while (nodes)
        {
            node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void try_reclaim(node* old_head)
    {
        if (threads_in_pop == 1)
        {
            delete old_head;

            node* claimed_list = to_be_deleted.exchange(nullptr);

            //Double check is needed, because if we don't check it here, it can delete nodes from to_be_deleted
            // though for other thread old_head can be in to_be_deleted list, and in else operator it fails(ABC ex).
            if (!--threads_in_pop)
            {
                delete_nodes(claimed_list);
            }
            else if (claimed_list)
            {
                //Just look for last node
                node* last = claimed_list;
                while(node* const next = last->next)
                {
                    last = next;
                }

                //Unite claimed with to_be_deleted
                last->next = to_be_deleted;
                while(!to_be_deleted.compare_exchange_weak(last->next, claimed_list));
                last->next = to_be_deleted;
            }
        }
        else
        {
            old_head->next = to_be_deleted;
            while(!to_be_deleted.compare_exchange_weak(old_head->next, old_head));
            --threads_in_pop;
        }
    }

    std::shared_ptr<T> pop()
    {
        ++threads_in_pop;
        node* old_head = head.load();
        //Infinite cycle waiting for new value
        while (old_head && !head.compare_exchange_weak(old_head, old_head->next));

        std::shared_ptr<T> res;
        if (old_head)
            res.swap(old_head->value);

        try_reclaim(old_head);

        return res;
    };
};



#endif //MODERNCPPCONCURENCY_UNIT6_LOCKFREE_STACK_H
