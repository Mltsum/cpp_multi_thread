#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <list>
#include <mutex>

/*
 * 设计模式:
 *      1）单例设计模式 : 某个类的对象只能创建一个; 比如配置文件的读写。
 *            1. 单例模式new出的对象如何进行手动释放，而不交由系统尽心释放？ 方法是在构造时在创建一个类中的类。
 *            2. 强烈建议在主线程，创建其他线程之前，实例化单例的类。
 *               如果需要再子线程中实例化对象，则要做一些特殊注意！
 *            3. call_once() : 功能:保证某函数在多线程的情况下只被调用一次。 且具备互斥量的能力，且比互斥量消耗的资源更少。
 *               需要和std::once_flag结合使用。call_once()通过std::once_flag来决定是否执行
 *
 *
 *
 */

using  namespace  std;

/*
 * 设计一个锁，来进行多线程的加锁和解锁
 */
std::mutex my_mutex;

/*
 *  设置一个call_once()的系统标记
 */
std::once_flag o_flag;

/*
 * 单例类
 * 创建方法: 1) 私有构造函数  2) 设置一个私有类指针的属性     3) 创建getInstance函数,使得其返回类指针，在此类析构时，将类手动释放
 */
class MY_CLASS{
private:
    MY_CLASS(){}

private:
    static MY_CLASS *m_instance;

public:
    static void creatInstance(){
        m_instance = new MY_CLASS();
        static MY_CLASS_CYCLE myClassCycle;
    }

    static MY_CLASS *getInstance(){
        /*
         * 1) 多个线程调用该方法时，可能会同时进入 if(m_instance == NULL) 导致同时 new对象。解决该方法是来加锁
         * 2) 通过加锁来解决上述问题，但此代码效率是很低的，原因是很多线程会频繁getinstance进而调用其方法。频繁调用时会严重降低效率。因此再加一个if()
         * 3) if(m_instance == NULL){} 之后会增加代码的效率，所以这种思路时非常重要的！
         */
//        if(m_instance == NULL){
//            std::unique_lock<std::mutex> uniqlck(my_mutex);
//            if(m_instance == NULL){
//                m_instance = new MY_CLASS();
//                static MY_CLASS_CYCLE myClassCycle;
//                cout << "myclass is created..." << endl;
//            }
//        }

        /*
         * 运用call_once来解决多线程加锁的问题
         * o_flag 保证函数只执行一次
         * 使用: 1) 先用std::once_flag声明一个flag  2)定义只执行一次的函数 3) std::call_once(flag, 函数指针)
         */
        std::call_once(o_flag, creatInstance);
        return m_instance;
    }

    class MY_CLASS_CYCLE{
    public:
        ~MY_CLASS_CYCLE(){
            if(MY_CLASS::m_instance){
                delete MY_CLASS::m_instance;
                MY_CLASS::m_instance = NULL;
                cout << "myclass is released..." << endl;
            }
        }
    };

    void func(){
        cout << "test..." << endl;
    }
};

/*
 * 类静态成员变量的初始化
 */

MY_CLASS *MY_CLASS::m_instance = NULL;

/*
 * 定义子线程的入口函数
 */

void childFunction(){
    cout << "child thread start..." << endl;
    /*
     * 多个线程调用此函数，会调用getInstance()
     */
    MY_CLASS *myClass1 = MY_CLASS::getInstance();
    myClass1->func();
    cout << "child thread end..." << endl;
}

int main() {
    std::cout << "main thread start..." << std::endl;

    /*
     * 创建一个对象，返回该对象的指针
     * 单例模式实例化方法:1）创建一个类指针 2）采用getInstance()获取类指针
     */
//    MY_CLASS *myClass = MY_CLASS::getInstance();
//    myClass->func();


    /*
     * 模拟两个线程，同时进入同一个线程入口函数，此入口函数会实例化单例对象
     */
    std::thread thread1(childFunction);
    std::thread thread2(childFunction);
    thread1.join();
    thread2.join();


    std::cout << "main thread end..." << std::endl;
    return 0;
}
