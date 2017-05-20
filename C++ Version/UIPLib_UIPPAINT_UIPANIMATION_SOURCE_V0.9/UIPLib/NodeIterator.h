#ifndef __NODE_ITERATOR_H
#define __NODE_ITERATOR_H
#include "Node.h"
#include "ArrayIterator.h"
/*                                       
                                          ���ڵ�(���ģʽ)��������
	                      -------------------------root--------------------
					     /                          |                      \
					  node1                       node2                  node3
                    /       \                    /      \                  |
		       node4         node5           node6       node7           node8
                 |                             |
			   node9                         node10
	 
���:���ϵ��£������ұ���-->
                             [root,node1,node2,node3,node4,node5,node6,node7,node8,node9,node10]
<--�������Ҵӣ��ϵ��´�:�β�


���:���ϵ��£����ҵ������-->
                             [root,node3,node2,node1,node8,node7,node6,node5,node4,node10,node9]
<--�����ҵ���ӣ��ϵ��´�:�β�


���:���ϵ��£������ұ���-->
                             [root,node1,node4,node9,node5,node2,node6,node10,node7,node3,node8]
<--�������Ҵӣ��ϵ��´�:����


���:���ϵ���,���ҵ������-->
                             [root,node3,node8,node2,node7,node6,node10,node1,node5,node4,node9]
<--�����ҵ���ӣ��ϵ��´�:����

�ɴ˿ɼ���
1)���ڵ�����������²��ԣ�
       �������
		       �ȸ�
			       ����
				   ����
			   ���
			       ����
				   ����
	   �������
	           �ȸ�
			       ����
				   ����
			   ���
			       ����
				   ����
2)�����Ĺ�ϵ:
      ��Ȼ���Ⱦ��й̶���
	  �ȸ����ұ�������Ϊ�������
      �ȸ������������Ϊ�������

3)��д�ǵݹ�Ľڵ��������
      ��������������ϵ���ǿ��Է���:
	  1������������Ƚ��ȳ��Ķ���
      2����������Ǻ���ȳ��Ķ�ջ
      3�����еĺ������������ͨ����װ�ȸ���������ʵ��

	  ���������������������Ҫ��д�ĸ��ȸ����������Լ���װ�����ĸ�������������ܼư˸�������

	  ����������ǽ����ҵ��������Էº�����Ϊģ��������������
	  ��ô����ֻ��Ҫ��д�ĸ�������:

	  �������_�ȸ�<���Ҳ��Էº���>������
	  �������_�ȸ�<���Ҳ��Էº���>������
	  �������_���<���Ҳ��Էº���>������װ��
      �������_���<���Ҳ��Էº���>������װ��

      ��һ�����������ǻᷢ�ֹ�����Ⱥ�������ȵ��ȸ�����������ʹ��������ͬΪ�����еĴ��붼һ����
	  �������ʹ��queue���в������������ʹ��stack���в�����

	  �������ʹ��ģ��������뵽�������У������ֿ��Խ��ڵ�����������ٵ�����
     
	  �������ǻᷢ��stl��stack/queue�Ľӿڲ�һ������ô���Ҫʵ��������Ŀ�ģ�
	  ���Ǳ���Ҫ��stack��queue�Ľӿں��������һ�������ǲ�����Ϊģ��������뵽��������

	  ���������Ҫʵ��queue/stack�������������нӿ�ͳһ����

	  �������ǾͿ���ʵ�����ǵ�Ŀ����

	  ��������ֻ��Ҫ��д����������:
      A���ȸ�<Adpter,���Ҳ��Էº���>������
	  B�����<Adpter,���Ҳ��Էº���>������װ��

4)ʹ�õ������ģʽ:
      ������ģʽ
	  ������ģʽ��CStackAdpter��QueueAdpter�ֱ𽫽ӿ�ת���������Լ������ͳһ�Ľӿ�
	  ����ģʽ��  ���һ�����������ǲ��ԣ�������Ȼ��ǹ�����ȱ����ǲ��ԣ�ͨ��ʹ�ò�����Ϊģ�����
	              ������������ʹ��if���ж���䡣
      װ��ģʽ:   ���ǵĺ����������װ��ʹ�����ȸ��������Լ�ArrayIterator�����Ӧ�Ĺ��ܵ�

	  ��ʵ�ϣ���������ʹ����C++��ģ����ƣ����ǲ���Ҫʹ��IIterable��IAdapter�ӿ�
	  �����Ļ������Ƿ��麯�����ã���Ч�����ơ�
	  ����ֻ��Ҫע�͵� :public IIterable�Լ�:public IAdapter�����±���Ϳ�����
	 	   
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


//������ģʽ
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


//CNodeBackwardIterator�ǰ�װforwardInterator��ArrayIterator��Ľ�
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
	//���ǵ�BackwardIterator��һ��װ������װ��ForwardIterator�����Լ�CArrayIterator����
    CNodeForwardIterator<TNode,AdpterIndexer,Adpter> m_decorator;
	CArrayList<TNode*>  m_arrNodes;
    CArrayIterator<TNode*,CGetIndexRL> m_arrIter;
};


#endif