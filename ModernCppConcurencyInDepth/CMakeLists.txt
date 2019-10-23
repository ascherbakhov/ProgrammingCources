cmake_minimum_required(VERSION 3.10)
project(ModernCppConcurency)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package (Threads)
add_executable(ModernCppConcurency main.cpp thread_guard.h task3.cpp task3.h)
target_link_libraries (ModernCppConcurency ${CMAKE_THREAD_LIBS_INIT})
