在资源管理类中提供对原始资源的访问
===

资源管理类很棒，它们是对抗资源泄漏的堡垒。但是还是有许多API直接指涉资源，所以还是得绕过资源管理对象直接访问原始资源。

举个例子，条款13导入一个观念：使用智能指针如`auto_ptr`或`shared_ptr`保存factory函数如createInvestment的调用结果：

`std::tr1::shared_ptr<Investment> pInv(creatInvestment());`

假设你希望以某个函数处理Investment对象，像这样：

`int daysHeld(const Investment* pi); //返回投资天数`

你想要这么调用它：

`int days = daysHeld(pInv);`

却通不过编译，因为daysHeld需要的是`Investment*`指针，你传给它的却是个类型为`std::tr1::shared_ptr<Investment>`的对象。

这时候需要一个函数可将RAII class对象转换为其所内含之原始资源。有两个做法：显式转换和隐式转换。

`auto_ptr`和`shared_ptr`都提供一个get成员函数，用来执行显式转换，也就是它会返回智能指针内部的原始指针：

`int days = daysHeld(pInv.get());`

就像几乎所有智能指针一样，`auto_ptr`和`shared_ptr`也重载了指针取值操作符，它们允许隐式转换至底部原始指针：

```C++
class Investment{
public:
    bool isTaxFree() const ;
    ...
};

Investment* createInvestment();

std::tr1::shared_ptr<Investment> pInv1(createInvestment());
bool taxable1 = !(pInv1->isTaxFree());

std::auto_ptr<Investment> pInv2(createInvestment());
bool taxable2 = !((*pInv2).isTaxFree());

```

看下面的隐式转换和显示转换的例子：

```C++
FontHandle getFont();
void releaseFont(FontHandle fh);

class Font{
public :
    explicit Font(FontHandle fh):f(fh){}
    ~Font(){releaseFont(f);}
private:
    FontHandle f;
};

//提供显示转换：
class Font{
public :
    ...
    FontHandle get() const {return f;}
    ...
};

//隐式转换：
class Font{
public :
    ...
    operator FontHandle() const{return f ;}
    ...
};


```

上述显示转换用起来比较麻烦，每次都要进行get调用，但是隐式调用又容易产生问题，见MoreEffectiveC++条款5。
