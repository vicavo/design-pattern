#ifndef __GDI_RENDER_2D_H
#define __GDI_RENDER_2D_H
#include "IRender2D.h"
#include "UtilHelper.h"
class GDIRender2D;

class GDIRenderImage : public IRenderImage
{
	friend class GDIRender2D;

public:
	GDIRenderImage():m_pImage(NULL)
	{
	}

	~GDIRenderImage()
	{
		Destory();
	}

	bool Load(const WCHAR* path)
	{
		if(m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
		m_pImage = new Bitmap(path);
		if(m_pImage)
			m_strName = path;

		return (m_pImage != NULL);
	}

	void  Destory()
	{
		if(m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
	}

	Size GetSize() const
	{
		Size size;
		if(m_pImage)
		{
			size.Width =  m_pImage->GetWidth();
			size.Height = m_pImage->GetHeight();
		}
		return size;
	}

	bool CreateMemImage(const Size& size)
	{
		if(m_pImage)
		{
			delete m_pImage;
			m_pImage = NULL;
		}
		m_pImage = new Bitmap(size.Width,size.Height);
		return (m_pImage != NULL);
	}

	void SavePNG(const WCHAR* path)
	{
		if(!m_pImage)
			return;

		CLSID pngClsid;
		CUtilHelper::GetEncoderClsid(L"image/png", &pngClsid);
		m_pImage->Save(path, &pngClsid, NULL);
	}
private:
	Bitmap * m_pImage;  
};


class GDIRender2D : public IRender2D
{
public:
	GDIRender2D():m_pGraphics(NULL)
	{
		m_pFont = new Font(L"微软雅黑",10);
	}

	~GDIRender2D()
	{
		Destory();
	}

	bool Create(HWND hwnd)
	{
		RECT rc;
		::GetClientRect(hwnd,&rc);
		m_memImage.CreateMemImage(Size(rc.right-rc.left,rc.bottom-rc.top));
		m_pGraphics = Graphics::FromImage(m_memImage.m_pImage);
		if(m_pGraphics != NULL)
		{
			m_hwnd = hwnd;
			m_fLineWidth = 1.0F;
			m_clipRect.X = rc.left;
			m_clipRect.Y = rc.top;
			m_clipRect.Width = rc.right-rc.left;
			m_clipRect.Height = rc.bottom-rc.top;
			return true;
		}
		return false;
	}

	bool Resize(const Size& size)
	{
        if(m_pGraphics)
		{
			delete m_pGraphics;
			m_pGraphics = NULL;
		}

		m_memImage.CreateMemImage(size);
        m_pGraphics = Graphics::FromImage(m_memImage.m_pImage);

		if(m_pGraphics != NULL)
		{
			m_clipRect.X = 0;
			m_clipRect.Y = 0;
			m_clipRect.Width = size.Width;
			m_clipRect.Height = size.Height;
			return true;
		}

		return false;
	}

	void Destory()
	{
		if(m_pGraphics)
		{
			//blf
			m_memImage.Destory();
			delete m_pFont;
			delete m_pGraphics;
			m_pGraphics = NULL;
			m_pFont     = NULL;
		}
	}

    void SetClipRect(const Rect& clipArea)
	{
		if(m_pGraphics)
		{
			IRender2D::SetClipRect(clipArea);
			m_pGraphics->SetClip(clipArea);
		}
	}

	void Clear(const Color& clr = Color(255,0,0,0))
	{
		if(m_pGraphics)
			m_pGraphics->Clear(clr);
	}

	//使用双缓存绘图
	//该方法将后备缓冲区中的内容blit到前屏缓冲区中
	//以避免闪烁现象
	void Present()
	{
		HDC hdc = ::GetDC(m_hwnd);
		Graphics winGraphics(hdc);
		winGraphics.DrawImage(m_memImage.m_pImage,0,0);
	}

	ERenderType GetRenderType() const
	{
          return ERT_GDI;
	}

	void DrawLine(const Color& clr,const Point& p1,const Point& p2)
	{
		if(m_pGraphics)
			m_pGraphics->DrawLine(&Pen(clr,m_fLineWidth),p1,p2);
	}

	virtual void DrawLine(const Color& clr,const PointF& p1,const PointF& p2)
	{
		if(m_pGraphics)
			m_pGraphics->DrawLine(&Pen(clr,m_fLineWidth),p1,p2);
	}

	void DrawRect(const Color& clr,const Rect& rc,bool bFill = true)
	{
		if(!m_pGraphics)
			return;
		if(bFill)
			m_pGraphics->FillRectangle(&SolidBrush(clr),rc);
		else
			m_pGraphics->DrawRectangle(&Pen(clr,m_fLineWidth),rc);
	}

	void DrawEllipse(const Color& clr,const Rect& rc,bool bFill = true)
	{
		if(!m_pGraphics)
			return;
		if(bFill)
			m_pGraphics->FillEllipse(&SolidBrush(clr),rc);
		else
			m_pGraphics->DrawEllipse(&Pen(clr,m_fLineWidth),rc);
	}

	void DrawPolygon(const Color& clr,const Point* points,int count,bool bFill = true)
	{
		if(!m_pGraphics)
			return;
		if(bFill)
			m_pGraphics->FillPolygon(&SolidBrush(clr),points,count);
		else
			m_pGraphics->DrawPolygon(&Pen(clr,m_fLineWidth),points,count);
	}

	void DrawImage(const IRenderImage* pImage,const Rect& rcSrc,const Rect& rcDest)
	{
		if(!m_pGraphics || !pImage)
			return;
     
		const GDIRenderImage* pGdiImage = static_cast<const GDIRenderImage*>(pImage);
        
		m_pGraphics->DrawImage(pGdiImage->m_pImage,rcDest,rcSrc.X,rcSrc.Y,rcSrc.Width,rcSrc.Height,UnitPixel);
	}


	void DrawImage(const IRenderImage* pImage,const Rect& rc)
	{
		if(!m_pGraphics || !pImage)
			return;

		const GDIRenderImage* pGdiImage = static_cast<const GDIRenderImage*>(pImage);

		m_pGraphics->DrawImage(pGdiImage->m_pImage,rc);
	}

	void DrawText(const TCHAR* str,const Rect& rc,const Color& clr = Color(255,0,0,0))
	{
		if(!m_pGraphics || !m_pFont)
			return;

        StringFormat format;
		format.SetAlignment(StringAlignmentCenter);
		format.SetLineAlignment(StringAlignmentCenter);
        SolidBrush b(clr);

		RectF fRect;
		fRect.X = rc.X;
		fRect.Y = rc.Y;
		fRect.Width  = rc.Width;
		fRect.Height = rc.Height;
		
		m_pGraphics->DrawString(str,wcslen(str),m_pFont,fRect,&format,&b);
	}


	void DrawDirtyRect(HDC hdc,const Rect& rc,const Color& clr = Color(255,255,255,255))
	{
		Graphics g(hdc);
		g.DrawRectangle(&Pen(clr,3.0F),rc);
	}


	void SetTransform(const Matrix& mat)
	{
		if(!m_pGraphics)
			return;

		m_pGraphics->SetTransform(&mat);
	}

	
	void ResetTransform()
	{
		if(!m_pGraphics)
			return;

		m_pGraphics->ResetTransform();
	}


private:
	Graphics* m_pGraphics;
	GDIRenderImage m_memImage;
	Font*      m_pFont;
};

#endif