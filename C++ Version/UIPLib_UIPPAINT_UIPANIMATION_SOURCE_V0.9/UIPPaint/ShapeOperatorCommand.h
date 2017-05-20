#ifndef __SHAPE_COMMAND
#define __SHAPE_COMMAND
#include "../UIPLib/Base.h"
#include "UndoManager.h"
#include "Shape.h"
#include "Mediator.h"
class CShapeCreateCommand : public ICommand
{
public:
	CShapeCreateCommand(CShapeBase* pShape,CFacadeMediator* pMed) : m_pShape(pShape),m_pMediator(pMed),m_idx(-1)
	{
       
	}

	~CShapeCreateCommand()
	{
	  //ÄÚ´æÐ¹Â¶ÐÞÕý	
	  if(m_idx == -1)
	  {
		  delete m_pShape;
		  m_pShape = NULL;
	  }
	}

	void Execute()
	{
		m_idx = m_pMediator->AddShape(m_pShape);
	
	}

	void Undo()
	{
		m_pMediator->RemoveShape(m_idx);
		m_idx = -1;
	}

	bool IsValid()
	{
		return ((m_pShape!=0)&&(m_pMediator != NULL));
	}

private:
	CShapeBase*   m_pShape;
	CFacadeMediator*    m_pMediator;
	int           m_idx;
};
#endif