#include "IRender2D.h"

IRenderImage*  CRenderImageManager::FindRenderImage(const wstring& name)
{
	RenderImageIter iter = m_imageMap.find(name);
	if(iter != m_imageMap.end())
		return iter->second;
	else
	{
		IRenderImage* pImage = RenderFactory::CreateRenderImage(m_renderType);
		if(pImage == NULL)
			return NULL;//ÄÚ´æ·ÖÅäÊ§°Ü

		if(!pImage->Load(name.c_str()))//ÔØÈëÊ§°Ü
		{
			delete pImage;
			pImage = NULL;
			return NULL;
		}

		m_imageMap.insert(RenderImagePair(name,pImage));
		return pImage;
	}
}

bool CRenderImageManager::DestoryRenderImage(const wstring& name)
{
    RenderImageIter iter = m_imageMap.find(name);
	if(iter == m_imageMap.end())
		return false;
	delete iter->second;
	iter->second = NULL;
	m_imageMap.erase(iter);
	return true;
}

CRenderImageManager::~CRenderImageManager()
{
	RenderImageIter begin = m_imageMap.begin();
	RenderImageIter end  =  m_imageMap.end();
	while(begin != end)
	{
		delete begin->second;
		begin->second = NULL;
		++begin;
	}

	m_imageMap.clear();

}