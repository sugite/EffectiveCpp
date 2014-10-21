为多态基类声明virtual析构函数
Declare destructures virtual in polymorphic base classes.

有很多做法可以记录时间，设计一个TimeKeeper 基类和一些派生类作为不同的计时方法
class TimeKeeper{
	public:
		TimeKeeper();
		~TimeKeeper();
		...
};

class AtomicClock: public TimeKeeper{...};//原子钟
class WaterClock: public TimeKeeper{...}; //水钟
class WristWatch: public TimeKeeper{...}; //腕表

设计一个工厂函数，返回一个基类指针，指向新生成的派生类对象:
TimeKeeper* getTimeKeeper();

将工厂函数返回的每一个对象适当地delete很重要：

TimeKeeper* ptk = getTimeKeeper();

...
delete ptk ;

C++明确指出，当派生类对象经由一个基类指针被删除，而该基类的析构函数不是虚函数
结果将是未定义的，实际上通常是派生成分没被销毁。

所以应该是：
class TimeKeeper{
	public:
		TimeKeeper();
        virtual ~TimeKeeper();
		...
};

当class不企图被当作基类，令其析构函数为virtual往往是个馊主意（虚表）:
class Point{
	public:
		Point(int xc,int yc):x(xc),y(yc){}
		~Point();
	private:
		int x,y ;
};
上述类只占64bit，而如果将析构函数弄成virtual的，那么就要加上虚表所占空间
许多人的心得是：只有当class内含至少一个virtual函数，才将它声明为virtual的

因此不要继承STL容器中的类，因为这些类都不含virtual函数

有时候令class带一个纯虚析构函数，可能更方便。纯虚函数使得类成为抽象类，无法被
实例化：
class AWOV{
	public:
		virtual ~AWOV() = 0;
};
但是注意：必须定义这个纯虚析构函数：

AWOV::~AWOV(){}

因为派生类的析构函数会首先调用基类的析构函数，如果不定义，会报错

