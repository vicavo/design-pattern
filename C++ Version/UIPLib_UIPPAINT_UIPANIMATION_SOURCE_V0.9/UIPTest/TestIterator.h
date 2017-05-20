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

	//测试有父亲的节点添加情况
	/*
	CNode* node11 = new CNode();
	node11->SetName("node_11");
	node11->AddChild(node4);
	delete node11;
	*/

	return root;
}

//先根遍历器

//1)深度优先，从上到下，从左到右遍历器
typedef CNodeForwardIterator<CNode> CNFDLRIter;
//2)深度优先，从上到下，从右到左遍历器     
typedef CNodeForwardIterator<CNode,CGetIndexLR> CNFDRLIter;
//3)广度优先，从上到下，从左到右遍历器
typedef CNodeForwardIterator<CNode,CGetIndexLR,CQueueAdpter<CNode*> > CNFBLRIter;
//4)广度优先，从上到下，从右到左遍历器
typedef CNodeForwardIterator<CNode,CGetIndexRL,CQueueAdpter<CNode*> > CNFBRLIter;


//后根遍历器

//5)深度优先，从下到上，从左到右遍历器
typedef CNodeBackwardIterator<CNode> CNBDLRIter;
//6)深度优先，从下到上，从右到左遍历器
typedef CNodeBackwardIterator<CNode,CGetIndexRL> CNBDRLIter;
//7)广度优先，从下到上，从左到右遍历器
typedef CNodeBackwardIterator<CNode,CGetIndexRL,CQueueAdpter<CNode*> > CNBBLRIter;
//8)广度优先，从下到上，从右到左遍历器
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

	printf("深度优先，从上到下，从左到右遍历\n");
	CNFDLRIter fdlrIter(root.get());
	ForEachPrintInfo<CNFDLRIter>(fdlrIter);
	
    printf("深度优先，从上到下，从右到左遍历\n");
	CNFDRLIter fdrlIter(root.get());
	ForEachPrintInfo<CNFDRLIter>(fdrlIter);
		
	printf("广度优先，从上到下，从左到右遍历\n");
	CNFBLRIter fblrIter(root.get());
    ForEachPrintInfo<CNFBLRIter>(fblrIter);

    printf("广度优先，从上到下，从右到左遍历\n");
	CNFBRLIter fbrlIter(root.get());
    ForEachPrintInfo<CNFBRLIter>(fbrlIter);

	printf("深度优先，从下到上，从左到右遍历\n");
	CNBDLRIter bdlrIter(root.get());
    ForEachPrintInfo<CNBDLRIter>(bdlrIter);

	printf("深度优先，从下到上，从右到左遍历\n");
	CNBDRLIter bdrlIter(root.get());
    ForEachPrintInfo<CNBDRLIter>(bdrlIter);

	printf("广度优先，从下到上，从左到右遍历\n");
	CNBBLRIter bblrIter(root.get());
    ForEachPrintInfo<CNBBLRIter>(bblrIter);

	printf("广度优先，从下到上，从右到左遍历\n");
	CNBBRLIter bbrlIter(root.get());
    ForEachPrintInfo<CNBBRLIter>(bbrlIter);

}


#endif