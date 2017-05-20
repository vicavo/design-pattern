#ifndef __NODE_ITERATOR_H
#define __NODE_ITERATOR_H
#include "Node.h"
#include "ArrayIterator.h"
/*                                       
                                          树节点(组合模式)迭代遍历
	                      -------------------------root--------------------
					     /                          |                      \
					  node1                       node2                  node3
                    /       \                    /      \                  |
		       node4         node5           node6       node7           node8
                 |                             |
			   node9                         node10
	 
层次:从上到下，从左到右遍历-->
                             [root,node1,node2,node3,node4,node5,node6,node7,node8,node9,node10]
<--历遍左到右从，上到下从:次层


层次:从上到下，从右到左遍历-->
                             [root,node3,node2,node1,node8,node7,node6,node5,node4,node10,node9]
<--历遍右到左从，上到下从:次层


深度:从上到下，从左到右遍历-->
                             [root,node1,node4,node9,node5,node2,node6,node10,node7,node3,node8]
<--历遍左到右从，上到下从:度深


深度:从上到下,从右到左遍历-->
                             [root,node3,node8,node2,node7,node6,node10,node1,node5,node4,node9]
<--历遍右到左从，上到下从:度深

由此可见：
1)树节点遍历具有如下策略：
       广度优先
		       先根
			       左右
				   右左
			   后根
			       左右
				   右左
	   深度优先
	           先根
			       左右
				   右左
			   后根
			       左右
				   右左
2)遍历的关系:
      广度或深度具有固定性
	  先根左右遍历的逆为后根右左
      先根右左遍历的逆为后根左右

3)编写非递归的节点迭代器：
      根据上述遍历关系我们可以发现:
	  1、广度优先是先进先出的队列
      2、深度优先是后进先出的堆栈
      3、所有的后根迭代器可以通过包装先根迭代器来实现

	  如果按照上述分析，我们要编写四个先根迭代器，以及包装另外四个后根迭代器，总计八个迭代器

	  但是如果我们将左右迭代策略以仿函数作为模板参数传入迭代器
	  那么我们只需要编写四个迭代器:

	  广度优先_先根<左右策略仿函数>迭代器
	  深度优先_先根<左右策略仿函数>迭代器
	  广度优先_后根<左右策略仿函数>迭代包装器
      深度优先_后根<左右策略仿函数>迭代包装器

      进一步分析，我们会发现广度优先和深度优先的先根遍历，除了使用容器不同为，所有的代码都一样。
	  广度优先使用queue进行操作，深度优先使用stack进行操作。

	  如果我们使用模板参数传入到迭代器中，我们又可以将节点迭代操作减少到两个
     
	  但是我们会发现stl的stack/queue的接口不一样，那么如果要实现上述的目的，
	  我们必须要将stack和queue的接口函数适配成一样，我们才能作为模板参数传入到迭代器中

	  因此我们需要实现queue/stack适配器，将所有接口统一起来

	  这样我们就可以实现我们的目标了

	  最终我们只需要编写两个迭代器:
      A、先根<Adpter,左右策略仿函数>迭代器
	  B、后根<Adpter,左右策略仿函数>迭代包装器

4)使用到的设计模式:
      迭代器模式
	  适配器模式：CStackAdpter和QueueAdpter分别将接口转换成我们自己定义的统一的接口
	  策略模式：  左右还是右左遍历是策略，深度优先还是广度优先遍历是策略，通过使用策略作为模板参数
	              我们消除掉了使用if等判断语句。
      装饰模式:   我们的后根迭代器是装饰使用了先根迭代器以及ArrayIterator获得相应的功能的

	  事实上，由于我们使用了C++的模板机制，我们不需要使用IIterable和IAdapter接口
	  这样的话，都是非虚函数调用，有效率优势。
	  我们只需要注释掉 :public IIterable以及:public IAdapter并重新编译就可以了
	 	   
*/


template<typename T>
class IAdapter
{
public:
	virtual   void      Add(T t)             = 0;
	virtual   T&        Remove()             = 0;
	virtual   size_t    Size()    const      = 0;
	virtual   bool      IsEmpty() const      = 0;
};


//适配器模式
template<typename T> 
class CStackAdpter : public IAdapter<T>
{
public:
	void Add(T t)
	{
		m_stack.push(t);
	}

	T&   Remove()
	{
		T& t = m_stack.top();
		m_stack.pop();
		return t;
	}

	size_t  Size() const
	{
		return m_stack.size();
	}

	bool IsEmpty() const
	{
		return m_stack.empty();
	}

private:
	stack<T> m_stack;
};

template<typename T> 
class CQueueAdpter : public IAdapter<T>
{
public:
	void Add(T t)
	{
		m_queue.push(t);
	}

	T&   Remove()
	{
		T& t = m_queue.front();
		m_queue.pop();
		return t;
	}

	size_t  Size() const
	{
		return m_queue.size();
	}

	bool IsEmpty() const
	{
		return m_queue.empty();
	}
private:
	queue<T> m_queue;
};


template<typename TNode,
         typename Indexer = CGetIndexRL,
         typename Adpter  = CStackAdpter<TNode*> >
class CNodeForwardIterator : public IIterable<TNode>
{
public:
	CNodeForwardIterator() : m_pNode(NULL),m_pCurrent(NULL)
	{}

    CNodeForwardIterator(TNode* pNode)
	{
		assert(pNode);
        m_pNode    = pNode;
		Reset();
	}

	bool MoveNext()
	{
		if(m_apt.IsEmpty())
			return false;

		m_pCurrent = m_apt.Remove();
        int len =  m_pCurrent->GetChildCount();

		for(int i = 0; i < len; i++)
			m_apt.Add(m_pCurrent->GetChild(indexer(len,i)));

		return true;
	}

	TNode&  GetCurrent()
	{
		return (*m_pCurrent);
	}

	void Reset()
	{
		m_pCurrent = NULL;
        m_apt.Add(m_pNode);
	}

    void SetContainer(TNode* pNode)
	{
		assert(pNode);
        m_pNode    = pNode;
		Reset();
	}

	TNode* GetContainer()
	{
		return m_pNode;
	}

private:
	TNode*           m_pNode;
	TNode*           m_pCurrent;
    Adpter           m_apt;
    Indexer          indexer;
};


//CNodeBackwardIterator是包装forwardInterator和ArrayIterator后的结
template<typename TNode,
         typename AdpterIndexer = CGetIndexLR,
         typename Adpter = CStackAdpter<TNode*> >
class CNodeBackwardIterator : public IIterable<TNode>
{
public:
    CNodeBackwardIterator(TNode* pNode)
	{
		assert(pNode);
		m_decorator.SetContainer(pNode);
		m_arrIter.SetContainer(&m_arrNodes);
		Reset();
	}
	bool MoveNext()
	{
		return m_arrIter.MoveNext();
	}
	TNode&  GetCurrent()
	{
		return (*m_arrIter.GetCurrent());
	}
	void Reset()
	{
		m_arrNodes.Clear();
        while(m_decorator.MoveNext())
			m_arrNodes.Add(&m_decorator.GetCurrent());

		m_arrIter.Reset();
	}
private:
	//我们的BackwardIterator是一个装饰器，装饰ForwardIterator类型以及CArrayIterator类型
    CNodeForwardIterator<TNode,AdpterIndexer,Adpter> m_decorator;
	CArrayList<TNode*>  m_arrNodes;
    CArrayIterator<TNode*,CGetIndexRL> m_arrIter;
};


#endif