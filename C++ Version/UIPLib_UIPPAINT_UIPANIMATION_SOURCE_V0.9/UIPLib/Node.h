#ifndef __NODE_H
#define __NODE_H
#include "ArrayList.h"

class CNode
{
public:
	CNode() : m_pParent(NULL){
	}

	CNode(CNode* parent,const char* name) :m_pParent(NULL),m_strName(name){
		if(parent)
		{
			parent->AddChild(this);
		}
	}

	virtual ~CNode() {
		for(int i = 0 ; i < m_arrChildren.GetSize(); i++) {
			CNode* child = m_arrChildren[i];
			delete child;
		}
		m_arrChildren.Clear();
	}

	//删除num所得到的节点的所有子孙，会递归下去
	//释放内存
	virtual void RemoveChild(int num) {
		assert(num >= 0 && num < m_arrChildren.GetSize());
		delete m_arrChildren[num];
		m_arrChildren.Remove(num);
	}

	//分离，并不释放内存
	CNode* DetachChild(int num) {
		assert(num >= 0 && num < m_arrChildren.GetSize());
		CNode* child = m_arrChildren[num];
		m_arrChildren.Remove(num);
		child->SetParent(0);
		return child;
	}

	virtual int AddChild(CNode* node) {

		//参数为null的话，退出
        if(node == NULL)
			return -1;

		//不能自己加入自己
		if(node == this)
			return -1;

		//如果this是node的子孙，退出
		if(this->IsDescendantOf(node))
			return -1;

		//如果添加的节点有父亲，说明在另外一颗子树中
		//需要先从另外一颗子树中分离，然后再能添加
		CNode* oldParent = node->m_pParent;
		if(oldParent)
		{
			int idx = oldParent->GetChildIndex(node);
			assert(idx != -1);
			oldParent->DetachChild(idx);
		}

		m_arrChildren.Add(node);
		node->SetParent(this);
		return m_arrChildren.GetSize() - 1;
	}

	const char* GetName() const {
		return m_strName.c_str();
	}

	void SetName(const char* name) {
		m_strName = name;
	}

	//下面这些函数为树节点父子导航操作

	//判断this节点是否是参数节点的子孙
	bool  IsDescendantOf (const CNode* ancestor) const
	{
		if(!ancestor)
			return false;

		for (const CNode* n = m_pParent; n; n = n->m_pParent) {
			if (n == ancestor)
				return true;
		}
		return false;
	}

	CNode* GetParent() const {
		return m_pParent;
	}

	CNode* GetRoot()
	{
		CNode* root = this;
		while(root->m_pParent)
			root = root->m_pParent;
		return root;
	}

	int GetChildCount() const {
		return m_arrChildren.GetSize();
	}

	
	CNode* GetChild(int num) const {
		assert(num >= 0 && num < m_arrChildren.GetSize());
		return m_arrChildren[num];
	}

	int GetChildIndex(const CNode* node)
	{
		for(int i = 0; i < m_arrChildren.GetSize(); ++i)
			if(m_arrChildren[i] == node)
				return int(i);
		return -1;
	}

protected:
	void SetParent(CNode* node) {
		m_pParent = node;
	}

protected:
	std::string m_strName;
	CNode* m_pParent;
	CArrayList<CNode*>  m_arrChildren;
	
};

//模板化树节点
//必须被继承
template <class type> class CTypeNode : public CNode
{
protected:
	CTypeNode() {}
	CTypeNode(CTypeNode<type>* parent,const char* name) : CNode(parent,name){}

public:
	~CTypeNode(){}

public:
    type* GetChild(int num) const
	{
		return (static_cast<type *>(static_cast<CTypeNode<type> *> (CNode::GetChild(num))));
	}

    int GetChildIndex(const CTypeNode<type>* node)
	{
		return CNode::GetChildIndex(node);
	}
    
	type* DetachChild(int num)
	{
		return (static_cast<type *>(static_cast<CTypeNode<type> *>(CNode::DetachChild(num))));
	}

    int AddChild(CTypeNode<type>* node)
	{
		return CNode::AddChild(node);
	}
 
	type* GetParent() const
	{
		return (static_cast<type *>(static_cast<CTypeNode<type> *>(CNode::GetParent())));
	}

	bool  IsDescendantOf (const CTypeNode<type>* ancestor) const
	{
		return CNode::IsDescendantOf(ancestor);
	}

    type* GetRoot()
	{
		return (static_cast<type *>(static_cast<CTypeNode<type> *>(CNode::GetRoot())));
	}
protected:
    void SetParent(CTypeNode<type>* node)
	{
		CNode::SetParent(node);
	}
};


#endif