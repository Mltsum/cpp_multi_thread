#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include <future>

/*
 * 功能：
 *
 *   std::packaged_task : 打包任务，把任务包装起来
 *      1) 类模板，模板参数是各种可调用对象。std::packaged_task把各种可调用对象包装起来，方便作为线程入口函数
 *
 *
 */

using  namespace  std;

/*
 * 定义线程入口函数
 */

int mythread(){
    std::cout << "child mythread() start. id is " << std::this_thread::get_id() << std::endl;

    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);

    std::cout << "child mythread() end. id is " << std::this_thread::get_id() << std::endl;

    return 10;
}


int main() {
    std::cout << "main thread start..." << std::this_thread::get_id() << std::endl;

    /*
     * mythread 通过packaged_task包装起来
     * std::ref : std::ref 用于包装按引用传递的值
     */
    std::packaged_task<int()> mypt(mythread);
    std::thread thread1(std::ref(mypt));
    thread1.join();
    std::future<int> res = mypt.get_future();
    cout << res.get() << endl;

    /*
     * 对比不package,而是直接用async创建异步线程
     */
//    std::future<int> res1 = std::async(std::launch::async,mythread);
//    std::cout << res1.get() << std::endl;

    /*
     * package封装之后的对象也可以直接调用
     */
//    std::packaged_task<int()> mypt(mythread);
//    mypt();
//    std::future<int> res = mypt.get_future();
//    cout << res.get() << endl;

    std::cout << "main thread end..." << std::this_thread::get_id() << std::endl;
    return 0;
}
