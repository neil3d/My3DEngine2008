#include "StdAfx.h"
#include "NViewport.h"

namespace NexusEngine
{
	NViewport::NViewport(void)
	{
		m_obj = new nviewport;
		m_camera = gcnew NCamera;
	}

	NViewport::~NViewport(void)
	{
		delete m_obj;
		m_obj = NULL;
	}

	void NViewport::Update()
	{
		m_obj->camera = *(m_camera->NativePtr);
		m_obj->update();
	}

	void NViewport::CreateHitHash()
	{
		m_obj->create_hit_hash();
	}

	void NViewport::Destroy()
	{
		m_obj->destroy();
	}
}//namespace NexusEngine