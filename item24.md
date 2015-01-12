#若所有参数皆需类型转换，请为此采用non-member函数#

之前说过，令class支持隐式类型转换通常是个糟糕的主意（见MoreEffectoveC++条款五），但是这条规则有例外：在建立数值类型时。

比如建立一个有理数类：

```C++
class Rational{
	public:
		    Rational(int numerator = 0 , int denominator = 1 );
			    int numerator() const ;
				    int denominator() const ;
					    
	private:
					    ...
};
```

比如要实现乘法，肯定首先想到重载*操作符：

```C++
const Rational operator*(const Rational& rhs) const ;
```

但是你肯定不满足，你没法拿它和int相乘，因为：

```C++
Rational oneHalf(1,2);
Rational result = oneHalf*2 ; //正确，因为2隐式转换为Rational类型 
Rational result = 2*oneHalf ;//错误，左边的2可没有重载int类型的*
```

没错，**只有当参数被列于参数列表内，这个参数才是隐式类型转换的合格参与者**。

所以，如果要想支持混合算数运算，只能让2和oneHalf都在参数列表里面，也就是这样：

```C++
const Rational operator*(const Rational& lhs, const Rational& rhs);
```

这样一来，上面的函数就成了non-member函数了，但是不要弄成friend函数，因为如果可以避免friend函数就该避免，正如真实世界一样，朋友带来的麻烦往往多过其价值。
