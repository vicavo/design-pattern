#ifndef __BASE_H
#define __BASE_H
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <string>
#include <deque>
#include <stack>
#include <queue>
#include <memory>
#include <map>
#include <assert.h>
#include <string.h>
#include <new.h>
#include <TCHAR.h>
#include <math.h>
#include "FastDelegate.h"

using namespace fastdelegate;
using namespace Gdiplus;
using namespace std;
using namespace std::tr1;

#define PI      3.1415926F
#define DEG2RAD (PI / 180.0F)
#define RAD2DEG (180.0F / PI)

#ifdef _DEBUG
static void DebugOutput(const char * format, ...)
{
	char s[256];
	va_list argptr;

	va_start( argptr, format );
	wvsprintfA( s, format, argptr );
	va_end( argptr );
	
	OutputDebugStringA(s);
}
#endif

#endif