#include <iostream>
#include <thread>
#include <chrono>
#include <condition_variable>
#include "catch.hpp"

constexpr auto ONE_HUNDRED_MILLIS = std::chrono::milliseconds(100);
constexpr auto ONE_SECOND = std::chrono::seconds(1);

void foo (int a, int b, int c)
{
    std::cout << "Hello, foo(" << a << ", " << b << ", " << c << ")\n";
}

TEST_CASE ("Thread", "thread") {
    std::condition_variable cv;
    std::mutex m;

    std::thread t1{[] () {
        auto timeout = ONE_HUNDRED_MILLIS + std::chrono::duration<int, std::ratio<200, 2>>{};
        std::this_thread::sleep_for(timeout);
        std::cout << "Hello, thread1\n";
    }};

    std::thread t2{std::move(t1)};

    std::thread t3{ [] () {
        auto timeout = ONE_HUNDRED_MILLIS;
        std::cout << "Hello, thread3 ...\n";
        std::this_thread::sleep_for(timeout);
        std::cout << "... thread3\n";
    } };

    std::thread t4 { [& cv, & m] () {
        auto timeout = ONE_SECOND;
        std::this_thread::sleep_for(timeout);
        cv.notify_one();
    } };

    std::thread t5 { [& cv, & m] () {
        std::cout << "Hello, thread5 ...\n";
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock);
        std::cout << "... thread5\n";
    } };

    std::thread t6(std::bind(foo, 1, 2, 3));

    //t1.join(); // t1 moved to t2
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();
}

