了解C++默默编写并调用哪些函数
==

如果写下：
`class Empty{};`
则相当于：

```C++
class Empty{
	public:
		Empty(){...}
		Empty(const Empty& rhs){...}
		~Empty(){...}

		Empty& operator=(const Empty& rhs){...}
		//以上这些函数都是public并且inline的
};
```

编译器产生的析构函数是non-virtual，除非这个class的base class自身声明有virtual析构函数。

C++并不允许“让reference改指向不同对象”

如果你打算在一个“内含reference成员”的class内支持赋值操作，必须自定义copy assignment操作符，同样，内含const成员也是一样。


如果某个基类将赋值操作声明为private的，编译器则拒绝为其派生类生成赋值操作符。
