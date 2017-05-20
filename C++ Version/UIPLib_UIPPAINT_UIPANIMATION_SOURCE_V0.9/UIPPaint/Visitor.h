#ifndef __VISITOR_H
#define __VISITOR_H
class CRectShape;
class CCircleShape;
class CTriangleShape;

class CDrawerVisitor
{
public:
	virtual void VisitDraw(CRectShape* pShape);
	virtual void VisitDraw(CCircleShape* pShape);
    virtual void VisitDraw(CTriangleShape* pShape);
};

#endif