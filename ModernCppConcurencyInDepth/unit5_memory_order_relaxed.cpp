//
// Created by qazzer on 05/11/2019.
//

#include <atomic>
#include <thread>
#include <cassert>

std::atomic<bool> mor_x, mor_y;
std::atomic<int> mor_z;


/* Despite the fact that y is written before x
 * it can be not visible for other thread.
 * It't because of memory_order_relaxed that
 * doesn't guarantee ordering of mor_y store
 * for other thread
 */
void morelaxed_read_x_then_y()
{
    while(!mor_x.load(std::memory_order_relaxed));

    if(mor_y.load(std::memory_order_relaxed))
    {
        mor_z++;
    }
}

void morelaxed_write_y_then_x()
{
    mor_y.store(true, std::memory_order_relaxed);
    mor_x.store(true, std::memory_order_relaxed);
}

void unit5_memory_order_relaxed()
{
    std::thread thread1(morelaxed_write_y_then_x);
    std::thread thread2(morelaxed_read_x_then_y);

    thread1.join();
    thread1.join();

    assert(mor_z != 0);
}
