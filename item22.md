#将成员变量声明为private#

##为什么成员变量不该用public修饰##

理由如下：

1. 为保证一致性，如果没眼东西都是函数，用户只需要用就行了，而不需要记住是否得在后面加上小括号。
2. 为实现访问控制：不准访问，读写访问，只读访问，只写访问。
3. 封装。



同样的道理也适用于protected。

**protected并不比public更具有封装性**。
