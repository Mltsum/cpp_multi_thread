#include <iostream>
#include <thread>


/*
 * 传递类对象线程的参数
 */

class A{
public:
    /*
     * mutable 表示容许在即便包含它的对象被声明为 const 时仍可修改声明为 mutable 的类成员。
     */
    mutable int m_i;
    A(int a):m_i(a){std::cout<<"[A::A(inta)构造函数执行]"<<std::endl;}
    A(const A &a):m_i(a.m_i){std::cout<<"[A(constA&a)拷贝构造函数执行]"<<std::endl;}
    ~A(){std::cout<<"[~A()析构函数执行]"<<std::endl;}
};

/*
 * 经验证，传递类时，const 可加可不加（貌似跟编译器有关系）
 * 在向线程中传递类时，虽然下述代码传递的是引用，但是为了数据安全的考虑，不论是值传递，还是引用传递，都会拷贝一份数据。这样数据更安全，但是导致的结果是，子线程改变不了主线程的类的值。
 * 因此需要std::ref()
 */
void myprint(const A &buf){
    std::cout << "child thread start..." << std::endl;
    /*
     * 观察主线程对象的元素是否会被修改，主要是看主线程和子线程的对象的地址是不是同一个地址
     */
    buf.m_i = 199;


    std::cout << "child thread end..." << std::endl;
    return;
}


int main() {
    std::cout << "main thread start..." << std::endl;

    A class_obj(10);
    std::cout << "main thread 中，此时，类的属性m_i的值为" << class_obj.m_i << std::endl;
    /*
     * std::ref()表示明确告诉编译器传递引用到子线程中，不再传递拷贝。而是传递真的引用。
     */
    std::thread child_thread(myprint, std::ref(class_obj));
    child_thread.join();
    std::cout << "main thread 中，此时，类的属性m_i的值为" << class_obj.m_i << std::endl;

    std::cout << "main thread end..." << std::endl;
    return 0;
}
