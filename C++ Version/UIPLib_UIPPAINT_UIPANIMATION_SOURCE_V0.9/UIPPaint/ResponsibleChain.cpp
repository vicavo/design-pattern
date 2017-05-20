#include "ResponsibleChain.h"
#include "MyApplication.h"

void CControlHelpChain::HandleF1KeyEvent(CKeyEvent& e)
{
	if(e.KeyCode == VK_F1 && e.IsKeyDown)
	{
		MyApp* pApp = (MyApp*)App();
		pApp->GetHelpChain()->SendToChain();
	}
}