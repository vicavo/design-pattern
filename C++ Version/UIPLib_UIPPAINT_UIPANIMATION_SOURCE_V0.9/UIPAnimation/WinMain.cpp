#include "MyAppAnimation.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{

	//监控内存泄露,当我们退出程序后，在vs的输出窗口中可以看到内存泄露(如果有内存泄露的话)的相关描述
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	//程序运行流程

	//第一步: 调用模板函数创建Application
	CreateApplication<MyApp>();

	//第二步: 初始化Application
	App()->Init(hInstance,TEXT("右键创建曲线，中键封闭曲线(最后一个点必须是圆形)，左键点选拖动句柄调整曲率或位置，r-记录,p-回放"));

	//第三步: 进入Application运行流程，直到关闭程序
	return App()->Run();
}
