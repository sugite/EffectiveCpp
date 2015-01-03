复制对象时勿忘其每一个成分
====

设计良好的面向对象系统会将对象的内部封装起来，只留两个函数负责对象拷贝，那便是带着适切名称的`copy`构造函数和`copy assignment`操作符。

之前的条款5曾经说过，编译器会在必要的时候为我们的classes创建`copying`函数，并说明这些"编译器生成版"的行为：将被拷对象的所有成员变量都做一份拷贝。

所以，如果是你自己声明`copying`函数，就会告诉编译器，你将修改缺省实现中的某些行为。

比如一个表现顾客的`class`，我们手工写出`copying`函数：

```C++
void logCall(const std::string& funcName);

class Customer{
	public:
		...
		Customer(const Customer& rhs);
		Customer& operator=(const Customer& rhs);
		...

	private:
		std::string name;
	
};

Customer::Customer(const Customer& rhs):name(rhs.name)
{
	logCall("Customer copy constructor");
}

Customer& Customer::operator=(const Customer& rhs)
{
	logCall("Customer copy assignment operator);
	name = rhs.name;
	return *this;
}
```

到这里每一件事情看起来很好，实际上也确实很好，但随着另一个成员变量加入：

```C++
class Date{ ... };
class Customer{
	public:
		...
	private:
		std::string name;
		Date lastTransaction;
};
```

这时候既有的`copying`函数可就不包含复制Date的行为了，所以结论就是：

**如果你为class添加一个成员变量，那么必须同时修改`copying`函数。如果你忘记了，编译器可不会提醒你。**

这种情况，一旦发生继承，可能会造成此一主题最暗中肆虐的一个潜藏危机。请看：

```C++
class PriorityCustomer: public Customer{
	public:
		...
		PriorityCustomer(const PriorityCustomer& rhs);
		PriorityCustomer& operator=(const PriorityCustomer& rhs);
		...
	private:
		int priority;
};

PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs):priority(rhs.priority)
{
	logCall("PriorityCustomer copy constructor");
}

PriorityCustomer& PriorityCustomer:operator=(const PriorityCustomer& rhs)
{
	logCall("PriorityCustomer copying assignment");
	priority = rhs.priority;
	return *this;
}

```

这时候问题就来了，由于这个类是个派生类，派生类是继承了基类的成员变量副本的，而我们自定义的`copying`函数中并没有对继承而来的基类成员变量进行复制，所以：

* 对于构造函数，派生类的基类成分会被不带参数的构造函数初始化，上例即对name和lastransaction执行缺省的初始化动作。
* 对于=操作符，它不曾企图改变其基类的成员变量，所以那些成员变量保持不变。


如果我们想复制其基类的部分（那些成分往往是private的，我们无法直接访问它们），就应该让派生类的`copying`函数调用相应的基类函数：

```C++
PriorityCustomer::PriorityCustomer(const PriorityCustomer& rhs)
	:priority(rhs.priority),
	Customer(rhs)  //调用base class的 `copy`构造函数
{
	...
}

PriorityCustomer& PriorityCustomer:operator=(const PriorityCustomer& rhs)
{
	Customer::operator=(rhs)  ；  //对base class成分进行赋值动作
	...
}
```

PS：令`copy assignment`操作符调用`copy`构造函数是不合理的，因为这就像试图构造一个已经存在的对象。同样的，令`copy`构造函数调用`copy assignment`操作符同样无意义。

完。
