//
// Created by qazzer on 18/11/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT6_LOCKFREE_HAZARD_POINTERS_H
#define MODERNCPPCONCURENCY_UNIT6_LOCKFREE_HAZARD_POINTERS_H


#include <atomic>
#include <memory>
#include <thread>
/*
 *
 * Not ended, just main idea here
 */
template <class T>
class unit6_lockfree_stack_hazard
{
    struct node{
        node* next;
        std::shared_ptr<T> value;

        explicit node(T const & value): value(std::make_shared<T>(value)){};
    };
    std::atomic<node*> head;
    std::atomic<node*> to_be_deleted;
    static int const max_hazard_pointers = 100;

    struct hazard_pointer {
        std::atomic<std::thread::id> id;
        std::atomic<void*> pointer;
    };

    static hazard_pointer hazard_pointers[max_hazard_pointers];

    class hp_manager{
        hazard_pointer* hp;
    public:
        hp_manager():hp(nullptr)
        {
            for (unsigned int i=0; i < max_hazard_pointers; i++)
            {
                std::thread::id old_id;
                // Look for appropriate gap for current thread hazard_pointer
                if (hazard_pointers[i].id.compare_exchange_strong(old_id, std::this_thread::get_id()))
                {
                    hp = &hazard_pointers[i];
                    break;
                }
            }

            if (!hp)
                throw std::runtime_error("no hazard pointers available");
        }

        ~hp_manager()
        {
            hp->pointer.store(nullptr);
            hp->id.store(std::this_thread::get_id());
        }

        std::atomic<void*> &get_pointer()
        {
            return hp->pointer;
        }
    };

    std::atomic<void*> & get_hazard_pointer_for_current_thread()
    {
        // One instance of hp_manager on one thread;
        static thread_local hp_manager hz_owner;
        return hz_owner.get_pointer();
    }

    bool any_outstanding_hazards(node* p)
    {
        for(unsigned int i = 0; i < max_hazard_pointers; ++i)
        {
            if (hazard_pointers[i].pointer.load() == p)
            {
                return true;
            }
        }
        return false;
    }
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

    void reclaim_later(node* node)
    {
        node->next = to_be_deleted.load();
        while(!to_be_deleted.compare_exchange_weak(node->next, node));
    }

    void delete_nodes_with_no_hazards()
    {
        /*
         * Detach to_be_deleted and try to remove those which without hazard_pointers
         * We can return to to_be_deleted if any_outstanding_hazard
         */
        node* current = to_be_deleted.exchange(nullptr);

        while(current)
        {
            node* const next = current->next;

            if (!any_outstanding_hazards(current))
            {
                reclaim_later(current);
            }
            else
            {
                delete current;
            }
            current = next;
        }
    }

    std::shared_ptr<T> pop()
    {
        //allocate hazard_pointer
        std::atomic<void*> & hp = get_hazard_pointer_for_current_thread();
        node* old_head = head.load();

        // Here you need to do two things atomically: first, write a pointer
        // to the head of the list in hazard pointer, temporarily protecting it from
        // delete, and secondly, extract this element from the list by moving
        // head-> next pointer to the next element.
        //
        // The inner loop solves the first problem, the outer loop solves the second.
        //
        // Protection against deleting an element with hazard pointer only for
        // that is needed to be able to safely execute
        // compare_exchange_strong (old_head, old_head-> next). Other threads by
        // at the end of the work, check hazard pointers and delete the object
        // only if there are no links to it (see below)
        //
        do {
            node* temp;
            do{
                temp=old_head;
                hp.store(old_head);
                old_head=head.load();
            }
            while (temp != old_head);
        }
        while(old_head && !head.compare_exchange_strong(old_head, old_head->next));

        //can clear the hazard pointer since old_head is only visible to this thread
        hp.store(nullptr);

        std::shared_ptr<T> res;
        if (old_head)
        {
            res.swap(old_head->value);

            if (any_outstanding_hazards(old_head))
            {
                reclaim_later(old_head);
            }
            else
            {
                delete old_head;
            }
            delete_nodes_with_no_hazards();
        }
        return res;
    };
};



#endif //MODERNCPPCONCURENCY_UNIT6_LOCKFREE_HAZARD_POINTERS_H
