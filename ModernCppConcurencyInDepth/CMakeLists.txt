cmake_minimum_required(VERSION 3.10)
project(ModernCppConcurency)

set(CMAKE_CXX_STANDARD 14)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package (Threads)
add_executable(ModernCppConcurency main.cpp common.h unit1_hometask.cpp unit1_hometask.h unit2_mutex_lock.cpp unit2_mutex_lock.h unit2_deadlock_uniquelock.cpp unit2_deadlock_uniquelock.h unit3_conditionals.cpp unit3_conditionals.h unit3_thread_safe_queue.cpp unit3_thread_safe_queue.h unit3_futures_asyncs.cpp unit3_futures_asyncs.h unit3_package_task.cpp unit3_package_task.h unit3_promises_shared_futures.cpp unit3_promises_shared_futures.h unit4_thread_safe_queue.cpp unit4_thread_safe_queue.h unit4_parallel_quicksort.h unit5_atomic_flag_bool.cpp unit5_atomic_flag_bool.h unit5_atomic_pointers_custom.cpp unit5_atomic_pointers_custom.h unit5_memory_ordering.cpp unit5_memory_ordering.h unit5_memory_order_relaxed.cpp unit5_memory_order_relaxed.h unit5_memory_order_acquire_release.cpp unit5_memory_order_acquire_release.h unit5_memory_order_consume.cpp unit5_memory_order_consume.h unit5_release_sequence.cpp unit5_release_sequence.h unit5_spinlock_mutex.cpp unit5_spinlock_mutex.h unit6_lockfree_stack.h unit6_hazard_pointers.h unit6_lock_free_stack_ref_count.h unit7_thread_pool.h)
target_link_libraries (ModernCppConcurency ${CMAKE_THREAD_LIBS_INIT})
