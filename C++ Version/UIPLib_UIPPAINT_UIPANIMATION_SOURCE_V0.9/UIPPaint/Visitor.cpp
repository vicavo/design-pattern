#include "Visitor.h"
#include "Shape.h"
#include "../UIPLib/Application.h"
void CDrawerVisitor::VisitDraw(CRectShape* pShape)
{
	Color clr = pShape->GetLineColor();
	bool  bFill = false;
	if(pShape->IsFill())
	{
		bFill = true;
		clr = pShape->GetFillColor();
	}

	App()->GetRender()->DrawRect(clr,pShape->GetRect(),bFill);
	
}

void CDrawerVisitor::VisitDraw(CCircleShape* pShape)
{
	Color clr = pShape->GetLineColor();
	bool  bFill = false;
	if(pShape->IsFill())
	{
		bFill = true;
		clr = pShape->GetFillColor();
	}
	App()->GetRender()->DrawEllipse(clr,pShape->GetRect(),bFill);
}

void CDrawerVisitor::VisitDraw(CTriangleShape* pShape)
{
	Color clr = pShape->GetLineColor();
	bool  bFill = false;
	if(pShape->IsFill())
	{
		bFill = true;
		clr = pShape->GetFillColor();
	}

	App()->GetRender()->DrawPolygon(clr,pShape->GetPoints(),3,bFill);
}

