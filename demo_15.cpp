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
 *                3）lockguard的使用：lock_guard的构造函数里执行了mutex::lock()  析构的时候会调用mutex::unlock()
 * 死锁: 1个互斥量1把锁，   死锁必须要至少2把锁才能发生。
 *      1) A执行，先锁金锁，再尝试锁银锁（银锁未锁成功）...
 *      2）上下文切换，线程B锁银锁，锁成功，锁金锁（锁不成功）。
 *
 * 防止死锁的措施 : 1）保证上锁的顺序一致。  2)使用lock_guard
 *
 * std::lock() : 一次性的锁住两个或者两个以上的互斥量;
 *      1)不存在多个锁在多个线程中，因顺序问题导致死锁的原因;
 *      2)如果互斥量中有一个没锁住，lock()会一直等待，直到全部锁锁住。因此可以避免死锁的问题。
 *
 * unique_lock:
 *      1) unique_lock 是一个类模板
 *      2) unique_lock相对于lock_guard灵活很多。但是效率相对较低。
 *
 * std::try_to_lock : 尝试去锁互斥量，如果没有锁定成功，则不会去等，而是立即返回。
 *      1) 此之前不能进行lock() (否则相当于连续调用两次lock()，程序会卡死)
 *      2) 目的在于，当一个线程执行时间比较长，另一个线程也不会死等，可以去处理别的事情
 *
 * std::defer_lock : 初始化了一个没有加锁的mutex.(由一个现有的锁创建)
 *      1) 好处是可以拿unique_lock() 创建的对象，进行加锁和解锁，try_lock, release(对象和锁断开联系)
 *      2) 可以说unique_lock管理着一个mutex (unique_lock的所有权的概念: nique_lock有mutex的所有权)
 *      3) 上述的所有权可以转移(但不能复制哦)

 */

using  namespace  std;

/*
 * 以成员函数作为线程的入口函数
 */
class  CLASS_A{
public:
    void inMsgRecvQueue(){
        for (int i = 0; i < 10000; ++i) {
            cout << "inMsgRecvQueue push ..."<<i<< endl;
            /*
             * lock_guard : lock_guard的构造函数里执行了mutex::lock()  析构的时候会调用mutex::unlock()
             * 使用:类 ->实例化一个对象
             * 因此可以方式忘记使用unlock()
             * 可以使用 { } 去定义锁的范围
             */
            {
//                std::lock_guard<std::mutex> Msgguard(my_mutex);
//                my_mutex1.lock();
//                my_mutex2.lock();
                /*
                 * 用Lock()消除死锁问题之后，unlock()也可能导致问题，因此用lock_guard()消除unlock()
                 * std::adopt_lock 可以使得lock_guard不调用lock()
                 */
//                std::lock(my_mutex1,my_mutex2);
                /*
                 * std::adopt_lock : 表示互斥量已经被lock(), 不需要再进行lock()互斥量了
                 * 替换位unique_lock() :
                 *      1) std::adopt_lock 也是一样的效果
                 */
//                std::lock_guard<std::mutex> msgGuard1(my_mutex1, std::adopt_lock);
//                std::lock_guard<std::mutex> msgGuard2(my_mutex2, std::adopt_lock);
//                std::unique_lock<std::mutex> msgGuard1(my_mutex1, std::adopt_lock);
//                std::unique_lock<std::mutex> msgGuard2(my_mutex2, std::adopt_lock);
                /*
                 * 尝试去拿锁，拿不到也不阻塞，该线程可以接下来处理其他事情。
                 */
//                std::unique_lock<std::mutex> msgGuard1(my_mutex1, std::try_to_lock);
//                if(msgGuard1.owns_lock()){
//                    msgRecvQueue.push_back(i);
//                } else {
//                    cout << "lock none..." << endl;
//                }

                /*
                 *  std::defer_lock : 初始化未lock的锁,调用之后进行加锁
                 */
                std::unique_lock<std::mutex> msgGuard3(my_mutex1, std::defer_lock);
                /*
                 * 所有权的转移
                 * std::move(msgGuard3) 所有权的转移
                 */
                std::unique_lock<std::mutex> msgGuard4(std::move(msgGuard3));
                msgGuard4.lock();       // 相当于给my_mutex1加锁
                msgRecvQueue.push_back(i);
            }

        }
    }

    bool outMsgLULProc(int &command){
        /*
         * 此处,将锁的锁顺序不同线程保持一致，即可放置死锁
         * 或者使用lock_guard
         */
        my_mutex1.lock();
//        my_mutex2.lock();
//        std::lock(my_mutex1,my_mutex2);
        std::unique_lock<std::mutex> msgLock(my_mutex1, std::adopt_lock);

        /*
         * 线程休息2s
         * 延时的用法
         */
//        std::chrono::milliseconds dura(2000);
//        std::this_thread::sleep_for(dura);

        if (!msgRecvQueue.empty()) {
            /*
             * 对于容器 c ，表达式 c.front() 等价于 *c.begin() 。
             */
            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            cout << "inMsgRecvQueue pop..." << endl;
//            my_mutex1.unlock();
            return true;
        }
//        my_mutex1.unlock();
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
