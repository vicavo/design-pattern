#ifndef __TEST_H
#define __TEST_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayIterator.h"
#include "../UIPLib/NodeIterator.h"

typedef CArrayList<int>        CIntArray;
typedef CArrayIterator<int>    CIntIter;
typedef CArrayIterator<int,CGetIndexRL> CIntBackIter;



void TestArrayListIterator()
{
	CIntArray arr;

	for(int i = 0; i < 30; i++)
		arr.Add(i);

	{
		CIntIter iter(&arr);
		while(iter.MoveNext())
			printf("Current Value = %d\n",iter.GetCurrent());
	}

	{
		CIntBackIter iter(&arr);
		while(iter.MoveNext())
			printf("Current Value = %d\n",iter.GetCurrent());
	}

}

CNode* CreateTree()
{
	CNode* root =  new CNode(NULL,"root");
	CNode* node1 = new CNode(root,"node1");
	CNode* node2 = new CNode(root,"node2");
	CNode* node3 = new CNode(root,"node3");

	CNode* node4 = new CNode(node1,"node4");
	CNode* node5 = new CNode(node1,"node5");

	CNode* node6 = new CNode(node2,"node6");
	CNode* node7 = new CNode(node2,"node7");

	CNode* node8 = new CNode(node3,"node8");

	CNode* node9 = new CNode(node4,"node9");
	CNode* node10 = new CNode(node6,"node10");
	
	printf("root = %s\n",node10->GetRoot()->GetName());
	printf("node10 is descendant of node2 = %d\n",node10->IsDescendantOf(node2));

	//�����и��׵Ľڵ�������
	/*
	CNode* node11 = new CNode();
	node11->SetName("node_11");
	node11->AddChild(node4);
	delete node11;
	*/

	return root;
}

//�ȸ�������

//1)������ȣ����ϵ��£������ұ�����
typedef CNodeForwardIterator<CNode> CNFDLRIter;
//2)������ȣ����ϵ��£����ҵ��������     
typedef CNodeForwardIterator<CNode,CGetIndexLR> CNFDRLIter;
//3)������ȣ����ϵ��£������ұ�����
typedef CNodeForwardIterator<CNode,CGetIndexLR,CQueueAdpter<CNode*> > CNFBLRIter;
//4)������ȣ����ϵ��£����ҵ��������
typedef CNodeForwardIterator<CNode,CGetIndexRL,CQueueAdpter<CNode*> > CNFBRLIter;


//���������

//5)������ȣ����µ��ϣ������ұ�����
typedef CNodeBackwardIterator<CNode> CNBDLRIter;
//6)������ȣ����µ��ϣ����ҵ��������
typedef CNodeBackwardIterator<CNode,CGetIndexRL> CNBDRLIter;
//7)������ȣ����µ��ϣ������ұ�����
typedef CNodeBackwardIterator<CNode,CGetIndexRL,CQueueAdpter<CNode*> > CNBBLRIter;
//8)������ȣ����µ��ϣ����ҵ��������
typedef CNodeBackwardIterator<CNode,CGetIndexLR,CQueueAdpter<CNode*> > CNBBRLIter;


template<typename TIter>
void ForEachPrintInfo(TIter& iter)
{
	while(iter.MoveNext())
       printf("Current Node name = %s\n",iter.GetCurrent().GetName());
}


void TestNodeLevelIterator()
{
	shared_ptr<CNode> root(CreateTree());

	printf("������ȣ����ϵ��£������ұ���\n");
	CNFDLRIter fdlrIter(root.get());
	ForEachPrintInfo<CNFDLRIter>(fdlrIter);
	
    printf("������ȣ����ϵ��£����ҵ������\n");
	CNFDRLIter fdrlIter(root.get());
	ForEachPrintInfo<CNFDRLIter>(fdrlIter);
		
	printf("������ȣ����ϵ��£������ұ���\n");
	CNFBLRIter fblrIter(root.get());
    ForEachPrintInfo<CNFBLRIter>(fblrIter);

    printf("������ȣ����ϵ��£����ҵ������\n");
	CNFBRLIter fbrlIter(root.get());
    ForEachPrintInfo<CNFBRLIter>(fbrlIter);

	printf("������ȣ����µ��ϣ������ұ���\n");
	CNBDLRIter bdlrIter(root.get());
    ForEachPrintInfo<CNBDLRIter>(bdlrIter);

	printf("������ȣ����µ��ϣ����ҵ������\n");
	CNBDRLIter bdrlIter(root.get());
    ForEachPrintInfo<CNBDRLIter>(bdrlIter);

	printf("������ȣ����µ��ϣ������ұ���\n");
	CNBBLRIter bblrIter(root.get());
    ForEachPrintInfo<CNBBLRIter>(bblrIter);

	printf("������ȣ����µ��ϣ����ҵ������\n");
	CNBBRLIter bbrlIter(root.get());
    ForEachPrintInfo<CNBBRLIter>(bbrlIter);

}


#endif