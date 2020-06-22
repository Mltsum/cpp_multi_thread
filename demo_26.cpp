#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include <future>

/*
 * 功能：
 *      原子操作: std::atomic， 解决多线程对单个变量操作，导致值错误的问题。
 *
 *
 */

using  namespace  std;


/*
 * 封装了一个类型为Int的值
 * 如果是用int，则用两个线程加出来的值是有问题的。
 */
// int counter = 0;
std::atomic<int> counter = 0;

//std::mutex mutx;
/*
 * 两个线程执行++操作时，会加错。用锁可以解决此问题。
 * 其次的话，可以用atomic原子操作解决:不需要用互斥量加锁技术的多线程并发编程模式。
 * 原子操作可以理解为:多线程中不会被打断的执行片段，且效率会比锁高很多。
 *
 * 互斥量针对一个代码段；而原子操作一般只针对单个变量；
 * std::atomic是用来封装某个变量的
 */
void mythread(){
    for (int i = 0; i < 1000000; ++i) {
//        mutx.lock();
        counter++;
//        mutx.unlock();
    }
}

int main() {
    std::cout << "main thread start..." << std::this_thread::get_id() << std::endl;
    std::thread t1(mythread);
    std::thread t2(mythread);
    t1.join();
    t2.join();
    std::cout << "counter is " << counter << std::endl;

    std::cout << "main thread end..." << std::this_thread::get_id() << std::endl;
    return 0;
}
