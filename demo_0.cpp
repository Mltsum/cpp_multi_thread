#include <iostream>
#include <thread>


/*
 * 线程id的概念
 * 每个线程的id都是不一样的。通过std::this_thread::get_id()来获取线程id；
 */
void myprint(const int &i, char* buf){
   std::cout << "child thread start..." << std::endl;

    std::cout << "i = " << i << std::endl;
    std::cout << "buf = " << buf << std::endl;
    std::cout << "child thread id = " << std::this_thread::get_id() << std::endl;

    std::cout << "child thread end..." << std::endl;
    return;
}


int main() {
    std::cout << "main thread start..." << std::endl;

    int myvar = 1;
    char buf[] = "this is a test!";
    /*
     * 当使用detach时构造函数，有可能main thread执行完成后，进行子线程的参数的构造和析构。
     * 如用detach，会导致mysecondpar已经释放之后，子线程还会用mysecondpar来进行对象的构造，所以是有问题的。（用构建临时对象来解决）
     */
    std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;
    std::thread child_thread(myprint,myvar,buf);
    child_thread.join();

    std::cout << "main thread end..." << std::endl;
    return 0;
}
