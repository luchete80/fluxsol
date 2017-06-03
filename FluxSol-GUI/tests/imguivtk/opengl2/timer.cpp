#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

void timer_start(std::function<void(void)> func, unsigned int interval)
{
    std::thread([func, interval]() {
        while (true)
        {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}


void do_something()
{
    std::cout << "I am doing something" << std::endl;
}

int main() {
    timer_start(do_something, 1000);

    while(true);
}