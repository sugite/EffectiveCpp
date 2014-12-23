尽量用 const、enum、inline 替换 #define
===

比如	`#define ASPECT_RATIO 1.653`
使用常量替换
		`const double AspectRatio = 1.653`
好处是	

* 名称AspectRatio会出现在符号表中，报错检查方便
* 而用宏则只会显示1.653，追踪麻烦

注意的几点： 

1. 定义常量指针的时候，const 得写两次，比如
		`const char* const authorName = "Scott Meyers";`
		在C++ 里则使用string更好：
		`const std::string authorName("Scott Meyers");`
2. class 专属常量。由于 #define 无法限制作用域，所以需要让其成为
		class 的一个成员，同时为确保只有一个实体，得用 static 修饰：

```C++
class GamePlayer{
private:
	static const int NumTurns = 5 ;  //常量声明式，而非定义
	int scores[NumTurns] ;
	...
};
```

		上述常量是 int，char，bool ，属特例，否则应该提供定义式，如下：
		
```C++
class CostEstimate{
private: 
	static const double FudgeFactor ;//位于头文件
	...
};
const double CostEstimate::FudgeFactor = 1.35 ;//位于实现文件
```

		如果编译器不允许有特例存在，可用"the enum hach"补偿做法：
		
```C++
class GamePlayer{
private:
	enum {NumTurns = 5 } ;
	int scores[NumTurns] ;
	...
};
```

**注：取一个const的地址是合法的，但是取enum和#define的地址不合法**

而当用#define来定义如下函数时最好用inline来代替
`#define CALL_WITH_MAX(a,b) f((a)>(b)?(a):(b))`
如果`CALL_WITH_MAX(++a,b);`则会产生问题
应该这样：

```C++
template<typename T>
inline void callWithMax(const T &a ,const T &b)
{
	f( a > b ? a : b );
}
```

总之：

* 对于单纯常量，最好以const对象或enums替换#define
* 对于形似函数的宏，最好改用 inline 函数替换#define



