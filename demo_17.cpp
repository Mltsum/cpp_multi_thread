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
 *
 */

using  namespace  std;

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
    static MY_CLASS *getInstance(){
        if(m_instance == NULL){
            m_instance = new MY_CLASS();
            static MY_CLASS_CYCLE myClassCycle;
            cout << "myclass is created..." << endl;
        }
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


int main() {
    std::cout << "main thread start..." << std::endl;

    /*
     * 创建一个对象，返回该对象的指针
     * 单例模式实例化方法:1）创建一个类指针 2）采用getInstance()获取类指针
     */
    MY_CLASS *myClass = MY_CLASS::getInstance();
    myClass->func();

    std::cout << "main thread end..." << std::endl;
    return 0;
}
