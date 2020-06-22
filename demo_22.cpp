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
 *   std::promise : 异步的线程结果，除了用返回值返回给主线程之外，也可以用promise进行主线程和子线程的通信
 *      1) 其可以实现两个线程之间的数据传递
 *      2) 传递各种的数据类型都可以
 *
 *      步骤: 子线程中: 1）定义传入的promise 2)将子线程要传递的参数set_value到子线程中
 *        主线程: 1) 定义一个promise, 起一个子线程，将此promise传入  2）定义future, 从此promise中取出future,并获取值
 *
 */

using  namespace  std;

/*
 * 定义线程入口函数
 * 1）定义传入的promise 2)将子线程要传递的参数set_value到子线程中
 */

void mythread1(std::promise<int> &res, int calc){

    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);

    res.set_value(calc);

    return;
}


int main() {
    std::cout << "main thread start..." << std::this_thread::get_id() << std::endl;

    /*
     * 1) 定义一个promise, 起一个子线程，将此promise传入  2）待线程执行完成，定义future, 从此promise中取出future,并获取值
     */
    std::promise<int> myproc;
    std::thread t1(mythread1, std::ref(myproc), 100);
    t1.join();

    /*
     * future除了和package绑定之外，也可以和Promise进行绑定，进而获取结果值
     */
    std::future<int> ful = myproc.get_future();
    cout << ful.get() << endl;

    std::cout << "main thread end..." << std::this_thread::get_id() << std::endl;
    return 0;
}
