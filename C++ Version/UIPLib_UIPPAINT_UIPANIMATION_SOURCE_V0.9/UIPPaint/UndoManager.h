#ifndef __UNDO_MANAGER_H
#define __UNDO_MANAGER_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayList.h"

//命令模式
class ICommand
{
public:
	virtual ~ICommand(){}

	virtual void Execute() = 0;

	virtual void Undo() = 0;

	virtual bool IsValid() = 0;
	
};

//内存泄露bug修正
//使用智能指针
//原因:原来没有使用智能指针，当我们添加ICommand指针的数量操作nMax时候会导致内存泄露
//因此将ICommand变为智能指针管理后，就解决了内存泄露的问题。
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
     
		//我们栈最多容纳maxCount个命令，超过栈的最大容量，我们需要将左侧的命令给清楚掉
		while(m_undoStack.size() == m_nMaxCount)
		{
			m_undoStack.pop_front();
		}

		m_undoStack.push_back(pCmd);
		m_redoStack.clear();//清空m_redoStack

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

	//由于限制最多可undo/redo次数
	//当超过限制后，需要将栈顶的元素删除
	//因此使用deque非常合适
   deque<ICommandPtr> m_undoStack;
   deque<ICommandPtr> m_redoStack;
   size_t             m_nMaxCount;
};


#endif