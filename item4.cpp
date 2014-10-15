确定对象被使用前已先被初始化

对于无任何成员的内置类型，必须手工完成此事，如：
	int x = 0 ; 
	const char* text = "A C-Style string ";

	double d;
	std::cin>>d ;

至于内置类型以外的任何其他东西，初始化责任落在构造函数身上
规则很简单：确保每一个构造函数都将对象的每一个成员初始化
但是不要混淆来 赋值（Assignment） 和 初始化（Initialization） ：
	class PhoneNumber{...};
	class ABEntry{
	public :
		ABEntry(const string &name,const string &address ,
				const list<PhoneNumber>& phones);
	private :
		string theName ;
		string theAddress;
		list<PhoneNumber> thePhones;
		int numTimesConsulted;
	};

	ABEntry(const string &name,const string &address ,
				const list<PhoneNumber>& phones)
	{
		theName = name ;		//这些都是赋值，不是初始化
		theAddress = address ;
		thePhones = phones ;
		numTimesConsulted = 0;
	}

C++规定，对象的成员变量的初始化动作发生在进入构造函数本体之前。
上述构造函数较佳的写法应该是使用成员初值列：
	ABEntry(const string &name,const string &address ,
				const list<PhoneNumber>& phones)
		:theName(name),theAddress(address),
		thePhones(phones),numTimesConsulted(0)
		{ }
虽然此方法与上一个最终结果相同，但是效率更高。基于赋值的版本首先调用
default构造函数为成员赋初值，然后再对它们赋予新值。

***为免除"跨编译单元之初始化次序"问题，请以local static 对象替换non-local
static 对象。


	
