#ifndef __RESPONSIBLE_CHAIN_H
#define __RESPONSIBLE_CHAIN_H
#include "../UIPLib/ControlBase.h"
#include "../UIPLib/Application.h"
class MyApp;
class CChainBase
{
protected:
	CChainBase() : m_pNextChain(NULL)
	{}

public:
	virtual ~CChainBase()
	{
		if(m_pNextChain)
		{
		  delete m_pNextChain;
		  m_pNextChain = NULL;
		}
	}
	

public:
	virtual void SendToChain() = 0;
	

	CChainBase* AddToChain(CChainBase* pNext)
	{
		m_pNextChain = pNext;
		return m_pNextChain;
	}

	CChainBase* GetChain()
	{
		return m_pNextChain;
	}

	bool HasChain()
	{
		return m_pNextChain != NULL;
	}
	
protected:
	void SendMessageDown()
	{
		if(m_pNextChain)
			m_pNextChain->SendToChain();
	}

protected:
   CChainBase* m_pNextChain;
};

//结束标记类
class CEndChain : public CChainBase
{
public:
   void SendToChain()
   {}
};

class CControlHelpChain : public CChainBase
{

public:

	CControlHelpChain(CControlBase* pCtrl,const wstring& message) : m_pCtrl(pCtrl),m_strMessage(message)
	{
		if(pCtrl)
		{
			pCtrl->KeyEventDispatcher.RegisterListener(CKeyEventListener(&CControlHelpChain::HandleF1KeyEvent));
		}
	}

	void SendToChain()
	{
		if(App()->GetFocusCtrl() == m_pCtrl)
			MessageBox(App()->GetAppHwnd(),m_strMessage.c_str(),TEXT("ControlHelpSystem"),0);
		else
			SendMessageDown();//递归
	}

private:

	//使用静态私有函数来进行监听，因为所有的事件处理都一样的,共享一个回调函数
	//使用静态函数，共享一个函数内存地址，减少内存消耗
	//由此可见事件系统的强大。
	static void HandleF1KeyEvent(CKeyEvent& e);
	
protected:
	CControlBase*        m_pCtrl;
	wstring              m_strMessage;
};


#endif