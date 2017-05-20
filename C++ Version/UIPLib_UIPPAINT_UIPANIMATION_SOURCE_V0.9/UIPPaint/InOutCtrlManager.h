#ifndef __IN_OUT_CTRL_MANAGER_H
#define __IN_OUT_CTRL_MANAGER_H

#include "../UIPLib/ArrayList.h"
#include "../UIPControl/UIPIconButton.h"
#include "../UIPControl/UIPPaintArea.h"

class CInOutCtrlManager
{
public:
	CInOutCtrlManager(CPaintArea* pPaintArea) : m_pPaintArea(pPaintArea)
	{
		assert(m_pPaintArea);
	}

	int         AddIconButton(CIconButton* pButton)
	{
		assert(pButton);
		int idx = m_arrButtons.IndexOf(pButton);
		if(idx == -1)
		{
			if(m_arrButtons.Add(pButton))
				idx = m_arrButtons.GetSize() - 1;
		}
		return idx;
	}

	bool       RemoveButton(int idx)
	{
		return m_arrButtons.Remove(idx);
	}

	int        GetButtonCount() const
	{
		return m_arrButtons.GetSize();
	}

	CIconButton*   GetButton(int idx)
	{
		return m_arrButtons[idx];
	}

	CIconButton*   operator[] (int idx)
	{
		return m_arrButtons[idx];
	}

	int            GetButtonIndex(CIconButton* pButton)
	{
		assert(pButton);
		return m_arrButtons.IndexOf(pButton);
	}

	CPaintArea*    GetPaintArea()
	{
		return m_pPaintArea;
	}



private:
	CPaintArea*               m_pPaintArea;//显示输出到的PaintArea控件
	CArrayList<CIconButton*>  m_arrButtons;  //事件输入控件集合
};
#endif