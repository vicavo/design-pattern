#include "ISprite.h"
#include "../UIPLib/Application.h"

//桥接模式的实现者
class CSpriteCommonImpl
{
public:

	CSpriteCommonImpl() : m_bSelected(false),m_fRot(0.0F),m_bDrag(false),m_bShowBoundingArea(false)
	{

	}

	void    SetSelected (bool bSel)
	{
		m_bSelected = bSel;
	}

	bool    IsSelected () const
	{
		return m_bSelected;
	}

	void    SetPosition(const PointF& pt)
	{
		m_pos = pt;
	}

	PointF GetPosition()
	{
		return m_pos;
	}

	void     SetSize(const Size& sz)
	{
		m_size = sz;
	}

	Size     GetSize()
	{
		return m_size;
	}

	void     SetRotation(float rot)
	{
		m_fRot = rot;
	}

	float   GetRotation() const
	{
		return m_fRot;
	}

	Rect GetRect()
	{
		if(m_fRot == 0.0F)
			return Rect(m_pos.X - m_size.Width/2.0F,m_pos.Y - m_size.Height / 2.0F,
			            m_size.Width,m_size.Height);
		else
		{
			PointF ptMin(-m_size.Width/2.0F,-m_size.Height/2.0F);
			PointF ptMax( m_size.Width/2.0F, m_size.Height/2.0F);
			
			Matrix mat;
			GetMatrix(mat);
			float e[6];
			mat.GetElements(e);

			PointF ptMin2,ptMax2;
			ptMin2 = ptMax2 = PointF(e[4],e[5]);

            if(e[0] > 0.0F)
			{
				ptMin2.X += e[0] * ptMin.X;
				ptMax2.X += e[0] * ptMax.X;
			}
			else
			{
				ptMin2.X += e[0] * ptMax.X;
				ptMax2.X += e[0] * ptMin.X;
			}

			if(e[2] > 0.0F)
			{
				ptMin2.X += e[2] * ptMin.Y;
				ptMax2.X += e[2] * ptMax.Y;

			}
			else
			{
				ptMin2.X += e[2] * ptMax.Y;
				ptMax2.X += e[2] * ptMin.Y;
			}

			if(e[1] > 0.0F)
			{
				ptMin2.Y += e[1] * ptMin.X;
				ptMax2.Y += e[1] * ptMax.X;
			}
			else
			{
                ptMin2.Y += e[1] * ptMax.X;
				ptMax2.Y += e[1] * ptMin.X;
			}

			if(e[3] > 0.0F)
			{
				ptMin2.Y += e[3] * ptMin.Y;
				ptMax2.Y += e[3] * ptMax.Y;
			}
			else
			{
                ptMin2.Y += e[3] * ptMax.Y;
				ptMax2.Y += e[3] * ptMin.Y;
			}

			return Rect(ptMin2.X,ptMin2.Y,ptMax2.X - ptMin2.X,ptMax2.Y - ptMin2.Y);
		}
	}

	const Rect GetDrawRect() const
	{
		return Rect(-m_size.Width/2.0F,-m_size.Height/2.0F,m_size.Width,m_size.Height);
	}

	//为了省力和高效,目前仅支持AABB类型碰撞检测
	//TODO:以后增加精准的OBB碰检计数，当然相对来速度要慢一些，在2D中慢的也不算很多
	//     最精确的碰检是基于三角形计算重心坐标系的碰检技术，需要拆分多边形为三角形

	bool     HitTest(const Point& pt)
	{
		return GetRect().Contains(pt);
	}

	void     SetColor(const Color& clr)
	{
		m_clr  = clr;
	}

	const Color& GetColor() const
	{
		return m_clr;
	}

	void GetMatrix(Matrix& mat)
	{
		mat.Translate(m_pos.X,m_pos.Y);
	    mat.Rotate(m_fRot);
	}
	
	void BeginDraw(IRender2D* pRender)
	{
		Matrix mat;
		GetMatrix(mat);
		pRender->SetTransform(mat);
	}

	void EndDraw(IRender2D* pRender)
	{
		pRender->ResetTransform();

		if(m_bShowBoundingArea)
		pRender->DrawRect(Color(255,255,0,255),GetRect(),false);

	}

	void StartDrag()
	{
		m_bDrag = true;
	}

	void StopDrag()
	{
		m_bDrag = false;
	}

	void Drag(const Point& pt)
	{
		if(m_bDrag)
			SetPosition(PointF(pt.X,pt.Y));
	}

	void ShowBoundingRect(bool bShow)
	{
		m_bShowBoundingArea = bShow;
	}

private:
	bool         m_bSelected;
    float        m_fRot;
	Size         m_size;
	PointF       m_pos;
	Color        m_clr;
	bool         m_bDrag;
	bool         m_bShowBoundingArea;
};

class CRectSprite : public ISprite
{
public:
    virtual    void     SetSelected(bool bSel)
	{
		m_impl.SetSelected(bSel);
	}
	
	virtual    bool     IsSelected()      const
	{
		return m_impl.IsSelected();
	}

	virtual    void     SetPosition(const PointF& pt) 
	{
		m_impl.SetPosition(pt);
	}

	virtual    PointF  GetPosition() 
	{
		return m_impl.GetPosition();
	}

	virtual    void   SetSize(const Size& sz)
	{
		m_impl.SetSize(sz);
	}

	virtual    Size   GetSize() 
	{
		return m_impl.GetSize();
	}

	virtual    void     SetRotation(float angle) 
	{
		m_impl.SetRotation(angle);
	}

	virtual    float    GetRotation()   const 
	{
		return m_impl.GetRotation();
	}

	virtual      Rect GetRect() 
	{
		return m_impl.GetRect();
	}
	
	virtual    bool     HitTest(const Point& pt) 
	{
		return m_impl.HitTest(pt);
	}

	virtual    ESpriteType  GetType()   const 
	{
		return EST_RECT;
	}

	virtual void     SetColor(const Color& clr)
	{
		m_impl.SetColor(clr);
	}

	virtual const Color& GetColor() const
	{
		return m_impl.GetColor();
	}

	virtual void StartDrag()
	{
		m_impl.StartDrag();
	}

	virtual void StopDrag()
	{
		m_impl.StopDrag();
	}

	virtual    void     Drag(const Point& pt)
	{
		m_impl.Drag(pt);
	}

	virtual    void     ShowBoundingRect(bool bShow)
	{
		m_impl.ShowBoundingRect(bShow);
	}

	virtual void Draw()
	{
		IRender2D* pRender = App()->GetRender(); 
		m_impl.BeginDraw(pRender);
		Rect rc = m_impl.GetDrawRect();
		pRender->DrawRect(GetColor(),rc,true);
		m_impl.EndDraw(pRender);	
	}
private:
    CSpriteCommonImpl   m_impl;
 
};

class CCircleSprite : public ISprite
{
public:
	virtual    void     SetSelected(bool bSel)
	{
		m_impl.SetSelected(bSel);
	}

	virtual    bool     IsSelected()      const
	{
		return m_impl.IsSelected();
	}

	virtual    void     SetPosition(const PointF& pt) 
	{
		m_impl.SetPosition(pt);
	}

	virtual    PointF  GetPosition() 
	{
		return m_impl.GetPosition();
	}

	virtual    void   SetSize(const Size& sz)
	{
		if(sz.Width != sz.Height)
			m_impl.SetSize(Size(sz.Width,sz.Width));
		else
			m_impl.SetSize(sz);
	}

	virtual    Size   GetSize() 
	{
		return m_impl.GetSize();
	}


	virtual    void     SetRotation(float angle) 
	{
		m_impl.SetRotation(angle);
	}

	virtual    float    GetRotation()   const 
	{
		return m_impl.GetRotation();
	}

	virtual   Rect GetRect()
	{
		return m_impl.GetRect();
	}

	virtual    bool     HitTest(const Point& pt) 
	{
		return m_impl.HitTest(pt);
	}

	virtual    ESpriteType  GetType()   const 
	{
		return EST_RECT;
	}

	virtual void     SetColor(const Color& clr)
	{
		m_impl.SetColor(clr);
	}

	virtual const Color& GetColor() const
	{
		return m_impl.GetColor();
	}

	virtual void StartDrag()
	{
		m_impl.StartDrag();
	}

	virtual void StopDrag()
	{
		m_impl.StopDrag();
	}

	virtual    void     Drag(const Point& pt)
	{
		m_impl.Drag(pt);
	}

	virtual    void     ShowBoundingRect(bool bShow)
	{
		m_impl.ShowBoundingRect(bShow);
	}

	virtual void Draw()
	{
		IRender2D* pRender = App()->GetRender(); 
		m_impl.BeginDraw(pRender);
		Rect rc = m_impl.GetDrawRect();
		pRender->DrawEllipse(GetColor(),rc,true);
		m_impl.EndDraw(pRender);
	}

private:
	CSpriteCommonImpl   m_impl;

};

class CQuadBezierCurve : public IQuadBezierCurve
{
public:
	CQuadBezierCurve(const PointF& startPt,const PointF& ctrlPt, const PointF& endPt) : m_startPt(startPt),m_ctrlPt(ctrlPt),m_endPt(endPt),m_drawSteps(11),m_drawCtrlLine(true),m_bDirty(true)
	{}

	 virtual PointF   GetPosition(float t)
	 {
         assert(t >= 0.0F && t <= 1.0F);
		 float a1 = 1.0F - t;
		 float a2 = a1 * a1;

		 PointF pt;
		 pt.X = a2 * m_startPt.X + 2.0F * t * a1 * m_ctrlPt.X + t * t * m_endPt.X;
		 pt.Y = a2 * m_startPt.Y + 2.0F * t * a1 * m_ctrlPt.Y + t * t * m_endPt.Y;

		 return pt;
	 }

	virtual void     SetStartAnchorPoint(const PointF& pt)
	{
		m_startPt = pt;
		m_bDirty  = true;
	}

	virtual const    PointF& GetStartAnchorPoint() const
	{
		return m_startPt;
	}

	virtual void     SetEndAnchorPoint(const PointF& pt)
	{
		m_endPt   = pt;
		m_bDirty  = true;
	}

	virtual const    PointF& GetEndAncorPoint()    const
	{
		return m_endPt;
	}

	virtual void     SetControlPoint(const PointF& pt)
	{
		m_ctrlPt  = pt;
		m_bDirty  = true;
	}

	virtual const    PointF& GetControlPoint() const
	{
		return m_ctrlPt;
	}

	virtual void     SetDrawStepCount(int count = 10)
	{
		m_drawSteps = count;
		m_bDirty    = true;
	}

	virtual int      GetDrawStepCount()
	{
		return m_drawSteps;
	}

	virtual void     SetDrawControlLine(bool bDraw = false)
	{
		m_drawCtrlLine = bDraw;
	}

	virtual bool     GetDrawControlLine()
	{
		return m_drawCtrlLine;
	}

	virtual void     SetLineColor(const Color& clr)
	{
		m_lineClr  = clr;
	}

	virtual const Color& GetLineColor() const
	{
		return m_lineClr;
	}

	virtual void Draw() 
	{
		if(m_bDirty)
		{
			CalcDrawPoints();
			m_bDirty = false;
		}

        DrawLines();
	}

private:

	void   CalcDrawPoints()
	{
		m_arrPoints.Clear();
		float step = 1.0F / (m_drawSteps-1);
		m_arrPoints.Add(m_startPt);

		for(int i = 1; i < m_drawSteps-1; i++)
		{
			PointF pt = GetPosition(step * i);
			m_arrPoints.Add(pt);
		}

		m_arrPoints.Add(m_endPt);
	}

	void   DrawLines()
	{
		IRender2D* pRender = App()->GetRender();
		
		for(int i = 1; i < m_arrPoints.GetSize(); i++)
		{
			const PointF& start = m_arrPoints[i-1];
			const PointF& end   = m_arrPoints[i];
			pRender->DrawLine(m_lineClr,start,end);
		}

		if(m_drawCtrlLine)
			pRender->DrawLine(Color(255,255,0,0),m_startPt,m_ctrlPt);
	}

private:
	PointF                  m_startPt;
	PointF                  m_endPt;
	PointF                  m_ctrlPt;
	int                     m_drawSteps;
	bool                    m_drawCtrlLine;
	Color                   m_lineClr;

	//只有脏了，才重新计算并缓存起来
	bool                    m_bDirty;
	CArrayList<PointF>      m_arrPoints;
};

class CQuadBezierPath : public IQuadBezierPath
{
public:

     virtual void AddBezierCurve(const PointF& start,const PointF& ctrl,const PointF& end)
	 {
		 m_arrCurve.Add(CAnimFactory::CreateBezierCurve(start,ctrl,end));
	 }

	 virtual int  GetBezierCurveCount()
	 {
		 return m_arrCurve.GetSize();
	 }

	 virtual IBezierCurvePtr GetBezierCurve(int idx)
	 {
		 return m_arrCurve[idx];
	 }

	 virtual void Draw()
	 {
		 for(int i = 0; i < m_arrCurve.GetSize(); i++)
			 m_arrCurve[i]->Draw();
	 }

private:
	CArrayList<IBezierCurvePtr>  m_arrCurve;
};

ISpritePtr CAnimFactory::CreateSprite(ESpriteType     type,
		                          const PointF&   pt,
		                          const Size&     sz,
		                          const Color&    clr )
{
	switch(type)
	{
	case EST_RECT:
		{
			ISpritePtr ptr(new CRectSprite());
			ptr->SetPosition(pt);
			ptr->SetSize(sz);
			ptr->SetColor(clr);
			return ptr;
		}

	case EST_CIRCLE:
		{
			ISpritePtr ptr(new CCircleSprite());
			ptr->SetPosition(pt);
			ptr->SetSize(sz);
			ptr->SetColor(clr);
			return ptr;
		}

	default:
		assert(false);
		return ISpritePtr();
	}
}


IBezierCurvePtr  CAnimFactory::CreateBezierCurve(const PointF& start,
		                               const PointF& ctrl,
									   const PointF& end)
{
	IBezierCurvePtr ptr(new CQuadBezierCurve(start,ctrl,end));
	return ptr;
}


IBezierPathPtr     CAnimFactory::CreateBezierPath()
{
	IBezierPathPtr ptr(new CQuadBezierPath());
	return ptr;
}
