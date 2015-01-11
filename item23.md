#宁以non-member、non-friend替换member函数#

想象有个class用来表示网页浏览器：

```C++
class WebBrowser{
	public:
		    ...
				    void clearCache();
			    void clearHistory();
				    void removeCookies();
					    ...
};
```

许多用户会想一次性执行所有动作，因此也提供这个函数：

`void clearEverything(); //调用上面三个函数`

当然这一功能也可以由一个non-member函数调用适当的member函数而提供出来：

```C++
void clearEverything(WebBrowser& wb)
{
	    wb.clearCache();
		    wb.clearHistory();
			    wb.removeCookies();
}
```

那么哪一个比较好呢？？？


面向对象守则要求，数据以及操作数据的那些函数应该被捆绑在一起，这意味着建议member函数是个较好的选择，不幸的是，这个建议不正确。

让我们从封装开始讨论，如果某些东西被封装，它就不再可见，越多的东西被封装，越少的人可以看到它。而越少的人看到它，我们就有越大的弹性去改变它，因为我们的改变仅仅直接影响看到改变的那些人食物。因此，越多东西被封装，我们改变那些东西的能力也就越大，这就是我们首先推崇封装的原因：它使我们能够改变食物而只影响有限客户。

那么如何测量“有多少代码可以看到某一块数据”呢？我们计算能够访问该数据的函数数量，作为一种粗糙的量测。越多的函数可以访问它，数据的封装性就越低。

而很显然non-member访问的数据有限，member函数可以访问包括privete数据在内的所有数据，所以一个non-member函数比member函数更受欢迎。

在C++中，比较自然的做法是让clearBrowser成为一个non-member函数并且位于WebBrowser所在的同一个namespace内：

```C++
namespace WebBrowserStuff{
	    class WebBrowser{};
		    void clearBrowser(WebBrowser& wb);
			    ...
}
```

一个像WebBrowser这样的类会提供大量的便利函数，某些与书签有关，某些与打印有关，还有一些与cookies有关。。。通常大多数客户只对其中的某些感兴趣。分离它们最直接的做法就是将书签相关便利函数声明于一个头文件，将cookie相关的便利函数声明于另一个头文件：

```C++
//头文件webbrowser.h  这个头文件针对class WebBrowser自身
//及WebBrowser核心机能
namespace WebBrowserStuff{
	class WebBrowser{...};
	...
}

//头文件 webbrowserbookmark.h
namespace WebBrowserStuff{
	    ...//与书签有关的便利函数
}

//头文件 webbrowsercookie.h
namespace WebBrowserStuff{
	    ...
}
```

将所有便利函数放在多个头文件内但隶属于同一个命名空间，意味这客户可以轻松拓展这一组便利函数。想想C++标准库，看看是不是也是这样。


