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
 * 线程之间的共享数据
 * 只读：可以设置全局的遍历或者数据结构，用不同的线程可以共享
 * 读写：线程读写时，用线程锁进行共享数据的保护
 *
 * 数据保护：
 * 互斥量：mutex : 互斥量是个类对象，多个线程尝试用lock()来加锁，同一时间只有一个线程能锁成功（成功的标志为函数返回）。如果没所成功，lock()就会不断的尝试lock();
 *                1) 原则是只保护需要保护的代码行。保护多了会影响效率，保护少了达不到想要的效果
 *                2) 谨记 : lock()和unlock()必须对应，在多分支时，也必须对应。
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
            my_mutex.lock();
            msgRecvQueue.push_back(i);
            my_mutex.unlock();
        }
    }

    bool outMsgLULProc(int &command){
        my_mutex.lock();
        if (!msgRecvQueue.empty()) {
            /*
             * 对于容器 c ，表达式 c.front() 等价于 *c.begin() 。
             */
            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            cout << "inMsgRecvQueue pop..." << endl;
            my_mutex.unlock();
            return true;
        }
        my_mutex.unlock();
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

//    void  outMsgRecvQueue(){
//        for (int i = 0; i < 10000; ++i) {
//            my_mutex.lock();
//            if (!msgRecvQueue.empty()) {
//                int command = msgRecvQueue.front();
//                msgRecvQueue.pop_front();
//                cout << "inMsgRecvQueue pop..." << endl;
//                my_mutex.unlock();
//            } else {
//                cout << "queue is empty " << i << endl;
//                my_mutex.unlock();
//            }
//
//        }
//    }
private:
    list<int> msgRecvQueue;
    /*
     * 实例化一个锁
     * 操作：先lock(),操作共享数据，unlock()
     * lock()和unlock()必须是成对使用的; 其次，一些可能的分支也一定要记得lock()对应unlock;
     */
    std::mutex my_mutex;
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
