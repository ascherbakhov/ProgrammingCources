//
// Created by qazzer on 29/10/2019.
//

#include <future>
#include <iostream>
#include "unit3_promises_shared_futures.h"

void print_fut_result(std::future<int> fut)
{
    std::cout << "Future result is " << fut.get() << std::endl;
}


void unit3_run_promise_code()
{
    std::promise<int> promise;
    std::future<int> fut = promise.get_future();

    std::thread thread(print_fut_result, std::move(fut));

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    promise.set_value(10);

    thread.join(); // Watch out, if join before setting the value, deadlock will occur
}
