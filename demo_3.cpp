#include <iostream>
#include <thread>
/*
 * 智能指针定义于头文件memory
 */
#include <memory>

#define BUF_LEN 100
/*
 * 传递智能指针作为线程的参数
 * share_ptr允许多个指针指向同一个对象，unique_ptr则“独占”所指对象。同一时刻，只能有一个unique_ptr指向某个内存区域，所以他是禁止拷贝的。
 * 与shared_ptr不同，unique_ptr没有定义类似make_shared的操作，因此只可以使用new来分配内存，并且由于unique_ptr不可拷贝和赋值，初始化unique_ptr必须使用直接初始化的方式。
 */
void myprint(std::unique_ptr<int> pzn){
    std::cout << "child thread start..." << std::endl;

    std::cout << "child thread end..." << std::endl;
    return;
}


int main() {
    std::cout << "main thread start..." << std::endl;

    /*
     * 智能指针std::unique_ptr<int>
     * 智能指针传入时，也是传递引用，需要用std::move()
     * 创建unique_ptr : 要想创建一个unique_ptr，我们需要将一个new 操作符返回的指针传递给unique_ptr的构造函数。
     * 无法进行复制构造和赋值操作
     * 可以进行移动构造和移动赋值操作 : unique_ptr虽然没有支持普通的拷贝和赋值操作，但却提供了一种移动机制来将指针的所有权从一个unique_ptr转移给另一个unique_ptr。如果需要转移所有权，可以使用std::move()函数
     */
    std::unique_ptr<int> my_pointer(new int(100));
    std::thread child_thread(myprint, std::move(my_pointer));
    child_thread.join();

    std::cout << "main thread end..." << std::endl;
    return 0;
}
