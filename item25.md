#考虑写出一个不抛异常的swap函数#

std::swap（）是个很有用的函数，它可以用来交换两个变量的值，包括用户自定义的类型，只要类型支持copying操作，尤其是在STL中使用的很多，例如：

```C++
int main(int argc, _TCHAR* argv[])  
{  
    int a[10] = {1,2,3,4,5,6,7,8,9,10};  
    vector<int> vec1(a, a + 4);  
    vector<int> vec2(a + 5, a + 10);  
  
    swap(vec1, vec2);  
  
    for (int i = 0; i < vec1.size(); i++)  
    {  
        cout<<vec1[i]<<" ";  
    }  
    return 0;  
}  
```
 
上面这个例子实现的是两个vector的内容的交换，有了swap函数，省去了很多的麻烦！

##一、swap的原理##
 
缺省的swap的原理其实很简单，就是将两对象的值彼此赋予对方，其实现过程大致如下：

 
```C++
namespace std{  
    template<typename T>  
    void swap(T&a, T& b){  
        T temp(a);  
        a=b;  
        b=temp;  
    }  
} 
```
 
swap的实现是通过被交换类型的copy构造函数和赋值操作符重载实现的，会涉及到三个对象的复制。所以说，要对自定义的类型调用swap实现交换，必须首先保证自定义类型的copy构造函数和赋值操作符重载函数。

##二、swap的缺陷##
 

缺省的swap最主要的问题就是：**当对象内部包含指针成员时，它不仅要复制3三次被交换的对象，还要复制3次对象成员，而且复制的是指针对象所指向的内容！**例如：

```C++
#include <iostream>  
#include <vector>  
  
using namespace std;  
  
class Point{  
private:  
    int x,y;  
public:  
    Point():x(0),y(0){};  
    Point(int a, int b):x(a),y(b){};  
    void Print(){  
        cout<< x << " "<< y <<endl;  
    }  
    int GetX(){  
        return x;  
    }  
    int GetY(){  
        return y;  
    }  
};  
  
class Line{  
private:  
    Point *px, *py;  
public:  
    Line():px(),py(){};  
    Line(int a,int b,int c,int d):px(new Point(a,b)), py(new Point(c, d)){};  
    Line(const Line& li){  
        px = new Point(*li.px);  
        py = new Point(*li.py);  
    }  
    Line& operator=(const Line& li){  
        Point *p = px;  
        px = new Point(*li.px);  
        delete p;  
        p = py;  
        py = new Point(*li.py);  
        delete p;  
        return *this;  
    }  
    void Print(){  
        cout<<"( "<<px->GetX()<<" "<<px->GetY()<<" "<<py->GetX()<<" "<<py->GetY()<<" )"<<endl;  
    }  
};  
  
int main(int argc, _TCHAR* argv[])  
{  
    Line l1(1,1,2,2), l2(3,3,4,4);  
    swap(l1, l2);  
    l1.Print();  
    return 0;  
}  
```
 
一旦要置换两个Line对象值，swap需要复制三个Line，还要复制六个Point对象，详细可以看赋值运算符重载函数，这样是非常低效的，尤其是当Line的数据成员非常庞大的时候，实际上我们只需要交换各自成员的指针就可以了！

##三、swap的改进方案##

我们希望告诉std::swap：当Line被置换时，真正该做的是置换骑内部的px和py指针。实现这个过程有几个方案，我们先看最简单的方案：

###方案一：将std::swap针对Line特化###
 
C++规定：通常不允许改变std命名空间内的任何东西，但是可以为标准template（如swap）制造特化版本，使他专属于我们自己的class（例如Line）。根据这个性质，我们可以对std:swap针对Line进行特化。我们可以这样特化swap：

```C++
namespace std{  
    template<>  
    void swap<Line>(Line& l1, Line& l2){
    //std::swap()的特化版本，std::swap()只可以特化，不可以重载  
        cout<<"swap of std is called......"<<endl;  
        l1.swap(l2);  
    }
} 
```

在这个代码中，“template<>”表示它是std::swap的一个全特化版本，函数名之后的`<Line>`表示这一特化版本针对”T是Line”而设计。

完整的方案如下：

```C++
#include "stdafx.h"  
#include <iostream>  
#include <vector>  
  
using namespace std;  
  
class Point{  
private:  
    int x,y;  
public:  
    Point():x(0),y(0){};  
    Point(int a, int b):x(a),y(b){};  
    void Print(){  
        cout<< x << " "<< y <<endl;  
    }  
    int GetX(){  
        return x;  
    }  
    int GetY(){  
        return y;  
    }  
};  
  
class Line{  
private:  
    Point *px, *py;  
public:  
    Line():px(),py(){};  
    Line(int a,int b,int c,int d):px(new Point(a,b)), py(new Point(c, d)){};  
    void swap(Line& l){  //Line成员函数，用以实现指针成员交换  
        cout<<"swap of Line is called......"<<endl;  
        using std::swap;  
        swap(px, l.px);  // 交换指针  
        swap(py, l.py);  
    }  
  
    void Print(){  
        cout<<"( "<<px->GetX()<<" "<<px->GetY()<<" "<<py->GetX()<<" "<<py->GetY()<<" )"<<endl;  
    }  
};  
  
namespace std{  
    template<>  
    void swap<Line>(Line& l1, Line& l2){      
    // std::swap()的特化版本，std::swap()只可以特化，不可以重载  
        cout<<"swap of std is called......"<<endl;  
        l1.swap(l2);  
    }  
}  
  
int main(int argc, _TCHAR* argv[])  
{  
    Line l1(1,1,2,2), l2(3,3,4,4);  
    swap(l1, l2);  
    l1.Print();  
    return 0;  
}  
```

在这个例子中，一共出现了5次swap这个函数：

1. 第一次是main中调用的swap，这个调用的是我们自定义的std::swap()的特化版本

2. 第二次是我们自己定义的std::swap()对Line类型的特化，在函数名前面有“template<>”

3. 第三次是对Line特化的swap中调用的swap，也就是l1.swap(l2)，这个很明显是调用Line类型的swap()成员函数

4. 第四次是Line类型中的成员函数，void swap(Line& l)，这个public函数的目的是供给非Line成员函数调用的，也就是特化版本的swap，因为只有类的成员函数才可以调用类的private成员变量

5. 第五次是Line成员函数swap调用的swap，这个swap调用前面有个using std::swap的声明，表示后面调用的是std中的原始swap，当然不是特化版本的swap

其中可以被调用的swap有3个，std中原始的swap、std::swap的特化版本、Line中的成员函数swap，这3个函数中，真正给用户调用的只有第一个swap，也就是std::swap的Line特化版。通过这一系列函数就可以实现Line对象中指针成员的指针的交换，而不是Line对象整个的交换。

这种方式和STL容器有一致性，因为所有的STL容器也都提供有public swap成员函数和std::swap特化版本（用以调用前者）

        

###方案二、重载特化的std::swap###
 
上面这种方式是针对Line和Point都是非template class，现在假设Line和Point都是template class，那么这种方式还可不可以了？

假设Line类和Point类都是template class，如下定义：

```C++
#include <iostream>  
#include <string.h>  
  
template<typename T>  
class Point{  
private:  
     T x,y;  
public:  
     Point():x(0),y(0){};  
     Point(T a, T b):x(a),y(b){};  
     void Print(){  
          std::cout<< x << " "<< y <<endl;  
     }  
     T GetX(){  
          return x;  
     }  
     T GetY(){  
          return y;  
     }  
};  
  
template<typename T1, typename T2>  
class Line{  
private:  
     T1 px,py;  
public:  
     Line():px(),py(){};  
     Line(T2 a,T2 b,T2 c,T2 d):px(T1(a, b)), py(T1(c, d)){};  
     void Print(){  
          std::cout<<"( "<<px.GetX()<<" "<<px.GetY()<<" "<<py.GetX()<<" "<<py.GetY()<<" )"<<std::endl;  
     }  
     void swap(Line<T1, T2>& l){  
          std::cout<<"swap of Line is called......"<<std::endl;  
          using std::swap;  
          swap(px, l.px);  
          swap(py, l.py);  
     }  
};  
  
namespace std {  
    template<typename t1="" typename="" t2="">  
    void swap(Line<t1 t2="">& l1, Line<t1 t2="">& l2){  
        cout<<"swap of std is called......"<<std::endl;  
        l1.swap(l2);  
    }  
}  

int main() 
{  
    Line<Point<double>, double> l1(1, 1, 2, 2), l2(3, 3, 4, 4);  
    std::swap(l1, l2);  
    l1.Print();  
    return 0;  
}  
```

**其中std里面的swap函数就是对std::swap的一个重载版本，然而，这个方式并不是特别的推荐，按照《Effective C++》中的说法，这是一种非法的方式，是被C++标准禁止的，虽然能够编译和运行通过。**

###方案三、非特化非重载的non-member swap###

我们可以声明一个非Line类成员函数swap，让其调用Line的成员函数swap，这个非成员swap也非特化的std::swap，如下所示：

```C++
#include "stdafx.h"  
#include <iostream>  
#include <string.h>  
  
template<typename T>  
class Point{  
private:  
    T x,y;  
public:  
    Point():x(0),y(0){};  
    Point(T a, T b):x(a),y(b){};  
    void Print(){  
        std::cout<< x << " "<< y <<endl;  
    }  
    T GetX(){  
        return x;  
    }  
    T GetY(){  
        return y;  
    }  
};  
  
template<typename T1, typename T2>  
class Line{  
private:  
    T1 px,py;  
public:  
    Line():px(),py(){};  
    Line(T2 a,T2 b,T2 c,T2 d):px(T1(a, b)), py(T1(c, d)){};  
    void Print(){  
        std::cout<<"( "<<px.GetX()<<" "<<px.GetY()<<" "<<py.GetX()<<" "<<py.GetY()<<" )"<<std::endl;  
    }  
    void swap(Line<T1, T2>& l){  
        std::cout<<"swap of Line is called......"<<std::endl;  
        using std::swap;  
        swap(px, l.px);  
        swap(py, l.py);  
    }  
};  
  
template<typename T1, typename T2>  
void swap(Line<T1, T2>& a, Line<T1, T2>& b){  
    std::cout<<"swap of non-member is called......"<<std::endl;  
    a.swap(b);  
}  
  
int main()  
{  
    Line<Point<double>, double> l1(1, 1, 2, 2), l2(3, 3, 4, 4);  
    std::swap(l1, l2);  
    l1.Print();  
  
    return 0;  
  
}  
```

其实就是将方案二的std::swap重载改成了自定义的非成员函数，原理依然一样！

remember：

1. 当std::swap对你的类型效率不高时，提供一个swap成员函数，并确定这个函数不抛出异常。
2. 如果你提供一个member swap，也该提供一个non-member swap用来调用前者。对于class（而非template），也请特化std::swap。
3. 调用swap时应针对std::swap使用using声明式，然后调用swap并且不带任何“命名空间资格修饰符”。
4. 为“用户定义类型”进行std template全特化是好的，但千万不要尝试在std内加入某些对std而言全新的东西。

