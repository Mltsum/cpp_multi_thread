#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
#include <mutex>
#include <future>

/*
 * 功能：
 *      std::share_future : 解决多线程future.get(其值位转移，第一次转移之后，值为空)取值时的问题。如果用share_future,则为复制
 *
 *
 */

using  namespace  std;

int mythread1(){
    cout << "child thread1 start... " << endl;
    std::chrono::milliseconds dura(5000);
    std::this_thread::sleep_for(dura);

    cout << "child thread1 end... " << endl;
    return 5;
}

/*
 * 线程1的结果返回给子线程2
 * 传入参数更换为shared_future
 */
void mythread2(std::shared_future<int> &tmpf){
    cout << "child thread2 start... " << endl;
    auto result = tmpf.get();
    cout << "thread2 receive result is " << result << endl;

    cout << "child thread2 end... " << endl;
}

int main() {
    std::cout << "main thread start..." << std::this_thread::get_id() << std::endl;

    /*
     * 启动线程1执行完成
     */
    std::packaged_task<int()> mypt(mythread1);
    std::thread t1(std::ref(mypt));
    t1.join();

    /*
     * 从线程1取出feature,送给线程2
     */
    std::future<int> res = mypt.get_future();
    /*
     * 用future_share
     * 可以get多次
     */
    std::shared_future<int> res_s(std::move(res));
    res_s.get();
    res_s.get();
    res_s.get();

    std::thread t2(mythread2,std::ref(res_s));
    t2.join();

    std::cout << "main thread end..." << std::this_thread::get_id() << std::endl;
    return 0;
}
