#ifndef __UTIL_HELPER_H
#define __UTIL_HELPER_H
#include "Base.h"

//为方便计算，重载操作符

Size  operator / (const Size& size,int n);

Point operator + (const Point& pt,const Size& size);

Point operator - (const Point& pt,const Size& size);


class CUtilHelper
{
public:
	static Rect ToRect(const RECT& rect);
	
	static RectF ToFRect(const RECT& rect);
	
	static RECT FromFRect(const RectF& rc);
	
	static RECT FromRect(const Rect& rc);
	
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	static void MinPoint(Point& a,const Point& b)
	{
		a.X = (a.X < b.X) ? a.X : b.X;
		a.Y = (a.Y < b.Y) ? a.Y : b.Y;
	}

	static void MaxPoint(Point& a,const Point& b)
	{
		a.X = (a.X > b.X) ? a.X : b.X;
		a.Y = (a.Y > b.Y) ? a.Y : b.Y;
	}	

	static void MinPointF(PointF& a,const PointF& b)
	{
		a.X = (a.X < b.X) ? a.X : b.X;
		a.Y = (a.Y < b.Y) ? a.Y : b.Y;
	}

	static void MaxPointF(PointF& a,const PointF& b)
	{
		a.X = (a.X > b.X) ? a.X : b.X;
		a.Y = (a.Y > b.Y) ? a.Y : b.Y;
	}	

	static bool IsCtrlDown()  
	{
		return (::GetKeyState(VK_CONTROL) < 0);
	}

	//std::string没有大小写转换函数，必须要自己实现
	static void ToLower(string &str)  
	{ 
		for (string::iterator i = str.begin(); i != str.end(); i++)  
			if (*i >= 'A' && *i <= 'Z')  
				*i = (*i) + ('a' - 'A');  
	} 

	static void ToUpper(string &str)  
	{  
		for (string::iterator i = str.begin(); i != str.end(); i++)  
			if (*i >= 'a' && *i <= 'z')  
				*i = (*i) - ('a' - 'A');  
	} 

};
#endif