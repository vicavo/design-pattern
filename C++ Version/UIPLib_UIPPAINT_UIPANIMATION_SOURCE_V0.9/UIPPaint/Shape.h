#ifndef __SHAPE_H
#define __SHAPE_H
#include "../UIPLib/Base.h"
#include "../UIPLib/ArrayList.h"
#include "../UIPLib/UtilHelper.h"
#include "Visitor.h"

class CFacadeMediator;

class CShapeBase
{
protected:
	CShapeBase():m_bSelected(false),m_bFill(false),m_clrLine(Color(255,255,0,0)),m_clrFill(Color(255,0,255,0)),m_bVisible(true)
	{}

	CShapeBase(const Rect& rc):m_bSelected(false),m_bFill(false),m_clrLine(Color(255,255,0,0)),m_clrFill(Color(255,0,255,0)),m_rect(rc),m_bVisible(true)
	{}

public:
	virtual ~CShapeBase(){}

    void Select()
	{
		if(!m_bVisible)
			return;
		m_bSelected = false;
	}

	void Unselect()
	{
		if(!m_bVisible)
			return;
		m_bSelected = false;
	}

	bool IsSelected() const
	{
		return m_bSelected;
	}

	const Rect& GetRect() const
	{
		return m_rect;
	}

	void SetFill(bool bFill)
	{
		m_bFill = bFill;
	}

	bool IsFill() const
	{
		return m_bFill;
	}

	void SetLineColor(const Color& clr)
	{
		m_clrLine = clr;
	}

	const Color& GetLineColor() const
	{
		return m_clrLine;
	}

	void SetFillColor(const Color& clr)
	{
		m_clrFill = clr;
	}

	const Color& GetFillColor() const
	{
		return m_clrFill;
	}

	virtual bool HitTest(const Point& pt)
	{
		if(!m_bVisible)
			return false;

		return m_rect.Contains(pt);
	}

    virtual void VisitDrawer(CDrawerVisitor* pVisitor) = 0;
	

	void SetVisible(bool bVisible)
	{
		m_bVisible = bVisible;
	}

	bool IsVisible() const
	{
		return m_bVisible;
	}

protected:

	//用于计算shape的AABB框
    virtual void CalcRect()
	{}
	
protected:
    bool            m_bFill;
	bool            m_bSelected;
	bool            m_bVisible;
	Rect            m_rect;
	Color           m_clrLine;
    Color           m_clrFill;

};

class CRectShape : public CShapeBase
{
public:
	CRectShape(const Rect& rc):CShapeBase(rc)
	{}

	void SetRect(const Rect& rc)
	{
		m_rect = rc;
	}

	void VisitDrawer(CDrawerVisitor* pVisitor)
	{
		if(!m_bVisible)
			return;

		pVisitor->VisitDraw(this);
	}

};

class CCircleShape : public CShapeBase
{
public:

	CCircleShape(const Rect& rc) : CShapeBase(rc)
	{
        CalcCenterAndRadius();
	}

	CCircleShape(const Point& center,int r) : m_ptCenter(center),m_radius(r)
	{
		CalcRect();
	}

	void SetCenter(const Point& pt)
	{
		m_ptCenter = pt;
		CalcRect();
	}

	const Point& GetCenter() const
	{
		return m_ptCenter;

	}

	void SetRadius(int r)
	{
		m_radius = r;
		CalcRect();
	}

	int  GetRadius() const
	{
		return m_radius;
	}

	bool HitTest(const Point& pt)
	{
		if(!m_bVisible)
			return false;
		//避免使用价格昂贵的开方函数
		Point diff = pt - m_ptCenter;
		int len2 = diff.X * diff.X + diff.Y * diff.Y;
		return (len2 <= m_radius * m_radius);
	}

	void VisitDrawer(CDrawerVisitor* pVisitor)
	{
		if(!m_bVisible)
			return;

		pVisitor->VisitDraw(this);
	}

protected:
	void  CalcRect()
	{
		Point leftTop(m_ptCenter.X - m_radius,m_ptCenter.Y - m_radius);
		Size  size(m_radius * 2,m_radius * 2);
		m_rect = Rect(leftTop,size);
	}

	void CalcCenterAndRadius()
	{
		Point pt;
		Size  size;

		m_rect.GetLocation(&pt);
		m_rect.GetSize(&size);

		Size halfSize = size / 2;
        m_ptCenter = pt + halfSize;

		pt = m_ptCenter - pt;

		int diff = pt.X * pt.X + pt.Y + pt.Y;
		m_radius = int(sqrtf(float(diff)));
	}

protected:
	Point      m_ptCenter;
	int        m_radius;
};


class CTriangleShape : public CShapeBase
{
public:
	CTriangleShape(const Rect& rc) : CShapeBase(rc)
	{
		CalcPoints();
	}

	CTriangleShape(const Point& a,const Point& b, const Point& c)
	{
        m_points[0] = a;
		m_points[1] = b;
		m_points[2] = c;
        CalcRect();
	}

	void VisitDrawer(CDrawerVisitor* pVisitor)
	{
		if(!m_bVisible)
			return;

		pVisitor->VisitDraw(this);
	}

	const Point* GetPoints() const
	{
		return m_points;
	}

protected:
	void CalcRect()
	{
		Point minPt,maxPt;

		for(int i = 0; i < 2; i++)
		{
			CUtilHelper::MinPoint(minPt,m_points[i]);
			CUtilHelper::MaxPoint(maxPt,m_points[i]);
		}

		m_rect = Rect(minPt,Size(maxPt.X - minPt.X,maxPt.Y - minPt.Y));
	}

	void CalcPoints()
	{
		Point pt;
		Size  size;
		m_rect.GetLocation(&pt);
        m_rect.GetSize(&size);

		m_points[0].X = pt.X + size.Width / 2;
		m_points[0].Y = pt.Y;	

		m_points[1].X = pt.X;
		m_points[1].Y = pt.Y + size.Height;

		m_points[2] = pt + size;
	}


protected:
	Point m_points[3];
};

class CShapeManager
{
public:

	CShapeManager(CFacadeMediator* pMed):m_pMediator(pMed)
	{}

	~CShapeManager()
	{
		for(int i = 0; i < m_arrShapes.GetSize(); i++)
		{
			delete m_arrShapes[i];
			m_arrShapes[i] = NULL;
		}
	}

	int AddShape(CShapeBase* pShape)
	{
		if(!pShape)
			return -1;

		m_arrShapes.Add(pShape);
		return (m_arrShapes.GetSize() - 1);
	}

	bool RemoveShape(int idx)
	{
		return m_arrShapes.Remove(idx);
	}

	int GetShapeIndex(CShapeBase* pShape)
	{
		return m_arrShapes.IndexOf(pShape);
	}

	CShapeBase* GetShape(int idx)
	{
		return m_arrShapes[idx];
	}

	int  GetShapeCount()
	{
		return m_arrShapes.GetSize();
	}

	CShapeBase* FindShape(const Point& pt)
	{
		for(int i = m_arrShapes.GetSize()-1;i >= 0; i--)
		{
			if(m_arrShapes[i]->HitTest(pt))
				return m_arrShapes[i];
		}

		return NULL;
	}

private:
	CArrayList<CShapeBase*> m_arrShapes;
	CFacadeMediator*              m_pMediator;
};





#endif