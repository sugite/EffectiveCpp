成对使用new和delete时要采取相同的形式
===

当你使用new，有两件事情发生：

1. 内存被分配出来（通过名为operator new的函数）
2. 针对此内存会有一个（或更多）构造函数被调用

当你使用delete，也有两件事发生：

1. 针对此内存会有一个（或更多）析构函数被调用
2. 内存被释放（通过名为operator delete的函数）

那么，编译器如何知道要删除的那个指针到底指向一个对象还是一个对象数组？没错，只能看你delete时有没有加上中括号！

所以如果你在new表达式中使用[]，必须在相应的delete表达式中也使用[]。反之亦然。

警惕：

```C++
typedef std::string AddressLines[4];

std::string *pal = new AddressLines ;

//那么必须匹配数组形式：
delete[] pal;
delete pal; //未定义行为
```


