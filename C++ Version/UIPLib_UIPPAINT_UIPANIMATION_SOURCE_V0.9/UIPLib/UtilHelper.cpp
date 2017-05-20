#include "UtilHelper.h"

Size operator / (const Size& size,int n)
{
	Size ret;
	ret.Width  = size.Width  / n;
	ret.Height = size.Height / n;
	return ret;
}

Point operator + (const Point& pt,const Size& size)
{
	Point ret;
	ret.X = pt.X + size.Width;
	ret.Y = pt.Y + size.Height;
	return ret;
}

Point operator - (const Point& pt,const Size& size)
{
	Point ret;
	ret.X = pt.X - size.Width;
	ret.Y = pt.Y - size.Height;
	return ret;
}

Rect CUtilHelper::ToRect(const RECT& rect)
{
	Rect rc;
	rc.X  = rect.left;
	rc.Y = rect.top;
	rc.Width = rect.right - rect.left;
	rc.Height = rect.bottom - rect.top;
	return rc;
}

RectF CUtilHelper::ToFRect(const RECT& rect)
{
	RectF rc;
	rc.X  = rect.left;
	rc.Y = rect.top;
	rc.Width = rect.right - rect.left;
	rc.Height = rect.bottom - rect.top;
	return rc;
}

RECT CUtilHelper::FromFRect(const RectF& rc)
{
	RECT r;
	r.left = rc.X;
	r.top = rc.Y;
	r.right = rc.X + rc.Width;
	r.bottom = rc.Y + rc.Height;
	return r;
}

RECT CUtilHelper::FromRect(const Rect& rc)
{
	RECT r;
	r.left = rc.X;
	r.top = rc.Y;
	r.right = rc.X + rc.Width;
	r.bottom = rc.Y + rc.Height;
	return r;
}

int CUtilHelper::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}