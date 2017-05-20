#ifndef __IITERATOR_H
#define __IITERATOR_H

//遍历都有两种策略： 从前到后，或者从后到前
//定义两个仿函数用于上述两种策略
//通过仿函数，并将仿函数作为模板参数传入，我们可以去除掉if判断

//为什么要使用仿函数呢 ？
//1.仿函数比一般的函数灵活。

//2.仿函数有类型识别，可以作为模板参数(这个最重要)。

//3.执行速度上仿函数比函数和指针要更快的。

struct CGetIndexLR
{
	int operator() (int len,int idx)
	{
		return idx;
	}
};

struct CGetIndexRL
{
	int operator() (int len,int idx)
	{
		return (len - idx -1);
		
	}
};

//定义迭代器接口，这要实现下面三个接口就可以进行迭代
//根据微软.net库的IEnumator接口命名
/*
Iterator iter(containerPointer)//总是在带参数的构造函数中调用Reset()函数
while(iter.MoveNext())
   iter.GetCurrent();

重复利用iter，我们再次迭代时需要调用Reset()
iter.Reset();
while(iter.MoveNext())
   iter.GetCurrent();
*/
template<typename T>
class IIterable
{
public:
	virtual void Reset()         = 0;
	virtual T&   GetCurrent()    = 0;
    virtual bool MoveNext()      = 0;
};

#endif