在operator=中处理“自我赋值”
handle assignment to self in operator=.

自我赋值发生在对象被赋值给自己时:
class Widget{...};
Widget w ;
...
w = w ;

假设建立一个class用来保存一个指针指向一块动态分配的位图：
class Bitmap{...};
class Widget{
	...
	private:
		Bitmap *pb;
};

Widget& Widget::operator=(const Widget& rhs)
{
	delete pb ;
	pb = new Bitmap(*rhs.pb);
	return *this;
}
这里就有问题了，因为rhs和this有可能是同一个对象，所以应该是：
Widget& Widget::operator=(const Widget& rhs)
{
	if(this==&rhs) return *this;
	delete pb ;
	pb = new Bitmap(*rhs.pb);
	return *this;
}
但是这样仍然不具备“异常安全性”，因为new Bitmap可能会有异常，导致最后错误。
我们只需要在复制pb所指东西之前别删除pb：
Widget& Widget::operator=(const Widget& rhs)
{
	Bitmap* pOrig = pb ;
	pb = new Bitmap(*rhs.pb);
	delete pOrig;
	return *this;
}

另一个常见的方法是copy and swap ：
class Widget{
...
	void swap(Widget &rhs);
...
};

Widget& Widget::operator=(const Widget& rhs)
{
	Widget temp(rhs);
	swap(temp);
	return *this;
}
