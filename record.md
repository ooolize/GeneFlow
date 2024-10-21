<!--
 * @Description: 
 * @Author: lize
 * @Date: 2024-10-18
 * @LastEditors: lize
-->

## 知识点

+ require子句 合取析取  泛型
+ 嵌套require要求连接一个编译期谓词，用于任何接受编译期bool类型的地方

## 问题

+  std::decay<T>::template Field<T, 0> 是什么

std::decay<T> 将类型 T 进行某种“标准化”处理，消除一些类型特性，返回一个更基本的类型。

在 C++ 中，当你在模板的依赖类型中访问成员模板时，必须在调用时加上 template 关键字。否则，编译器会报错，因为它不确定这是一个模板成员。

Field

```c++

+ static和constexpr有什么关系 一起使用是什么意思

static 变量不一定是编译期常量(单例)。 static 关键字在 C++ 中的作用主要是控制变量的生命周期和链接属性

类成员中的 static：静态成员属于类本身，而不是类的具体对象。它们在整个程序中只存在一份，与对象实例无关。

局部变量中的 static：静态局部变量在函数内部使用时，它们的生命周期是从第一次调用开始，直到程序结束。它们在函数的多次调用之间保持值不变，只初始化一次。

全局或命名空间作用域中的 static：静态全局变量或静态函数的作用范围仅限于定义它们的编译单元（通常是文件），即它们具有内部链接。
static 和编译期常量的关


struct Point{
    double x;
    double y;

    static constexpr int field_count = 2;
    template<typename T>
    struct Field<T,0> {
        T& obj; // why
        decltype(auto) value() {return obj.x};
        decltype(auto)  name() const {return "x"};
    };

    template<typename T>
    struct Field<T,1> {
        T& obj; 
        decltype(auto) value() {return obj.y};
        decltype(auto)  name() const {return "y"};
    };
}

```