#include "Interpreter.h"
int main()
{
	//����ڴ�й¶,�������˳��������vs����������п��Կ����ڴ�й¶(������ڴ�й¶�Ļ�)���������
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	ClientTest();
	getchar();
	return 0;
}