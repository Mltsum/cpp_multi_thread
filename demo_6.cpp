#include <iostream>
#include <thread>
#include <memory>
#include <vector>

/*
 * 功能：
 * 线程之间的共享数据
 * 只读：可以设置全局的遍历或者数据结构，用不同的线程可以共享
 */

using namespace std;

/*
 * 设置共享数据(只读)
 * 1）只读的数据是安全可靠的，不需要额外处理。
 * 2) 读写的数据要保证:读的时候不能写，写的时候不能读，线程不能同时读或者同时写；
 * 3) 一般程序中不会用这种全局的数据结构来设置线程间的数据共享，因为会破坏面向对象的思想
 */

vector<int> global_v = {1};

/*
 * 线程入口函数
 */
void myprint(const int &i){
    std::cout << "child thread " << i << " id is = " << std::this_thread::get_id() << "global_v = "<< global_v[0] << std::endl;
}

int main() {
    std::cout << "main thread start..." << std::endl;

    /*
     * 创建多个线程,用一个vector维护
     */
    vector<thread> threads;
    for (auto i = 0; i < 10; i++){
        threads.push_back(thread(myprint, i));
    }

    /*
     * 多个线程的执行顺序和系统的线程调度有关
     */
    for (auto iter = threads.begin(); iter != threads.end() ; iter++) {
        iter->join();
    }

    std::cout << "main thread end..." << std::endl;
    return 0;
}
