#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include <future>

/*
 * 功能：
 *   如何线程入口函数有返回值，如何拿到异步线程的返回结果？？？
 *
 *   std::async : 启动一个异步任务(自动创建一个线程，并开始执行对应的线程入口函数)，启动起来一个异步任务之后，返回一个std::future对象(含有线程入口函数返回的结果)
 *   std::future对象 : 提供了访问异步操作结果的机制(可能马上拿不到，但是等待异步完成之后，能够拿到)
 *   希望线程返回结果
 *
 *   std::launch : 向std::async传递一个参数
 *      1. std::launch::deferred : 表示延迟调用，线程入口函数会被延迟到std::future的wait()或者get()才执行
 *
 *
 */

using  namespace  std;

/*
 * 定义线程入口函数
 */

int mythread(){
    std::cout << "child mythread() start. id is " << std::this_thread::get_id() << std::endl;

    std::chrono::milliseconds dura(10000);
    std::this_thread::sleep_for(dura);

    std::cout << "child mythread() end. id is " << std::this_thread::get_id() << std::endl;

    return 10;
}


int main() {
    std::cout << "main thread start..." << std::this_thread::get_id() << std::endl;

    /*
     * 启动一个异步线程，并绑定异步线程和future结果
     * std::launch::deferred : 延迟执行，等待wait或者get才执行线程，deferred其实是一个枚举
     * std::launch::deferred : 延迟调用,其实是在主线程中调用(线程id一致)
     * std::launch::async : 子线程立马开始执行（系统默认）
     */
//    std::future<int> res = std::async(std::launch::deferred,mythread);
    std::future<int> res = std::async(std::launch::async,mythread);

    std::cout << "continue ..."  << std::endl;
    /*
     * get() 会等待异步的线程执行完成
     * get() 只能调用1次，不能调用多次
     * 如果不写get()或者wait, 异步线程也会正常执行完成。
     */
    std::cout << res.get() << std::endl;
    /*
     * wait()会等待线程，但是不会返回结果
     */
//    res.wait();

    std::cout << "main thread end..." << std::this_thread::get_id() << std::endl;
    return 0;
}
