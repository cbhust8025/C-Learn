# 单例模式
> 最简单易实现的一种设计模式，属于创建型模式，顾名思义，就是在实例类的过程中对类的性质进行控制。单例的意思是，只有一个实例，也只能有一个实例。

## 特点：只有一个实例

* 该类不能被复制。
* 该类不能被公开构造。
在C++中，意味着用于单例模式的类的构造函数、拷贝构造函数、赋值函数都不能被公开调用。

## 类别：从构造时机的不同
* 懒汉模式：当需要类的时候构造。构造一个静态局部变量，只初始化一次即第一次调用类实例时，生命周期是等同于程序周期的。适用于多线程。
* 饿汉模式：在main函数外面调用实例的构造，静态实例的初始化，C/C++可以保证在进入main函数之前执行。适用于多线程。