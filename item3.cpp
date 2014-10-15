尽可能使用 const

char greeting[] = "Hello" ;
char* p = greeting ;			//non-const pointer ,non-const data
char* const p = greeting ;		//const pointer , non-const data 
const char* p = greeting ;		//non-const pointer , const data 
const char* const p = greeting ;//const pointer , const data 
综上：
	如果const出现在星号左边，则被指物是常量；
	如果出现在星号右边，则指针自身是常量。

迭代器中的 const 说明：
	std::vector<int> vec ;
	...
	const std::vector<int>::iterator iter = //iter的作用像个T* const
		vec.begin();
	*iter = 10 ;							//没问题
	++iter ;								//错误，iter是const
	std::vector<int>::const_iterator cIter=
		vec.begin();						//cIter的作用像个const T*
	*cIter = 10 ;							//错误
	++cIter ;								//没问题

函数中的const：
	令函数返回一个常量值，往往可以降低因客户错误而造成的意外，比如：
	class Rational {...};
	const Rational operator*(const Rational &lhs,const Rational &rhs);
	Rational a,b,c;
	假如程序员失误写成  if(a*b=c) (本来是想作比较，"=="写成了"=")，便
	可预防这种情况。

const 成员函数
	如果两个函数只是常量性不同，则可以被重载:
	class TextBlock{
	public:
		...
		const char& operator[](std::size_t pos) const
		{return text[pos];}
		char& operator[](std::size_t pos)
		{return text[pos];}
	private:
		std::string text;
	};

	TextBlock tb("Hello");
	std::cout << tb[0];			//调用的是non-const []
	const TextBlock ctb("Hello");
	std::cout << ctb[0];		//调用的是const []

****想在 const 成员函数中修改非静态成员，需要加 mutable 关键字。

在 const 和 non-const 成员函数中避免重复（避免维护、编译、代码体积代价）：
	class TextBlock{
	public :
		...
		const char& operator[](std::size_t pos) const
		{
			...
			...
			...
			return text[pos];
		}

		char& operator[](std::size_t pos)
		{
			return const_cast<char&>(
					static_cast<const TextBlock&>(*this)
						[pos]
					);
		}
		...
	};
	上述通过强制转换的方法不能反过来，因为在 const 不能改变对象的逻辑状态



