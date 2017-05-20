#ifndef __ANIMATION_CONTROLER
#define __ANIMATION_CONTROLER
#include "ISprite.h"
#include "../UIPControl/UIPPaintArea.h"
struct SMotionInfo
{
	int                 m_lineIdx;
	float               m_timeT;
	float               m_dt;//如果多个运动的速度要求一样的话，可以将这个变量放在享元类里面
    PointF              m_pos;
	

	SMotionInfo(int lineIdx = 0,float timeT = 0.0F,float dt = 0.008F) : m_lineIdx(lineIdx),m_timeT(timeT),m_dt(dt)
	{}
};

class CMotionMemento
{
public:
	void  AddMotionInfo(const SMotionInfo& m)
	{
		m_arrMotionInfo.Add(m);
	}

	void Reset()
	{
		m_arrMotionInfo.Clear();
	}

	const SMotionInfo&  GetMotionInfo(int idx)
	{
		return m_arrMotionInfo[idx];
	}

	int  GetMotionInfoCount() const
	{
		return m_arrMotionInfo.GetSize();
	}

private:
	CArrayList<SMotionInfo> m_arrMotionInfo;
};

//flyWeight类
//1、所有运动信息以参数传入计算得到
//2、共享了组合对象ISpritePtr
//3、共享了聚合对象IBezierPathPtr
//4、共享了float类型的成员变量
class CPathFollower
{
public:

	CPathFollower(IBezierPathPtr path):m_pPath(path),m_maxSpeed(0.02F)
	{
		assert(m_pPath->GetBezierCurveCount() > 0);
		m_pSprite = CAnimFactory::CreateSprite(EST_RECT,m_pPath->GetBezierCurve(0)->GetStartAnchorPoint(),Size(30,20),Color(255,255,255,0));
		m_pSprite->ShowBoundingRect(true);
	}

	ISpritePtr  GetSprite()
	{
		return m_pSprite;
	}

	void SetMaxSpeed(float speed)
	{
		m_maxSpeed = speed;
	}

	float GetMaxSpeed() const
	{
		return m_maxSpeed;
	}

	void Draw(SMotionInfo& info)
	{
		Update(info);
		m_pSprite->Draw();
	}
private:

	void Update(SMotionInfo& info)
	{
		if(info.m_dt > m_maxSpeed)
			info.m_dt = m_maxSpeed;

		info.m_timeT += info.m_dt;
		info.m_lineIdx = (info.m_lineIdx + (info.m_timeT >= 1.0F)) * 
			             (info.m_lineIdx < m_pPath->GetBezierCurveCount());

		if(info.m_lineIdx == m_pPath->GetBezierCurveCount())
			info.m_lineIdx = 0;

		info.m_timeT = fmod(info.m_timeT,1.0F);

		PointF lastPos = info.m_pos;
		info.m_pos  = m_pPath->GetBezierCurve(info.m_lineIdx)->GetPosition(info.m_timeT);
		PointF diff = info.m_pos - lastPos;
		float angle = atan2f(diff.Y,diff.X) * RAD2DEG;
		m_pSprite->SetPosition(info.m_pos);
		m_pSprite->SetRotation(angle);
	}

private:

	ISpritePtr          m_pSprite;
	IBezierPathPtr      m_pPath;
	float               m_maxSpeed;
};

typedef shared_ptr<CPathFollower> CPathFollowerPtr;

//CAnimationControler是备忘录模式中的发起人和负责人
class CAnimationControler
{
public:

	CAnimationControler(CPaintArea* pArea) : m_pArea(pArea),m_hitIdx(-1),m_count(0),m_bEnd(false)
	{
		assert(m_pArea);
		m_pPath = CAnimFactory::CreateBezierPath();

		pArea->RenderEventDispatcher.RegisterListener(CRenderEventListener(this,&CAnimationControler::HandlePaintAreaRenderEvent));

		pArea->MouseButtonEventDispatcher.RegisterListener(CMouseButtonEventListener(this,
			&CAnimationControler::HandleMouseButtonEvent));

		pArea->MouseMoveEventDispatcher.RegisterListener(CMouseMoveEventListener(this,
			&CAnimationControler::HandleMouseMoveEvent));

		pArea->KeyCharEventDispatcher.RegisterListener(CKeyCharEventListener(this,
			&CAnimationControler::HandleKeyCharEvent));
	}


	void Record()
	{
		m_memento.Reset();
		for(int i = 0; i < m_arrPathInfo.GetSize(); i++)
		{
			m_memento.AddMotionInfo(m_arrPathInfo[i]);
		}
	}

	void Replay()
	{
		for(int i = 0; i < m_memento.GetMotionInfoCount(); i++)
		{
			m_arrPathInfo[i] = m_memento.GetMotionInfo(i);
		}
	}

private:

	void HandlePaintAreaRenderEvent(CRenderEvent& e)
	{
		
		m_pPath->Draw();

		for(int i = 0; i < m_arrSprite.GetSize(); i++)
			m_arrSprite[i]->Draw();

		if(m_pFollower.get())
		{
			for(int i = 0; i < m_arrPathInfo.GetSize(); i++)
				m_pFollower->Draw(m_arrPathInfo[i]);
			
		}

	}

	void HandleMouseButtonEvent(CMouseButtonEvent& e)
	{
		if(e.EventSrc == m_pArea)
		{
			if(e.ButtonType == LBUTTON && e.ButtonState == STATE_DOWN)
			{
				m_hitIdx = GetHitSpriteIdx(e.MousePt);
				if(m_hitIdx != -1)
					m_arrSprite[m_hitIdx]->StartDrag();
			}
			else if(e.ButtonType == LBUTTON && e.ButtonState == STATE_UP)
			{
				if(m_hitIdx != -1)
				{
					m_arrSprite[m_hitIdx]->StopDrag();
					m_hitIdx = -1;
				}
			}
			else if(e.ButtonType == RBUTTON && e.ButtonState == STATE_DOWN)
			{
				if(m_bEnd)
					return;
                //m_count 0 base
				//偶数时为Rect，奇数时为Circle
				if((m_count % 2) == 0)
					m_arrSprite.Add(CAnimFactory::CreateSprite(EST_RECT,PointF(e.MousePt.X,e.MousePt.Y),
					Size(10,10)));
				else
					m_arrSprite.Add(CAnimFactory::CreateSprite(EST_CIRCLE,PointF(e.MousePt.X,e.MousePt.Y)                    ,Size(10,10),Color(255,0,0,0)));
				//m_count计数器加1
				m_count++;

				//二阶贝塞尔曲线由锚点+控制点+锚点构成
				//添加一条二阶曲线时最后一个点必须是锚点
				//由于上面m_count++了，所以变成了奇数
				if((m_count >= 3) && (m_count % 2 ) != 0)
				{
					m_pPath->AddBezierCurve(m_arrSprite[m_count-3]->GetPosition(),
						                    m_arrSprite[m_count-2]->GetPosition(),  
											m_arrSprite[m_count-1]->GetPosition());
				}
			}//用于封闭曲线
			else if(e.ButtonType == MBUTTON && e.ButtonState == STATE_DOWN)
			{
				if(m_bEnd)
					return;//如果标记变量为true，说明已经是封闭的曲线了，直接退出
                 
				//添加最后一个锚点，最后一个锚点的上一个必须是控制点
				//并且最起码要一条以上曲线才能由另外一条曲线进行封闭
               	if((m_count > 3) && (m_count % 2 ) == 0)
				{
					m_pPath->AddBezierCurve(m_arrSprite[m_count-2]->GetPosition(),
                                            m_arrSprite[m_count-1]->GetPosition(),
											m_arrSprite[0]->GetPosition());
				
					//创建一个享元对象指针
					m_pFollower.reset(new CPathFollower(m_pPath));

                    //一个享元对象可以使用多个外部的运行信息，目前为3个
					for(int i = 0; i < 3; i++)
					{
						SMotionInfo info(0,1.0F/3 * i,i * 0.004F + 0.008F);
						m_arrPathInfo.Add(info);
					}
					
					m_bEnd = true;//标记封闭曲线为true
				}

			}
		}
	}

	void HandleMouseMoveEvent(CMouseMoveEvent& e)
	{
		if(e.EventSrc == m_pArea)
		{
			if(m_hitIdx != -1)
			{
				//m_hitIdx 0 base的
				//奇数代表的是控制点
				//偶数代表的是锚点
				//m_arrSprite的个数和IBezierPathPtr的曲线个数之间的关系是
				//m_arrSprite.count / 2  = IBezierPathPtr.count
				if((m_hitIdx % 2) != 0)
				{
                     int ctrlPtIdx = m_hitIdx / 2;//控制点在哪条曲线上
					 m_pPath->GetBezierCurve(ctrlPtIdx)->SetControlPoint
						      (m_arrSprite[m_hitIdx]->GetPosition());
				}
				else
				{
					int anchorPtIdx = m_hitIdx / 2;//锚点在哪条曲线上
					if(anchorPtIdx == 0)//0曲线比较特别处理
					{
						m_pPath->GetBezierCurve(anchorPtIdx)->SetStartAnchorPoint
							(m_arrSprite[m_hitIdx]->GetPosition());
						m_pPath->GetBezierCurve(m_pPath->GetBezierCurveCount()-1)
							->SetEndAnchorPoint(m_arrSprite[m_hitIdx]->GetPosition());
					}
					else
					{
						m_pPath->GetBezierCurve(anchorPtIdx)->SetStartAnchorPoint
							(m_arrSprite[m_hitIdx]->GetPosition());
						m_pPath->GetBezierCurve(anchorPtIdx-1)
							->SetEndAnchorPoint(m_arrSprite[m_hitIdx]->GetPosition());
					}
                     
				}
				m_arrSprite[m_hitIdx]->Drag(e.MousePt);

			}
		}
	}

	void HandleKeyCharEvent(CKeyCharEvent& e)
	{
		if(e.EventSrc == m_pArea)
		{
			if(e.KeyChar == 'r')
				Record();
			else if(e.KeyChar == 'p')
				Replay();
		}
	}

private:
	int  GetHitSpriteIdx(const Point& pt)
	{
		for(int i = m_arrSprite.GetSize() - 1; i >= 0; i--)
		{
			if(m_arrSprite[i]->HitTest(pt))
				return i;
		}

		return -1;
	}
private:

	CPaintArea*                  m_pArea;

	IBezierPathPtr               m_pPath;
	CPathFollowerPtr             m_pFollower;
	CArrayList<ISpritePtr>       m_arrSprite; 
	CArrayList<SMotionInfo>      m_arrPathInfo;
	

	int                          m_hitIdx;
	int                          m_count;
	bool                         m_bEnd;

	CMotionMemento               m_memento;
};

typedef shared_ptr<CAnimationControler> CAnimPtr;
#endif