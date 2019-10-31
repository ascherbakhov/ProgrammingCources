//
// Created by qazzer on 31/10/2019.
//

#ifndef MODERNCPPCONCURENCY_UNIT4_PARALLEL_QUICKSORT_H
#define MODERNCPPCONCURENCY_UNIT4_PARALLEL_QUICKSORT_H

#include <list>
#include <algorithm>

template <class T>
std::list<T> unit4_parallel_quicksort(std::list<T> input)
{
    if (input.size() < 2)
    {
        return input;
    }

    std::list<T> result;
    result.splice(result.begin(), input, input.begin()); // put fist element from input list in the 1st place of result
    T pivot = *result.begin();

    // Exchange elements with t < pivot predicate;
    // returns iterator fo 1st element of 2nd part
    auto divide_point = std::partition(input.begin(), input.end(),
            [&](T const& t)
            {
                return t < pivot;
            });

    std::list<T> lower_list;
    //Move to the end(or begin) of lower list from input list elements from begin to divide_point
    // input list now contains only upper part of list
    lower_list.splice(lower_list.end(), input, input.begin(), divide_point);

    auto new_lower(unit4_parallel_quicksort(std::move(lower_list)));
    auto new_upper(unit4_parallel_quicksort(std::move(input)));

    result.splice(result.begin(), new_lower);
    result.splice(result.end(), new_upper);

    return result;
}

void unit4_sort_example()
{
    std::list<int> l1= {21,123,123,21,31,2,4,21,1,23,123,123,234};
    auto res = unit4_parallel_quicksort(l1);
    for (auto x: res)
        std::cout <<" " << x;
}

#endif //MODERNCPPCONCURENCY_UNIT4_PARALLEL_QUICKSORT_H
