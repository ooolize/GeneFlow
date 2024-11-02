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

+ 类作用域不允许有全特化和部分特化 是一个缺陷

```c++


```
+ 确保在适当的上下文中使用 static 成员，局部类（函数内部类）中不允许 static 数据成员
```c++
void func() {
    GENE_DEFINE(LocalPoint, double x, double y);  // 错误：局部类不允许静态成员
}
```
+ 非/类型参数包  形参包展开 折叠表达式
```c++
template<typename ...Args> // 有很多类型 叫做Args...
void f(Args... a){ // 此处 Args 是类型参数包，args 是值参数包
  auto p = a+...; // 折叠表达式 递归的简写 a+... 一旦有了折叠表达式 a就代表的是一个参数 而不是参数包了
}
 //（(f(Index)|| ...))  的外部括号是必须的 以明确表达式的展开范围和运算顺序。
```


+ decltype

decltype(auto)：会根据表达式的实际类型和值类别来推导，可以是普通值类型、左值引用类型或右值引用类型。
decltype((auto))：无论表达式实际是左值还是右值，都会将其推导为左值引用类型。
decltype(auto) value() 是14对它的语法糖 auto value()-> decltype(auto)


+ 所以何时绑定的Field中的T&obj？
field保存对象的引用 逐个field进行递归分解和赋值 当所有field都被赋值之后 就绑定成功

+ 打印方法--dump
+ why not Field static
+ 判断是否满足某个概念 
```c++
if constexpr (Addable<T>) 
```
+ at与[]

+ 聚合初始化
```c++
class MyClass {
public:
    int& ref;  // 引用成员
};
int main() {
    int a = 1;
    MyClass obj{a};  // 可以 
     MyClass obj(a); // 错误 
    return 0;
}
```


+ 以下错误
```c++
template <lz::use_concepts::Reflect Reflect, typename F>
static constexpr Result for_each_field(Reflect& obj, F& f) {
  for (std::size_t i = 0; i < Reflect::fields_count; ++i) {
    auto& field = obj.template Field<Reflect, i>;
    if (auto res = f(field); res != Result::SUCCESS) {
      return res;
    }
  }
  return Result::SUCCESS;
}
在运行时动态访问类模板成员（即通过索引 i 动态决定类型）是无法实现的，因为模板实例化是在编译时进行的，不能在运行时依赖变量

需要使用递归展开 or 折叠表达式


```

+ std::invocable<F>

要求F支持无参调用， 如果要指定参数 std::invocable<F, Args...> 

+ 实际上是生成解析的代码
```c++

+ static和constexpr有什么关系 一起使用是什么意思

static 变量不一定是编译期常量(单例)。 static 关键字在 C++ 中的作用主要是控制变量的生命周期和链接属性

类成员中的 static：静态成员属于类本身，而不是类的具体对象。它们在整个程序中只存在一份，与对象实例无关。

局部变量中的 static：静态局部变量在函数内部使用时，它们的生命周期是从第一次调用开始，直到程序结束。它们在函数的多次调用之间保持值不变，只初始化一次。

全局或命名空间作用域中的 static：静态全局变量或静态函数的作用范围仅限于定义它们的编译单元（通常是文件），即它们具有内部链接。
static 和编译期常量的关


+ void_t

用来配合SFINAE 检测某个类型的特性
```c++
template<typename,typename void>
class has_foo: std::false_type {};

template<typename T>
class has_foo<T,std::void_t<decltype(std::declval<T>().foo())>>: std::true_type {};

```

+ std::is_same_v<A, B> std::is_same<A, B>::value 

```