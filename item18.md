# 让接口容易被正确使用，不易被误用 #

想开发一个“容易被正确使用，不容易被误用”的接口，首先必须考虑客户可能做出什么样的错误。比如设计一个日期class构造函数：

```C++
class Date{
public:
    Date(int month , int day , int year);
    ...
};
```

显然不合理，因为客户会错误调用：

- `Date d(30,3,1995);`
- `Date d(2,30,1995);`

这时候类型系统就帮上忙了，导入简单的外覆类型（wrapper types）来区别天数月份，然后于Date构造函数中使用这些类型：

```C++
struct Day(){
explicit Day(int d):val(d){}
int val;
};
struct Month(){
explicit Month(int m):val(m){}
int val;
};
struct Year(){
explicit Year(int y):val(y){}
int val;
};

class Date{
public:
    Date(const Month& m, const Day& d, const Year& y);
    ...
};
```

这样能避免类型错误，一旦正确的类型就位，限制其值有时候就通情达理：

```C++
class Month{
public:
    static Month Jan(){ return Month(1); }
    static Month Feb(){ return Month(2); }
    ...
    static Month Dec(){ return Month(12); }
    
private:
    explicit Month(int m);
    ...
};

Date d(Month::Jan(),Day(30),Year(1995));
```

另一准则：**除非有好理由，否则应该尽量令你的types的行为与内置types保持一致。**

**任何接口如果要求客户必须记得做某些事情，就是有着“不正确使用”的倾向。**

就像之前的一个factory函数：

`Investment* createInvestment();`

所以为了避免用户忘了删除指针或者忘了使用智能指针，应该先发制人：

`std::tr1::shared_ptr<Investment> createInvestment();`

同时还可以指定删除器防范DLL问题，比如可以用来自动接触互斥锁：

```C++
std::tr1::shared_ptr<Investment> createInvestment()
{
    std::tr1::shared_ptr<Investment> retVal(static_cast<Investment*>(0),
                                    getRidOfInvestment);
    retVal = ...;
    return retVal ;
}
```

不过要记住，如果pInv管理的原始指针可以在建立pInv之前先确定下来，那么将原始指针传给pInv构造函数会比将pInv先初始化为null再对它做一次赋值操作为佳。原因见条款26.


