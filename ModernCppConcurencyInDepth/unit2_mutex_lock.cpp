//
// Created by qazzer on 23/10/2019.
//

#include <mutex>
#include <list>

std::list<int> simpleList;
std::mutex m;

void pushToList(int x)
{
    simpleList.push_back(x);
    throw std::runtime_error("Example");
}

void wrong_mutex_use()
{
    m.lock();
    try
    {
        pushToList(3);
        m.unlock(); //Need to unlock in all catch scenarios manually
    }
    catch(int a)
    {
        m.unlock(); //Need to unlock in all catch scenarios manually
    }
    catch (std::runtime_error& error)
    {
        m.unlock(); //Need to unlock in all catch scenarios manually
    }

}

void correct_mutex_use()
{
    std::lock_guard<std::mutex> l(m); // Mutex will be unlocked at lock_guard destructor
    try
    {
        pushToList(3);
    }
    catch (std::runtime_error& error)
    {

    }
}
