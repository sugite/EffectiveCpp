令operator=返回一个reference to *this
==

```C++
class Widget{
	public:
		...
		Widget& operator=(const Widget& rhs)
		{
			...
			return *this;
		}
		Widget& operator+=(const Widget& rhs)
		{
			...
			return *this;
		}
		Widget& operator=(int rhs)
		{
			...
			return *this;
		}
};
```

