#include "StdAfx.h"
#include "NActorEditorBase.h"
#include "NEditorEngine.h"

namespace NexusEngine
{
	NActorEditorBase::NActorEditorBase(void)
	{
		m_hotHandle = EActorEditHandle::Invalid;
		m_level = nullptr;		

		m_leftDrag = false;

		m_actorHandles = NULL;
		m_moveFactor = 2;
		m_rotateFactor = 0.5f;
	}

	NActorEditorBase::~NActorEditorBase(void)
	{
		Close();
	}

	bool NActorEditorBase::InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam, EditorCommand cmd)
	{
		bool ret = false;
		LPARAM lParam = msg.LParam.ToInt32();
		WPARAM wParam = msg.WParam.ToInt32();

		switch(msg.Msg)
		{
		case WM_LBUTTONDOWN:
			{				
				npoint npt(LOWORD(lParam), HIWORD(lParam));				
				OnMouseLButtonDown(npt.x, npt.y, cam, cmd);
				ret = true;
			}
			break;
		case WM_LBUTTONUP:			
			OnMouseLButtonUp();
			ret = true;
			break;
		case WM_RBUTTONDOWN:
			
			ret = true;
			break;
		case WM_RBUTTONUP:
			
			ret = true;
			break;
		case WM_MOUSEMOVE:
			{				
				npoint npt(LOWORD(lParam), HIWORD(lParam));				
				this->OnMouseMove(npt.x, npt.y, 
					(wParam&MK_CONTROL)!=0,
					cam);
				ret = true;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				WPARAM wParam = msg.WParam.ToInt32();
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);				
				ret = true;
			}
			break;
		case WM_KEYDOWN:
			{
				int vk = msg.WParam.ToInt32();
				switch (vk)
				{
				case 'M':	// 把Actor放到"地面"上
					PlaceActorToGround();
					ret = true;
					break;
				case VK_DELETE:
					if(m_actor)
					{
						m_level->DestroyActor(m_actor->Name);
						BindActor(nullptr);
					}
					ret = true;
					break;
				}
			}
			break;
		}

		return ret;
	}

	void NActorEditorBase::PlaceActorToGround()
	{
		if(!m_actor)
			return;
		if(m_actor->Frozen)
			return;

		Vector3 pos = m_actor->Location;
		Vector3 chkEnd(pos.x, pos.y-5000, pos.z);
		NCheckResult chk;
		if( m_level->LineCheck(chk, pos, chkEnd) )
		{
			m_actor->Location = chk.location;
			ActorMoved();
		}
		else
		{
			// 反向测试一次
			Vector3 chkEnd2(pos.x, pos.y+5000, pos.z);
			if( m_level->LineCheck(chk, pos, chkEnd2) )
			{
				m_actor->Location = chk.location;
				ActorMoved();
			}
		}
	}

	void NActorEditorBase::DrawWidgets(NRenderPrimitiveDrawInterface^ PDI)
	{
		PDI->BeginLineBatch();
		if( m_actor != nullptr )
		{
			Color4f clr(0.95f, 0.95f, 1.0f);
			if(m_actor->Frozen)
				clr = Color4f(0.65f, 0.75f, 0.85f);
			m_actor->DrawBounds(PDI, clr);
		}				
		PDI->EndLineBatch();

		DrawActorHandles(PDI, false);
	}

	void NActorEditorBase::OnMouseMove(int x, int y, bool ctrlDown, NCamera^ cam)
	{
		if( m_level == nullptr )
			return;

		if( m_leftDrag )
		{
			int deltaX = x-m_cursorPos.X;
			int deltaY = y-m_cursorPos.Y;
			m_cursorPos.X = x;
			m_cursorPos.Y = y;

			if(m_actor != nullptr
				&& !m_actor->Frozen)
			{
				if (ctrlDown)
				{
					// 按照"地面方式"移动
					nexus::ray mray;
					cam->NativePtr->get_mouse_ray(x, y, mray);

					nexus::nlevel* lv = m_level->NativePtr;
					nexus::ncheck_result cr;

					float testDist = cam->NativePtr->get_zfar();
					if( lv->line_check(cr, mray.origin, mray.origin+mray.dir*testDist) )
					{						
						m_actor->Location = Vector3::FromNative(cr.location);						
						ActorMoved();
					}
				}
				else // 按"移动轴手柄方式"移动
				{
					switch(m_hotHandle)
					{
					case EActorEditHandle::MoveX:
						{
							Vector3 pos = m_actor->Location;
							pos.x += deltaX*m_moveFactor;
							m_actor->Location = pos;
						}
						break;
					case EActorEditHandle::MoveY:
						{
							Vector3 pos = m_actor->Location;
							pos.y -= deltaY*m_moveFactor;
							m_actor->Location = pos;
						}
						break;
					case EActorEditHandle::MoveZ:
						{
							Vector3 pos = m_actor->Location;
							pos.z += deltaX*m_moveFactor;
							m_actor->Location = pos;
						}
						break;
					case EActorEditHandle::RotateX:
						{
							Vector3 rot = m_actor->Rotation;
							rot.x += deltaY*m_rotateFactor;
							m_actor->Rotation = rot;
						}
						break;
					case EActorEditHandle::RotateY:
						{
							Vector3 rot = m_actor->Rotation;
							rot.y += deltaY*m_rotateFactor;
							m_actor->Rotation = rot;
						}
						break;
					case EActorEditHandle::RotateZ:
						{
							Vector3 rot = m_actor->Rotation;
							rot.z += deltaY*m_rotateFactor;
							m_actor->Rotation = rot;
						}
						break;
					}// end of switch
				}

				ActorMoved();
			}// end of if			
		}
	}

	void NActorEditorBase::OnMouseLButtonDown(int x, int y, NCamera^ cam, EditorCommand cmd)
	{
		m_leftDrag = true;
		m_cursorPos.X = x;
		m_cursorPos.Y = y;
	}

	void NActorEditorBase::OnMouseLButtonUp()
	{
		m_leftDrag = false;
	}

	void NActorEditorBase::Init()
	{
		m_actorHandles = new nactor_handles;
		m_actorHandles->init();
	}

	void NActorEditorBase::Close()
	{
		if( m_actorHandles )
		{
			m_actorHandles->close();
			delete m_actorHandles;
			m_actorHandles = NULL;
		}
	}

	void NActorEditorBase::DrawActorHandlesHitProxy(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy)
	{
		DrawActorHandles(PDI, hitProxy);
	}
	
	int NActorEditorBase::FindActorHandle(hit_id hid)
	{
		if( m_actorHandles )
		{
			return m_actorHandles->find_handle(hid);			
		}
		return -1;
	}

	void NActorEditorBase::BindActor(NActor^ ac)
	{	
		m_actor = ac;

		if( m_actor != nullptr
			&& m_actorHandles )
			m_actorHandles->set_actor_space( ac->NativePtr->get_space() );
	}

	void NActorEditorBase::SetHotActorHandle(EActorEditHandle h)
	{
		m_hotHandle = h;
		if(m_actorHandles)
		{
			m_actorHandles->set_hot_handle((int)h);
		}
	}

	void NActorEditorBase::ActorMoved()
	{
		if( m_actorHandles )
			m_actorHandles->set_actor_space( m_actor->NativePtr->get_space() );
	}

	void NActorEditorBase::UpdateView(NCamera^ cam)
	{
		if( m_actorHandles )
			m_actorHandles->fit_camera(cam->NativePtr);
	}

	void NActorEditorBase::DrawActorHandles(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy)
	{
		if(!m_actor)
			return;

		EditorCommand cmd = ((NEditorEngine^)NEditorEngine::Instance())->Command;
		if(m_actorHandles)
		{
			switch(cmd)
			{
			case EditorCommand::SelectObject:
			case EditorCommand::SelectMove:
				m_actorHandles->draw_move(PDI->NativePtr, hitProxy);			
				break;
			case EditorCommand::SelectRotate:
				m_actorHandles->draw_rotate(PDI->NativePtr, hitProxy);			
				break;
			}// end of switch				
		}
	}
}//namespace NexusEngine