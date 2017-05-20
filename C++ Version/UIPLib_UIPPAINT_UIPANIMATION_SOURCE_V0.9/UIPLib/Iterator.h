#ifndef __IITERATOR_H
#define __IITERATOR_H

//�����������ֲ��ԣ� ��ǰ���󣬻��ߴӺ�ǰ
//���������º��������������ֲ���
//ͨ���º����������º�����Ϊģ��������룬���ǿ���ȥ����if�ж�

//ΪʲôҪʹ�÷º����� ��
//1.�º�����һ��ĺ�����

//2.�º���������ʶ�𣬿�����Ϊģ�����(�������Ҫ)��

//3.ִ���ٶ��Ϸº����Ⱥ�����ָ��Ҫ����ġ�

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

//����������ӿڣ���Ҫʵ�����������ӿھͿ��Խ��е���
//����΢��.net���IEnumator�ӿ�����
/*
Iterator iter(containerPointer)//�����ڴ������Ĺ��캯���е���Reset()����
while(iter.MoveNext())
   iter.GetCurrent();

�ظ�����iter�������ٴε���ʱ��Ҫ����Reset()
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