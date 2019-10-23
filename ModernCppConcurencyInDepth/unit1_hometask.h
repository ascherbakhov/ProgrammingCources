//
// Created by qazzer on 23/10/2019.
//

#ifndef MODERNCPPCONCURENCYINDEPTH_TASK3_H
#define MODERNCPPCONCURENCYINDEPTH_TASK3_H

#include <queue>

void cleaning_crew_run(std::queue<int> &commandsQueue, bool &done_flag);
void engine_crew_run(std::queue<int> &commandsQueue, bool &done_flag);

void task3_run();

#endif //MODERNCPPCONCURENCYINDEPTH_TASK3_H
