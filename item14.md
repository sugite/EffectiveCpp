在资源管理类中小心*copying*行为
===

上一条款中，引入了“资源取得时机便是初始化时机”（*Resource Acquisition Is Initialization；*RAII），并以此描述了智能指针和引用计数型智慧指针，但是这两者都是表现在heap-based资源上，然而并非所有的资源都是heap-based，对于那些资源，前面提到的两种指针往往不适合作为资源掌管者。

例如，假设我们使用C API函数处理类型为Mutex的互斥器对象，公有lock和unlock两函数可用：

```C
void lock(Mutex *pm);
void unlock(Mutex *pm);
```

为了确保绝不会忘记将一个被锁住的Mutex解锁，可能会希望建立一个class用来管理。这样的class的基本结构由RAII守则支配，也就是“资源在构造期间获得，在析构期间释放”：

```C++
class Lock{
public:
    explicit Lock(Mutex *pm):mutexPtr(pm)
    {
        lock(mutexPtr);
    }
    
    ~Lock()
    {
        unlock(mutexPtr);
    }
    
private:
    Mutext *mutexPtr;
};
```

这样很好，但是如果Lock对象被复制，会发生什么事：

```C++
Mutex m ;
Lock ml1(&m);
Lock ml2(ml1);
```

当一个RAII对象被复制，会发生什么？大多数会选择以下两种可能：

- **禁止复制**，许多时候RAII对象被复制并不合理，这时候就应该禁止，正如item06所说的：将*copying*操作声明为private：   

```C++
class Lock : private Uncopyable{
public:
    ...
};
```

- **对底层资源祭出引用计数法**，有时候我们希望保有资源，直到它的最后一个使用者被销毁。这种情况复制RAII对象时，应该将资源的被引用数递增，`tr1::shared_ptr`便是如此：

```C++
class Lock{
public:
    explicit Lock(Mutex *pm):mutexPtr(pm,unlock)
    {
        lock(mutexPtr.get()); //get方法见item15
    }
private:
    std::tr1::shared_ptr<Mutex> mutexPtr;
};
```

- **复制底部资源**，也就是深度拷贝。
- **转移底部资源的拥有权**，也就是auto_ptr。

记住：

- 复制RAII对象必须一并复制它所管理的资源，所以资源的*copying*行为决定RAII对象的*copying*行为。
- 普遍而常见的RAII class *copying*行为是：禁止*copying*，施行引用计数法。不过其他行为也都可能被实现。
