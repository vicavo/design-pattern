#ifndef __DRAWER_H
#define __DRAWER_H
#include "../UIPLIB/IRender2D.h"

/*
目前存在的主要不足之处在于:
在UIPLib中的IRender2D中，我们使用的是整数
而动画都需要使用浮点数，但是使用浮点数修改的代码太多了，就留给各位同学去实现浮点数版本
在动画过程中，由于浮点数到整数的截断，导致数值不稳定，因而有时会产生抖动现象
如果使用浮点数，则会消除掉这些抖动现象。
*/

enum ESpriteType
{
	EST_RECT,
	EST_CIRCLE
};

class IDrawer
{
public:
	virtual void Draw()          =  0;
	
};

//动画精灵接口，桥接模式
//桥接模式的核心是接口与实现分离
//也就是面向接口编程
class ISprite : public IDrawer
{
public:
	virtual    ~ISprite(){} 

	virtual    void     SetSelected(bool bSel)             = 0;
	
	virtual    bool     IsSelected()      const            = 0;

	virtual     Rect GetRect()             = 0;
	
	virtual    bool     HitTest(const Point& pt)           = 0;

    //中心点位置，不是左上角!!!!
	virtual    void     SetPosition(const PointF& pt)       = 0;

	virtual    PointF    GetPosition()                      = 0;

	virtual    void     SetSize(const Size& sz)            = 0;

	virtual    Size     GetSize()                          = 0;

	//旋转以角度表示而不是弧度表示!!!
	virtual    void     SetRotation(float angle)           = 0;

	virtual    float    GetRotation()   const              = 0;

	virtual    ESpriteType  GetType()   const              = 0;   

	virtual    void     SetColor(const Color& clr)         = 0;

	virtual    const Color&  GetColor() const              = 0;

	//拖拽功能
	virtual    void     StartDrag()                        = 0;

	virtual    void     Drag(const Point& pt)              = 0;

	virtual    void     StopDrag()                         = 0;

	virtual    void     ShowBoundingRect(bool bShow)       = 0;

};

//二次贝塞尔曲线，由两个锚点和一个控制点构成
//我们自己实现二次贝塞尔曲线功能。
//GDI+中是绘制贝塞尔曲线是三次贝塞尔曲线，两个锚点+两个控制点
class IQuadBezierCurve : public IDrawer
{
public:

	//位置相关API
    virtual PointF   GetPosition(float t)                     = 0;

	virtual void     SetStartAnchorPoint(const PointF& pt)    = 0;

	virtual const    PointF& GetStartAnchorPoint() const      = 0;

	virtual void     SetEndAnchorPoint(const PointF& pt)      = 0;

	virtual const    PointF& GetEndAncorPoint()    const      = 0;

	virtual void     SetControlPoint(const PointF& pt)        = 0;

	virtual const    PointF& GetControlPoint() const          = 0;

    //绘制相关API
	virtual void     SetDrawStepCount(int count)              = 0;

	virtual int      GetDrawStepCount()                       = 0;

	virtual void     SetDrawControlLine(bool bDraw )          = 0;

	virtual bool     GetDrawControlLine()                     = 0;

	virtual void     SetLineColor(const Color& clr)           = 0;

	virtual const Color& GetLineColor() const                 = 0;

};


typedef shared_ptr<ISprite>            ISpritePtr;
typedef shared_ptr<IQuadBezierCurve>   IBezierCurvePtr;


class IQuadBezierPath : public IDrawer
{
public:

     virtual void AddBezierCurve(const PointF& start,const PointF& ctrl,const PointF& end) = 0;

	 virtual int  GetBezierCurveCount() = 0;

	 virtual IBezierCurvePtr GetBezierCurve(int idx) = 0;

};

typedef shared_ptr<IQuadBezierPath>    IBezierPathPtr;


class CAnimFactory
{
public:
	ISpritePtr         static CreateSprite(ESpriteType     type,
		                                   const PointF&   pt,
		                                   const Size&     sz,
		                                   const Color&    clr = Color(255,255,0,0));

	IBezierCurvePtr    static CreateBezierCurve(const PointF& start,
		                                        const PointF& ctrl,
										        const PointF& end);

	IBezierPathPtr     static CreateBezierPath();
};


#endif