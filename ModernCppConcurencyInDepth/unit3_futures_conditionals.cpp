//
// Created by qazzer on 26/10/2019.
//

#include "unit3_futures_conditionals.h"

bool arrived = false;
int total_distance = 10;
int distance_covered = 0;
std::condition_variable cv;
std::mutex mt;

void keep_moving()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "1 second" << std::endl;
        distance_covered++;


        if (distance_covered == total_distance)
        {
            cv.notify_one();
            break;
        }
    }
    std::cout << "Moving finised" << std::endl;
}

void ask_driver_to_wake_up()
{
    std::unique_lock<std::mutex> ul(mt);
    //Cv wake up  due to notification from another thread or spurious(OS)
    cv.wait(ul, [](){ return distance_covered == total_distance;});
    std::cout<< "Finally im here" << std::endl;
}

void run_code()
{
    std::thread driving_thread(keep_moving);
    std::thread passengers_thead(ask_driver_to_wake_up);

    driving_thread.join();
    passengers_thead.join();
}
