#ifndef __STATE_MAMAGER_H
#define __STATE_MANAGER_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayList.h"
#include "Shape.h"
class CStateManager;
class CStateBase
{
protected:
	CStateBase(CStateManager* pStateMgr) : m_pStateMgr(pStateMgr)
	{
	}

public:
	virtual ~CStateBase()
	{
		m_pStateMgr = NULL;
	}

	virtual void OnEnter(){}
	virtual void OnLeave(){}

	virtual void MouseDown(const Point& pt){}

	CStateManager* GetStateMgr()
	{
		return m_pStateMgr;
	}

protected:
   CStateManager*   m_pStateMgr;

};

class CRectState : public CStateBase
{
public:
	CRectState(CStateManager* pMgr):CStateBase(pMgr)
	{
	}

	void MouseDown(const Point& pt);

};

class CCircleState : public CStateBase
{
public:
	CCircleState(CStateManager* pMgr):CStateBase(pMgr)
	{
	}

	void MouseDown(const Point& pt);
	
};


class CTriangleState : public CStateBase
{
public:
	CTriangleState(CStateManager* pMgr):CStateBase(pMgr)
	{
	}

	void MouseDown(const Point& pt);
};


class CFillState : public CStateBase
{
public:
	CFillState(CStateManager* pMgr):CStateBase(pMgr)
	{
	}

	void MouseDown(const Point& pt);
	
};

class CStateManager
{
public:

	CStateManager(CFacadeMediator* pMed) : m_pMediator(pMed),m_pCurrentState(NULL)
	{}
	

	~CStateManager()
	{
		for(int i = 0; i < m_arrStates.GetSize(); i++)
		{
			delete m_arrStates[i];
			m_arrStates[i] = NULL;
		}
		m_pCurrentState = NULL;
		m_pMediator     = NULL;
	}

public:

	void SetMediator(CFacadeMediator* pMed)
	{
		m_pMediator = pMed;
	}

	CFacadeMediator* GetMediator()
	{
		return m_pMediator;
	}

	CStateBase* GetCurrentState() const
	{
		return m_pCurrentState;
	}

	 void SetCurrentState(CStateBase* pCurrentState)
	 {
		 assert(pCurrentState);
		 if(m_pCurrentState != pCurrentState)
		 {
			 pCurrentState->OnEnter();
			 if(m_pCurrentState)
				 m_pCurrentState->OnLeave();
			 m_pCurrentState = pCurrentState;
			 
		 }
	 }

	void MouseDown(const Point& pt)
	{
		assert(m_pCurrentState);
		m_pCurrentState->MouseDown(pt);
	}

	int   GetStateCount() const
	{
		return m_arrStates.GetSize();
	}

	int   AddState(CStateBase* pState)
	{
		assert(pState);
		int idx = m_arrStates.IndexOf(pState);
		if(idx == -1)
		{
			if(m_arrStates.Add(pState))
				idx = m_arrStates.GetSize() - 1;
		}

		SetCurrentState(pState);

		return idx;
	}

	int  GetStateIndex(CStateBase* pState)
	{
		return m_arrStates.IndexOf(pState);
	}

	bool  RemoveState(int idx)
	{
		return m_arrStates.Remove(idx);
	}

	CStateBase*  GetState(int idx)
	{
		return m_arrStates[idx];
	}

private:
	CFacadeMediator*          m_pMediator;
	CStateBase*               m_pCurrentState;
	CArrayList<CStateBase*>   m_arrStates;  
};


#endif