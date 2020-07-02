#include "StdAfx.h"
#include "NGameEngine.h"

namespace NexusEngine
{
	NGameEngine::NGameEngine(void):NEngine(L"ngame_engine")
	{
	}

	ngame_engine* NGameEngine::NativePtr::get()
	{
		return dynamic_cast<ngame_engine*>(RawPtr);
	}

	void NGameEngine::Update(float deltaTime, NViewport^ vp)
	{
		vp->Update();
		NativePtr->update(deltaTime, *(vp->GetNaitvePtr()));
	}

	void NGameEngine::Render( NViewport^ vp )
	{
		NativePtr->render(*(vp->GetNaitvePtr()));
	}

	void NGameEngine::ScreenShot( System::String^ fileName,UINT32 width, UINT32 height )
	{
		pin_ptr<const wchar_t> nativeFileName = PtrToStringChars(fileName);
		NativePtr->screen_shot(nativeFileName,width,height);
	}

	void NGameEngine::ViewportShot( System::String^ fileName )
	{
		pin_ptr<const wchar_t> nativeFileName = PtrToStringChars(fileName);
		NativePtr->viewport_shot(nativeFileName);
	}

	NActor^ NGameEngine::PickLevelActor( NViewport^ vp, int mouseX, int mouseY )
	{
		nactor::ptr pick = NativePtr->pick_level_actor(*(vp->GetNaitvePtr()), mouseX, mouseY);

		if( !pick )
			return nullptr;

		//System::String^ actorName = gcnew System::String(
		//	pick->get_name().name_str.c_str());

		//for (int index=0; index<m_levelDict->Count; ++index)
		//{
		//	m_levelDict[index].Value.Find(actorName)	
		//}
		//return lv->FindActor(actorName);
		return nullptr;
	}

}//namespace NexusEngine