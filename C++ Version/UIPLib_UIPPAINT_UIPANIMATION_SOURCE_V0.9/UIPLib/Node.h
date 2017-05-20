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

	//ɾ��num���õ��Ľڵ�����������ݹ���ȥ
	//�ͷ��ڴ�
	virtual void RemoveChild(int num) {
		assert(num >= 0 && num < m_arrChildren.GetSize());
		delete m_arrChildren[num];
		m_arrChildren.Remove(num);
	}

	//���룬�����ͷ��ڴ�
	CNode* DetachChild(int num) {
		assert(num >= 0 && num < m_arrChildren.GetSize());
		CNode* child = m_arrChildren[num];
		m_arrChildren.Remove(num);
		child->SetParent(0);
		return child;
	}

	virtual int AddChild(CNode* node) {

		//����Ϊnull�Ļ����˳�
        if(node == NULL)
			return -1;

		//�����Լ������Լ�
		if(node == this)
			return -1;

		//���this��node������˳�
		if(this->IsDescendantOf(node))
			return -1;

		//�����ӵĽڵ��и��ף�˵��������һ��������
		//��Ҫ�ȴ�����һ�������з��룬Ȼ���������
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

	//������Щ����Ϊ���ڵ㸸�ӵ�������

	//�ж�this�ڵ��Ƿ��ǲ����ڵ������
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

//ģ�廯���ڵ�
//���뱻�̳�
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