#ifndef __APP_KEY_EVENT_H
#define __APP_KEY_EVENT_H
/*
����Undo/Redo���Ե�ʱ������ʹ�õ�������м�undo,����Ҽ�redo����������Ҫ����win32��׼��ctrl+ZΪundo,
ctrl+YΪredo����ʱ����CApplication�ļ����¼��Ǵӻ�ý���Ŀؼ������ģ���undo/redo��Ҫȫ�ֵļ����¼���
����ԭ����CApplication�ļ����¼����������ǵ�����

�����ַ�����
1) ��UIPLib�е�CApplication������һ��CAppKeyEvent�¼��������ơ�������Ҫ���Ǹ���UIPLib�е�Դ���롣
2) ������UIPPaint�н�����չʵ�ָ��¼��������ơ���������£�����Ҫ�޸�UIPLib��Դ���룬�ܷ��Ͽ���ԭ��
��ô�����Եڶ��ַ�ʽ������չ��
������Ӻܺõ���ʾ�����ǵ�UIPLib����չ��
*/
#include "../UIPLib/EventSystem.h"
class CApplication;

struct CAppKeyEvent : public CEvent<CApplication*>
{
	int  KeyCode;//VK_��ֵ��Ϊwin32�����
    bool IsKeyDown;

	CAppKeyEvent(CApplication* pApp,CKeyEvent& e) : CEvent(pApp,"AppKeyEvent"),KeyCode(e.KeyCode),IsKeyDown(e.IsKeyDown)
	{}
};

typedef CEventDispatcher<CAppKeyEvent>         CAppKeyEventDispatcher;
typedef CEventListener<void(CAppKeyEvent&)>    CAppKeyEventListener;

#endif