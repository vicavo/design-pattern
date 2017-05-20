#include "Interpreter.h"
int main()
{
	//监控内存泄露,当我们退出程序后，在vs的输出窗口中可以看到内存泄露(如果有内存泄露的话)的相关描述
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	ClientTest();
	getchar();
	return 0;
}