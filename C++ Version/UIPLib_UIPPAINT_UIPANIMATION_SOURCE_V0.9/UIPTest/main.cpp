#include "TestIterator.h"
#include "Observer.h"
int main()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    
	TestArrayListIterator();
	TestNodeLevelIterator();
	TestObserver();
	getchar();
	return 0;
}