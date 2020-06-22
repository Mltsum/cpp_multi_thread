#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
/*
 * 互斥量需要引入该头文件
 */
#include <mutex>

/*
 * 功能：
 *  线程池 : 将多个线程放入一起，统一管理, 统一调度;
 *  实现方式: 在程序启动时，一次性创建好一定数量的线程(避免临时创建销毁线程，引起的资源消耗(保存上下文，恢复上下文，线程切换会带来很大的消耗)等，使得程序更加稳定)。
 *
 *  线程创建的数量极限问题:
 *      1)创建线程需要消耗系统资源等。
 *      2)根据经验，2000个线程是极限。
 *
 *  建议 : 1) 创建的线程的数量=CPU数量.  或者创建的线程的数量=CPU数量*2.
 *        2) 创建的线程数和业务强相关。
 *
 */

using namespace std;

/*
 * 以成员函数作为线程的入口函数
 */
class  CLASS_A{
public:
    void inMsgRecvQueue(){
        for (int i = 0; i < 10000; ++i) {
            cout << "inMsgRecvQueue push ..."<<i<< endl;
            {

                std::lock(my_mutex1,my_mutex2);
                msgRecvQueue.push_back(i);
                my_mutex2.unlock();
                my_mutex1.unlock();
            }

        }
    }

    bool outMsgLULProc(int &command){
        std::lock(my_mutex1,my_mutex2);
        if (!msgRecvQueue.empty()) {
            /*
             * 对于容器 c ，表达式 c.front() 等价于 *c.begin() 。
             */
            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            cout << "inMsgRecvQueue pop..." << endl;
            my_mutex1.unlock();
            my_mutex2.unlock();
            return true;
        }
        my_mutex1.unlock();
        my_mutex2.unlock();
        return  false;
    }

    void  outMsgRecvQueue(){
        int command = 0;
        for (int i = 0; i < 10000; ++i) {
            if(outMsgLULProc(command) == true){
                cout << "inMsgRecvQueue pop..." << endl;
            } else {
                cout << "queue is empty " << i << endl;
            }

        }
    }

private:
    list<int> msgRecvQueue;
    /*
     * 实例化一个锁
     * 操作：先lock(),操作共享数据，unlock()
     * lock()和unlock()必须是成对使用的; 其次，一些可能的分支也一定要记得lock()对应unlock;
     */
    std::mutex my_mutex1;
    std::mutex my_mutex2;
};


int main() {
    std::cout << "main thread start..." << std::endl;

    CLASS_A class_a;

    thread child_thread1(&CLASS_A::inMsgRecvQueue, &class_a);
    thread child_thread2(&CLASS_A::outMsgRecvQueue, &class_a);
    child_thread1.join();
    child_thread2.join();

    std::cout << "main thread end..." << std::endl;
    return 0;
}
