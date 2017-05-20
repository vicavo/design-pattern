#ifndef __IRENDER_H
#define __IRENDER_H
#include "Base.h"
enum ERenderType
{
	ERT_GDI,
	ERT_OPENGL,
	ERT_DIRECTX
};


struct RenderFactory;

struct IRenderImage
{
	virtual ~IRenderImage(){}
    virtual bool Load(const WCHAR* path) = 0;
	virtual void Destory() = 0;

	virtual Size GetSize() const = 0;
	virtual bool CreateMemImage(const Size& size) = 0;
	virtual void SavePNG(const WCHAR* path) = 0;

	void SetImageName(const wstring& name)
	{
		m_strName = name;
	}

	const wstring& GetImageName() const
	{
		return m_strName;
	}

protected:
	wstring  m_strName;
};

typedef map<wstring,IRenderImage*>    RenderImageMap;
typedef RenderImageMap::iterator     RenderImageIter;
typedef pair<wstring,IRenderImage*>   RenderImagePair;

struct IRender2D;

class CRenderImageManager
{
    friend struct IRender2D;
public:
	CRenderImageManager(ERenderType type = ERT_GDI):m_renderType(type)
	{
	}

	IRenderImage*  FindRenderImage(const wstring& name);
	
	bool           DestoryRenderImage(const wstring& name);

	size_t         GetRenderImageCount() const
	{
		return m_imageMap.size();
	}

	~CRenderImageManager();
	
private:
	ERenderType    m_renderType;
	RenderImageMap m_imageMap;
};

struct IRender2D
{
	virtual ~IRender2D(){m_hwnd = NULL;}

	virtual ERenderType  GetRenderType() const = 0;

	virtual bool Create(HWND hwnd) = 0;
	virtual bool Resize(const Size& size) = 0;

	virtual void Clear(const Color& clr = Color(255,0,0,0)) = 0;
	virtual void Present() = 0;
	virtual void Destory() = 0;
	
	virtual void DrawLine(const Color& clr,const Point& p1,const Point& p2) = 0;
	virtual void DrawLine(const Color& clr,const PointF& p1,const PointF& p2) = 0;
	virtual void DrawRect(const Color& clr,const Rect& rc,bool bFill = true) = 0;
	virtual void DrawEllipse(const Color& clr,const Rect& rc,bool bFill = true) = 0;
	virtual void DrawPolygon(const Color& clr,const Point* points,int count,bool bFill = true) = 0;

	virtual void DrawImage(const IRenderImage* pImage,const Rect& rc) = 0;
	virtual void DrawImage(const IRenderImage* pImage,const Rect& rcSrc,const Rect& rcDest) = 0;

	//绘制到前屏DC中去，而不是内存DC中
	//其他函数都是绘制到内存DC(Graphics)中去
	virtual void DrawDirtyRect(HDC hdc,const Rect& rc,const Color& clr = Color(255,255,255,255)) = 0;

	//字体相对比较复杂，在本程序中，为了演示，使用固定的字体
	//各位可以自己添加接口函数实现更加复杂的字体绘制系统
	virtual void DrawText(const TCHAR* str,const Rect& rc,const Color& clr = Color(255,0,0,0)) = 0;

	virtual void SetTransform(const Matrix& mat) = 0;
	
	virtual void ResetTransform() = 0;
	

	virtual void SetClipRect(const Rect& clipArea)
	{
       m_clipRect = clipArea;
	}

	const Rect& GetClipRect() const
	{
		return m_clipRect;
	}
    
	void SetLineWidth(float lineWidth)
	{
		m_fLineWidth = lineWidth;
	}

	float GetLineWidth() const
	{
		return m_fLineWidth;
	}

	const HWND GetHwnd() const
	{
		return m_hwnd;
	}

	IRenderImage* FindImage(const wstring& name)
	{
		return m_imageMgr.FindRenderImage(name);
	}

	bool DestoryImage(const wstring& name)
	{
		return m_imageMgr.DestoryRenderImage(name);
	}

	size_t GetImageCount() const
	{
		return m_imageMgr.GetRenderImageCount();
	}

protected:
	Rect   m_clipRect;
	HWND   m_hwnd;
	float  m_fLineWidth;
	CRenderImageManager m_imageMgr;
};

//工厂模式
struct RenderFactory
{
	static IRender2D*    CreateRender       (ERenderType renderType);
	static IRenderImage* CreateRenderImage  (ERenderType renderType);
};







#endif