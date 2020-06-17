#include <iostream>
#include <thread>
#include <memory>
#include <vector>

/*
 * 功能：
 * 创建和等待多个线程
 */

using namespace std;

/*
 * 线程入口函数
 */
void myprint(const int &i){
    std::cout << "child thread " << i << " id is = " << std::this_thread::get_id() << std::endl;
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
