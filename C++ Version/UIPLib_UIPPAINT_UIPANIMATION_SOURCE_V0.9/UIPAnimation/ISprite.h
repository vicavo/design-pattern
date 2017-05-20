#ifndef __DRAWER_H
#define __DRAWER_H
#include "../UIPLIB/IRender2D.h"

/*
Ŀǰ���ڵ���Ҫ����֮������:
��UIPLib�е�IRender2D�У�����ʹ�õ�������
����������Ҫʹ�ø�����������ʹ�ø������޸ĵĴ���̫���ˣ���������λͬѧȥʵ�ָ������汾
�ڶ��������У����ڸ������������Ľضϣ�������ֵ���ȶ��������ʱ�������������
���ʹ�ø������������������Щ��������
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

//��������ӿڣ��Ž�ģʽ
//�Ž�ģʽ�ĺ����ǽӿ���ʵ�ַ���
//Ҳ��������ӿڱ��
class ISprite : public IDrawer
{
public:
	virtual    ~ISprite(){} 

	virtual    void     SetSelected(bool bSel)             = 0;
	
	virtual    bool     IsSelected()      const            = 0;

	virtual     Rect GetRect()             = 0;
	
	virtual    bool     HitTest(const Point& pt)           = 0;

    //���ĵ�λ�ã��������Ͻ�!!!!
	virtual    void     SetPosition(const PointF& pt)       = 0;

	virtual    PointF    GetPosition()                      = 0;

	virtual    void     SetSize(const Size& sz)            = 0;

	virtual    Size     GetSize()                          = 0;

	//��ת�ԽǶȱ�ʾ�����ǻ��ȱ�ʾ!!!
	virtual    void     SetRotation(float angle)           = 0;

	virtual    float    GetRotation()   const              = 0;

	virtual    ESpriteType  GetType()   const              = 0;   

	virtual    void     SetColor(const Color& clr)         = 0;

	virtual    const Color&  GetColor() const              = 0;

	//��ק����
	virtual    void     StartDrag()                        = 0;

	virtual    void     Drag(const Point& pt)              = 0;

	virtual    void     StopDrag()                         = 0;

	virtual    void     ShowBoundingRect(bool bShow)       = 0;

};

//���α��������ߣ�������ê���һ�����Ƶ㹹��
//�����Լ�ʵ�ֶ��α��������߹��ܡ�
//GDI+���ǻ��Ʊ��������������α��������ߣ�����ê��+�������Ƶ�
class IQuadBezierCurve : public IDrawer
{
public:

	//λ�����API
    virtual PointF   GetPosition(float t)                     = 0;

	virtual void     SetStartAnchorPoint(const PointF& pt)    = 0;

	virtual const    PointF& GetStartAnchorPoint() const      = 0;

	virtual void     SetEndAnchorPoint(const PointF& pt)      = 0;

	virtual const    PointF& GetEndAncorPoint()    const      = 0;

	virtual void     SetControlPoint(const PointF& pt)        = 0;

	virtual const    PointF& GetControlPoint() const          = 0;

    //�������API
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