#include "StdAfx.h"
#include "NEditorEngine.h"
#include "ThumbnailCreator.h"
#include "NCamera.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	nDEFINE_CLASS(neditor_engine_cli, neditor_engine)
	NEditorEngine::NEditorEngine(void):NEngine(L"neditor_engine_cli")
	{
		m_thumbCreator = nullptr;
		m_terrainEd = nullptr;
		m_navEditor = nullptr;
		m_selectionLock = false;
	}

	neditor_engine_cli* NEditorEngine::NativePtr::get()
	{
		return dynamic_cast<neditor_engine_cli*>(RawPtr);
	}

	void NEditorEngine::RenderLevel(System::String^ lvName, NViewport^ vp, bool present)
	{
		vp->Update();

		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		NativePtr->render_level( szLvName, *(vp->GetNaitvePtr()), present );
	}

	void NEditorEngine::RenderMainLevel(System::String^ lvName, NViewport^ vp)
	{
		vp->Update();
		if(m_actorEd)
			m_actorEd->UpdateView(vp->Camera);

		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		NativePtr->render_level( szLvName, *(vp->GetNaitvePtr()), true );
	}

	void NEditorEngine::UpdateLevel(System::String^ lvName, float delta_time,NViewport^ vp)
	{
		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		NativePtr->update_level(szLvName, delta_time,*(vp->GetNaitvePtr()));
	}

	NActor^ NEditorEngine::PickMainLevelActor(NViewport^ vp, int mouseX, int mouseY)
	{
		System::String^ lvName = this->MainLevelName;
		pin_ptr<const wchar_t> szLvName = PtrToStringChars(lvName);
		nactor::ptr pick = NativePtr->pick_main_level_actor(*(vp->GetNaitvePtr()), szLvName, mouseX, mouseY);

		if( !pick )
			return nullptr;

		NLevel^ lv = m_levelDict[lvName];
		System::String^ actorName = gcnew System::String(
			pick->get_name().name_str.c_str());
		return lv->FindActor(actorName);
	}

	EActorEditHandle NEditorEngine::PickActorHandle(NViewport^ vp, int mouseX, int mouseY)
	{
		hit_id pick_id = vp->GetNaitvePtr()->get_hit_id(mouseX, mouseY);
		int handle_id = m_actorEd->FindActorHandle(pick_id);
		if(handle_id!=-1)
			return EActorEditHandle(handle_id);
		else
			return EActorEditHandle::Invalid;
	}

	void NEditorEngine::InitModules(NEngineConfig^ engineCfg, NEditorConfig editofCfg)
	{
		BEGIN_NATIVE_GUARD

		NEngine::InitModules(engineCfg);
		
		m_editofCfg = editofCfg;
		CreateSubEditors();		

		m_thumbCreator = gcnew ThumbnailCreatorManager();
		m_thumbCreator->Init();

		m_commandMgr = gcnew NEditorCommandManager();
		NativePtr->set_command_manager( m_commandMgr );

		END_NATIVE_GUARD
	}

	void NEditorEngine::Close()
	{
		if(m_mainLevel!=nullptr)
		{
			DestroyLevel(m_mainLevel);
			delete m_mainLevel;
			m_mainLevel = nullptr;
		}

		m_thumbCreator->Close();
		m_thumbCreator = nullptr;
		m_actorEd->Close();
		NEngine::Close();
	}

	System::IntPtr NEditorEngine::CreateThumbnail(NResourceLoc loc, int w, int h)
	{
		return (System::IntPtr)m_thumbCreator->CreateThumbnail(loc, w, h);
	}

	int NEditorEngine::HandleViewportMessage(System::Windows::Forms::Message% msg, NViewport^ view)
	{
		//-- 按住Alt键，则所有操作都转发给Camera
		if( GetKeyState(VK_MENU)&0x1000 )
			return 2;

		if(msg.Msg == WM_KEYDOWN)
		{
			int vk = msg.WParam.ToInt32();
			if(vk == VK_SPACE)
			{
				// 使用空格键来切换锁定选择状态
				m_selectionLock = !m_selectionLock;
				if(SelectionLockTag)
					SelectionLockTag(this, nullptr);
			}
		}

		int ret = 0;
		switch( m_command )
		{
			//-- 所有摄像机相关的Command由外部的CameraCtrl处理
		case EditorCommand::FreeCamera:
		case EditorCommand::PanView:
		case EditorCommand::ArcRotateView:
			ret = 2;
			break;
		case EditorCommand::SelectObject:
		case EditorCommand::SelectMove:
		case EditorCommand::SelectRotate:
		case EditorCommand::SelectScale:
			{
				//-- 处理鼠标左键单击选择对象				
				if(msg.Msg == WM_LBUTTONDOWN)
				{
					if(!m_selectionLock)
					{
						LPARAM lParam = msg.LParam.ToInt32();
						int mouseX = LOWORD(lParam);
						int mouseY = HIWORD(lParam);

						EActorEditHandle h = PickActorHandle(view, mouseX, mouseY);
						if( h == EActorEditHandle::Invalid )
						{
							NActor^ selActor = PickMainLevelActor(view, mouseX, mouseY);
							if(GetKeyState(VK_SHIFT)&0x1000)
							{
								if( m_actorEd )
									m_actorEd->AddSelectedActor(selActor);	
							}
							else
							{
								if( m_actorEd )
									m_actorEd->BindActor(selActor);	
							}											
						}
					}//end of if
				}
				else if (msg.Msg == WM_MOUSEMOVE)
				{
					if(m_actorEd)
					{
						if(m_command != EditorCommand::SelectObject
							&& !m_actorEd->IsLeftDraging())
						{
							LPARAM lParam = msg.LParam.ToInt32();
							int mouseX = (short)LOWORD(lParam);
							int mouseY = (short)HIWORD(lParam);

							EActorEditHandle h = PickActorHandle(view, mouseX, mouseY);

							m_actorEd->SetHotActorHandle(h);
						}
					}// end of if
				}// end of else				
				//--
				if(m_actorEd)
					ret = m_actorEd->InputMessgaeProc(msg, view->Camera, m_command)?1:0;
				else
					ret = 2;
			}
			break;
		case EditorCommand::TerrainMode:
			if( m_terrainEd )
				ret = m_terrainEd->InputMessgaeProc(msg, view->Camera)?1:0;
			else
				ret = 2;
			break;

		case EditorCommand::NavigateMapMode:
			if( m_navEditor )
				ret = m_navEditor->InputMessgaeProc(msg, view->Camera)?1:0;
			else
				ret = 2;
			break;
			break;
		}

		return ret;
	}

	void NEditorEngine::CreateSubEditors()
	{
		m_terrainEd = gcnew NTerrainEditor();		
		m_navEditor = gcnew NavigateMapEditor();
		
		System::Runtime::Remoting::ObjectHandle^ actorEdObj = 
			System::Activator::CreateInstance(m_editofCfg.actorEdClassAssembly, 
			m_editofCfg.actorEditorClass);
		m_actorEd = (NActorEditorBase^)actorEdObj->Unwrap();
		m_actorEd->SetMainLevel(m_mainLevel);

		m_actorEd->Init();
	}

	void NEditorEngine::CreateMainLevel(System::String^ levelName)
	{
		//-- free old
		if(m_mainLevel != nullptr)
		{
			DestroyLevel(m_mainLevel);
			m_mainLevel = nullptr;
		}

		//-- create new
		NLevel^ newLv = CreateLevel(levelName, "nlevel");
		newLv->Init();
		
		m_mainLevel = newLv;
		ResetSubEditors();
	}

	void NEditorEngine::ResetSubEditors()
	{
		m_terrainEd->BindTerrain(nullptr);
		m_actorEd->SetMainLevel(m_mainLevel);
		m_actorEd->BindActor(nullptr);
		m_navEditor->EndNavigateMapEditor();
	}

	void NEditorEngine::MakeTextureAtlas(NResourceLoc inputFolder,NResourceLoc outFolder, int w, int h)
	{
		nexus::resource_location sourceLoc,targetLoc;
		inputFolder.ToNative(sourceLoc);
		outFolder.ToNative(targetLoc);

		BEGIN_NATIVE_GUARD

		texture_atlas_creator tac;
		tac.make_atlas(sourceLoc,targetLoc, w, h);

		END_NATIVE_GUARD
	}

	System::IntPtr NEditorEngine::LoadTextureAsBitmap(NResourceLoc loc)
	{
		return (System::IntPtr)m_thumbCreator->LoadTextureAsBitmap(loc);
	}
}//namespace NexusEngine