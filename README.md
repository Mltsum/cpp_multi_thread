# cpp_multi_thread
## demo_0 : thread id的使用
## demo_1 : 线程之间类对象的传递以及类属性的更改
## demo_3 : 线程之间智能指针的传递及unique_ptr的简单应用
## demo_4 : 线程之间传递类的成员函数的传递
## demo_5 : 创建多个线程
## demo_6 : 多线程的共享数据-只读和读写
## demo_7 : 互斥量mutex lock()和unlock()的应用
## demo_8 : 互斥量mutex lock_guard()的使用
## demo_9 : 死锁的原因以及处理死锁的方式
## demo_10 : std::lock()来消除死锁。
## demo_11 : lock_guard加std::adopt_lock来消除unlock()
## demo_12 : unique_lock()来代替lock_guard()
## demo_13 : std::try_to_lock() 搭配 msgGuard1.owns_lock()(是否拿到锁)，解决线程死等的问题
## demo_14 : std::refer_lock()的应用(初始化一个未lock的锁) 可以对unique出来的对象进行锁操作
## demo_15 : unique_lock所有权以及所有权的转移
## demo_16 : 单例模式的案例
## demo_17 : 单例模式中，手动进行类对象delete的小技巧
## demo_18 : 如何解决多线程在单例模式中，getinstance时，可能冲突的问题？
## demo_19 : 用call_once来解决线程加锁的问题
## demo_20 : 当线程的入口函数有返回值时，主线程如何拿到？