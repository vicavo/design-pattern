#include "MyAppAnimation.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{

	//����ڴ�й¶,�������˳��������vs����������п��Կ����ڴ�й¶(������ڴ�й¶�Ļ�)���������
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//������������

	//��һ��: ����ģ�庯������Application
	CreateApplication<MyApp>();

	//�ڶ���: ��ʼ��Application
	App()->Init(hInstance,TEXT("�Ҽ��������ߣ��м��������(���һ���������Բ��)�������ѡ�϶�����������ʻ�λ�ã�r-��¼,p-�ط�"));

	//������: ����Application�������̣�ֱ���رճ���
	return App()->Run();
}
