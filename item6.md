若不想使用编译器自动生成的函数，就该明确拒绝
===

```C++
class Uncopyable{
	protected:
		Uncopyable(){}
		~Uncopyable(){}
	private:
		Uncopyable(const Uncopyable&);
		Uncopyable& operator=(const Uncopyable&);
};

class HomeForSale:private Uncopyable{
	...
};
```

为驳回编译器自动提供的机能，可将相应的成员函数声明为private并且不予实现
使用像Uncopyable这样的基类也是一种做法
