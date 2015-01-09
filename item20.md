#宁以pass-by-reference-to-const替换pass-by-value#

直接上代码：

```C++
class Person{
	public:
		    Person();
			    virtual ~Person();
				    ...
	private:
						    string name ;
							    string address ;
};

class Student :public Person{
	public:
		    Student();
			    ~Student();
				    ...
	private:
						    string schoolName ;
							    string schoolAddress;
};

bool validateStudent(Student s);
Student plato ;
bool platoIsOK = validateStudent(plato);
```

看看最后一句代码发生了什么，没错，将以plato为蓝本构造s，所以Student类的构造函数会被调用一次，还没完，离开了函数还要析构，还没完，string也要被构造，还有，基类也会被构造一次，所以这里由于validateStudent函数是以值传递的，导致将要执行6次（1+2+1+2）构造函数和6次析构函数。代价太大！！！

所以这里我们要用pass-by-reference-to-const的方式来传参：

`bool validateStudent(const Student& s);`

这样，就不会产生临时对象，就不会导致上面那些事情发生。

##以by reference的方式传参还能避免slicing（对象切割）问题##

**当一个派生类对象以传值的方式传递并被视为一个基类对象，基类的copy构造函数会被调用，而“造成此对象的行为像个派生类对象”的那些特化性质全被切割，仅留下一个基类对象。**


依旧看代码：

```C++
class Window{
	public:
		    ...
				    string name() const;
			    vitual void display() const ;
				    
};

class WindowWithScrollBars: public Window{
	public:
		    ...
				    virtual void display() const ;
};

void printNameAndDisplay(Window w)
{
	    cout<<w.name();
		    w.display();
}

WindowWithScrollBars wwsb ;
printNameAndDisplay(wwsb);

```

这里，参数w会被构造成一个Window对象，所以这里调用的是Window::display,绝不会是WindowWithScrollBars::display。

解决之道就是用const来传参：

```C++
void printNameAndDisplay(const Window& w)
{
	    cout<<w.name();
		    w.display();
}
```


----------


另：

其实reference底层就是指针，而内置类型规模比较小，比如int，而迭代器和函数实际上也是指针，所以对于**内置类型以及STL的迭代器和函数对象**而言，pass-by-value比较适当，其他对象用pass-by-reference-to-const比较合适。


