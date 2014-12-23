别让异常逃离析构函数
==

C++并不禁止析构函数抛出异常，但它不鼓励这样做：

```C++
class Widget{
	public:
		...
		~Widget(){...}   //假设这个可能抛出一个异常
};

void doSomething()
{
	std::vector<Widget> v ; 
	...
}						//v在这里被自动销毁
```

假如v中有十个Widget对象，那么v销毁时会有多个异常同时存在，这将导致不明确行为

但如果析构函数必须执行一个动作，而这个动作失败时会抛出异常，该怎么办？
例如，假设有一个class负责数据库连接：

```C++
class DBConnection{
	public:
		...
		static DBConnection create();

		void close();
};
```

为确保客户不会忘记调用close()，一个方法是创建一个管理DBConnection的类，并在其
析构函数中调用close：

```C++
class DBConn{
	public:
		...
		~DBConn(){
			db.close();
		}
	private:
		DBConnection db;
};
```

只要close成功，正常，但是如果调用导致异常，DBConn析构函数会传播该异常，也就是
允许它离开这个析构函数。
两个办法解决：

```C++
DBConn::~DBConn()
{
	try {db.close();}
	catch(...){
		std::abort();
	}
}
```

如果程序遭遇一个在析构期间发生的错误后无法继续执行，强迫结束程序是合理的。
另一个办法则是不abort，直接吞下。

然而，这两个办法都没什么吸引力。因为两者都无法对“导致close抛出异常”的情况做出反应

重新设计DBConn接口，使其客户有机会对可能出现的问题作出反应:

```C++
class DBConn{
	public:
		void close()
		{
			db.close();
			closed = true;
		}

		~DBConn()
		{
			if(!closed){
				try{db.close();}
				catch(...){
					...
				}
			}
		}
	private:
		bool closed;
		DBConnection db ;
};
```

如果某个操作可能在失败时抛出异常，而又存在某种需要必须处理该异常，那么这个异常必须来自析构函数以外的某个函数


