//
// Created by qazzer on 23/10/2019.
//

#include <queue>
#include <thread>
#include <iostream>

void engine_crew_run(std::queue<int> &commandsQueue, bool &done_flag)
{
    while (true)
    {
        if (done_flag)
        {
            std::cout << "Cleaners work completed" << done_flag << "Remain commands len:" << commandsQueue.size()<< std::endl;
            return;
        }

        if (!commandsQueue.empty())
        {
            auto command = commandsQueue.front();
            if (command != 0)
            {
                if (command == 1)
                {
                    std::cout << "Stop engine!" << std::endl;
                } else if (command == 2)
                {
                    std::cout << "Full ahead!" << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            commandsQueue.pop();
        } else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }

    }
}

void cleaning_crew_run(std::queue<int> &commandsQueue, bool &done_flag)
{
    while (true)
    {
        if (done_flag)
        {
            std::cout << "Cleaners work completed" << done_flag << "Remain commands len:" << commandsQueue.size()
                      << std::endl;
            return;
        }
        if (!commandsQueue.empty())
        {
            auto command = commandsQueue.front();
            if (command != 0)
            {
                std::cout << "Cleaning things!" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            commandsQueue.pop();
        } else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }
}

// TODO: cleaning_crew and engine_crew has a lot of same code. Get rid of this code.
void task3_run()
{
    std::cout << "Hello, Captain!" << std::endl;
    std::cout << "Please choose command!" << std::endl;
    std::cout << "1 is for cleaning!" << std::endl;
    std::cout << "2 if for full speed ahead!" << std::endl;
    std::cout << "3 is for stop!" << std::endl;
    std::cout << "100 is for exit!" << std::endl;

    auto engine_work_crew = std::queue<int>();
    auto clean_work_queue = std::queue<int>();
    bool done_flag = false;

    std::thread engine_crew(engine_crew_run, std::ref(engine_work_crew), std::ref(done_flag));
    std::thread cleaning_crew(cleaning_crew_run, std::ref(clean_work_queue), std::ref(done_flag));

    int cmd = 0;

    while (cmd != 5)
    {
        cmd = 0;
        std::cin >> cmd;
        switch (cmd)
        {
            case 1 ... 2:
            {
                engine_work_crew.push(cmd);
                break;
            }
            case 3:
            {
                clean_work_queue.push(cmd);
                break;
            }
            case 5:
                done_flag = true;
                break; // Do waiting for others threads to stop
            default:
                std::cout << "Invalid command" << std::endl;
                break;
        }
    }

    if (engine_crew.joinable())
    {
        engine_crew.join();
    }

    if (cleaning_crew.joinable())
    {
        cleaning_crew.join();
    }

}