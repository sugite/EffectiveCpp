以对象管理资源
===

有一基类：

```C++
class Investment{..};
```

程序通过一个工厂函数供应某个特定的Investment对象：

```C++
Investment* createInvestment();
```

现有函数f调用类这个方法：

```C++
void f()
{
    Investment* pInv = createInvestment() ;
    ...
    delete pInv;
}
```

很显然，上述delete语句可能无法正确执行。因为可能在delete之前由于某些原因f已经提前return了，或者说由于某些错误在delete之前抛出类异常，于是，内存泄漏了！

为了确保不发生这种情况，我们的确能谨慎编写程序来避免这一问题，但是这里我们介绍另一种方法：把资源放进对象内，我们便可倚赖C++的“析构函数自动调用机制”确保资源被释放。

许多资源被动态分配于heap内而后被用于单一区块或函数内。它们应该在控制流离开那个区块或函数时被释放。标准程序库提供的`auto_ptr`正是针对这种形势而设计的特制产品。`auto_ptr`是个“类指针对象”，也就是所谓的“智能指针”，其析构函数自动对其所指对象调用delete：

```C++
void f()
{
    std::auto_ptr<Investment> pInv(createInvestment());
    ...
}
```

这个简单的例子示范“以对象管理资源”的两个关键想法：

1. **获得资源后立刻放进管理对象内**
2. **管理对象运用析构函数确保资源被释放**

由于`auto_ptr`被销毁时会自动删除它所指之物，所以一定要注意别让多个智能指针同时指向同一对象。为了预防这一点，智能指针有一个不寻常的性质：**若通过copying函数复制它们，它们会变成null，而复制所得的指针将取得资源的唯一拥有权。**如下。

```C++
std::auto_ptr<Investment> pInv1(createInvestment());
std::auto_ptr<Investment> pInv2(pInv1);//现在pInv1为null,pInv2指向对象。
pInv1 = pInv2;//现在pInv2为null,pInv1指向对象。
```

所以不要在STL容器上用智能指针，因为这些容器要求其元素发挥正常的复制行为。

另一方案是“引用计数型智慧指针”：

```C++
void f()
{
    ...
    std::tr1::shared_ptr<Investment> pInv(createInvestment());
    ...
}
```

与智能指针不同的是，`shared_ptr`的复制行为是正常的，所以它们可被用于STL容器以及其他“`auto_ptr`之非正统复制行为并不适用”的语境上。

**上述两者都在其析构函数内做delete而不是delete[]动作。这意味着在动态分配而得的数组身上使用它们是个馊主意，尽管如此，这么做仍能通过编译：**

```C++
std::auto_ptr<std::string> aps(new std::string[10]);
std::tr1::shared_ptr<int> spi(new int[1024]);
```


