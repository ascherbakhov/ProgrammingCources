cmake_minimum_required(VERSION 3.10)
project(ModernCppConcurency)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package (Threads)
add_executable(ModernCppConcurency main.cpp thread_guard.h unit1_hometask.cpp unit1_hometask.h unit2_mutex_lock.cpp unit2_mutex_lock.h unit2_deadlock_uniquelock.cpp unit2_deadlock_uniquelock.h unit3_futures_conditionals.cpp unit3_futures_conditionals.h)
target_link_libraries (ModernCppConcurency ${CMAKE_THREAD_LIBS_INIT})
