#include <iostream>
#include <thread>
/*
 * 类的成员函数作为子线程的入口函数
 */
#include <memory>

/*
 * 定义一个类
 */
class CLASS_A{
public:
    CLASS_A(){};
    ~CLASS_A(){};
    void function(int var){
        std::cout << "class A function operation with var = " << var << std::endl;
    }

public:
    int a_mi;
};


int main() {
    std::cout << "main thread start..." << std::endl;

    CLASS_A class_a;
    /*
     * 如果要传递类的方法到子线程时，传递顺序应为：&函数+对象名+参数
     */
    std::thread child_thread(&CLASS_A::function, class_a, 10);
    child_thread.join();

    std::cout << "main thread end..." << std::endl;
    return 0;
}
