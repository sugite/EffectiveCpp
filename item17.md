以独立语句将newed对象置入智能指针
===

例子：

```C++
int priority();
void processWidget(std::tr1::shared_ptr<Widget> pw , int priority);
```

调用processWidget：

```C++
processWidget(new Widget , priority());
```

上述无法通过编译，因为tr1::shared_ptr构造函数是个explicit函数，无法进行隐式转换，所以需要这样写：

```C++
processWidget(std::tr1::shared_ptr<Widget>(new Widget) , priority());
```

但是这样写仍然会有内存泄漏的风险，因为在调用processWidget之前，编译器要做三件事：

1. 调用priority
2. 执行new Widget
3. 调用tr1::shared_ptr构造函数

那么，C++编译器以什么样的次序完成这三件事呢？不一定！！！

有可能是先执行2，再执行1，最后执行3，那么，如果在1中抛出异常导致程序终止，不就没办法释放new出来的空间吗？所以我们需要使用分离语句：先创建Widget，把它置入智能指针内，再把智能指针传入processWidget内：

```C++
std::tr1::shared_ptr<Widget> pw(new Widget);
processWidget(pw,priority());
```

这样便不会出现上述问题。所以**以独立语句将newed对象存储于智能指针内，如果不这样做，一旦异常被抛出，有可能导致难以察觉的资源泄漏**
