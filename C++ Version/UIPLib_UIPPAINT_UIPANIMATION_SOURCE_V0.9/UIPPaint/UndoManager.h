#ifndef __UNDO_MANAGER_H
#define __UNDO_MANAGER_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayList.h"

//����ģʽ
class ICommand
{
public:
	virtual ~ICommand(){}

	virtual void Execute() = 0;

	virtual void Undo() = 0;

	virtual bool IsValid() = 0;
	
};

//�ڴ�й¶bug����
//ʹ������ָ��
//ԭ��:ԭ��û��ʹ������ָ�룬���������ICommandָ�����������nMaxʱ��ᵼ���ڴ�й¶
//��˽�ICommand��Ϊ����ָ�����󣬾ͽ�����ڴ�й¶�����⡣
typedef shared_ptr<ICommand> ICommandPtr;

class CUndoManager
{
public:
	CUndoManager(size_t nMax = 25):m_nMaxCount(nMax)
	{

	}

	~CUndoManager()
	{
		m_redoStack.clear();
		m_redoStack.clear();
	}

	void SetMaxCount(int nMax)
	{
		m_nMaxCount = nMax;
	}

	int GetMaxCount() const
	{
		return m_nMaxCount;
	}

	bool CanUndo() const
	{
		return (m_undoStack.size() > 0);
	}

	bool CanRedo() const
	{
		return (m_redoStack.size() > 0);
	}

    void PushCommand(ICommandPtr pCmd,bool bExecute = true)
	{
		if(!pCmd->IsValid())
			return;
     
		//����ջ�������maxCount���������ջ�����������������Ҫ����������������
		while(m_undoStack.size() == m_nMaxCount)
		{
			m_undoStack.pop_front();
		}

		m_undoStack.push_back(pCmd);
		m_redoStack.clear();//���m_redoStack

		if(bExecute)
			pCmd->Execute();

	}

	bool Undo()
	{
		if(m_undoStack.empty())
			return false;

		ICommandPtr pCmd = m_undoStack.back();
		
		m_undoStack.pop_back();

		while(m_redoStack.size() == m_nMaxCount)
		{
			m_redoStack.pop_front();
		}

		m_redoStack.push_back(pCmd);
		pCmd->Undo();
		return (m_undoStack.size() > 0);
	}

	bool Redo()
	{
		if(m_redoStack.empty())
			return false;

		ICommandPtr pCmd = m_redoStack.back();
		m_redoStack.pop_back();

		m_undoStack.push_back(pCmd);
		pCmd->Execute();
		return (m_redoStack.size() > 0);
	}


private:

	//������������undo/redo����
	//���������ƺ���Ҫ��ջ����Ԫ��ɾ��
	//���ʹ��deque�ǳ�����
   deque<ICommandPtr> m_undoStack;
   deque<ICommandPtr> m_redoStack;
   size_t             m_nMaxCount;
};


#endif