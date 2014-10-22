绝不在构造和析构过程中调用虚函数
Never call virtual function during construction or destruction

假如有以下继承体系：
class Transaction{
	public:
		Transaction();
		virtual void logTransaction() const = 0;

		...
};

Transaction::Transaction()
{
	...
	logTransaction();
}

class BuyTransaction:public Transaction{
	public:
		virtual void logTransaction() const ;
		...
};

class SellTransaction:public Transaction{
	public:
		virtual void logTransaction() const ;
		...
};
现在，执行以下语句，会发生什么：
BuyTransaction b ;

显然，基类构造函数执行的时候调用的logTransaction不是派生类的版本。
同样道理也适用于析构函数。一旦派生类析构函数开始执行，对象内的派生类成员变量
便呈现未定义值，所以C++视它们仿佛不存在。进入基类析构函数后对象就成为一个基类
对象，而C++的任何部分包括虚函数等等也就那么看待它。


