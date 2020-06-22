#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include <future>

/*
 * 功能：
 *      std::future 其余的用法:
 *          wait_for : 线程等待一段时间，然后取结果
 *
 *
 */

using  namespace  std;

int mythread1(){
    cout << "child thread start... " << endl;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);

    cout << "child thread end... " << endl;
    return 5;
}

int main() {
    std::cout << "main thread start..." << std::this_thread::get_id() << std::endl;

    std::future<int> result = std::async(mythread1);
    cout << "continue ... " << endl;
    /*
     * wait_for 在取用子线程的结果feature时，等待一定的时间
     */
    std::future_status status = result.wait_for(std::chrono::seconds(6));
    if(status == std::future_status::timeout){
        cout << "timeout... " << endl;
    } else if(status == std::future_status::ready){
        cout << "ready... " << endl;
    } else {
        /*
         * deferred : 如果设置了deferred(get才开始执行)， 则此条件成立
         */
        cout << "deferred... " << endl;
    }

    std::cout << "main thread end..." << std::this_thread::get_id() << std::endl;
    return 0;
}
