cmake_minimum_required(VERSION 3.10)
project(ModernCppConcurency)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package (Threads)
add_executable(ModernCppConcurency main.cpp thread_guard.h unit1_hometask.cpp unit1_hometask.h unit2_mutex_lock.cpp unit2_mutex_lock.h unit2_deadlock_uniquelock.cpp unit2_deadlock_uniquelock.h unit3_conditionals.cpp unit3_conditionals.h unit3_thread_safe_queue.cpp unit3_thread_safe_queue.h unit3_futures_asyncs.cpp unit3_futures_asyncs.h unit3_package_task.cpp unit3_package_task.h unit3_promises_shared_futures.cpp unit3_promises_shared_futures.h)
target_link_libraries (ModernCppConcurency ${CMAKE_THREAD_LIBS_INIT})
