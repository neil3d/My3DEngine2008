#include "StdAfx.h"
#include "NActorEditorBase.h"
#include "NEditorEngine.h"
#include "NActorMoveCommand.h"

namespace NexusEngine
{
	NActorEditorBase::NActorEditorBase(void)
	{
		m_hotHandle = EActorEditHandle::Invalid;
		m_level = nullptr;		

		m_leftDrag = false;

		m_actorHandles = NULL;
		m_moveFactor = 2;
		m_rotateFactor = 0.05f;

		m_move_align=1.0f;
		m_rotate_align=0.001f;
		m_scale_align=0.001f;

		m_shift_down=false;
		m_enable_shift_copy=true;

		m_actor_list=gcnew ActorList();
		m_copy_list=gcnew System::Collections::Generic::List<String^>();
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
				npoint npt = npoint((short)LOWORD(lParam), (short)HIWORD(lParam));
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
				case VK_SHIFT:	//按住shift移动的话，视为复制行为
					m_shift_down=true;
					break;
				case VK_DELETE:
					for(int i=0;i<m_actor_list->Count;++i)
					{
						if(m_actor_list[i]->Frozen) continue;
						m_level->DestroyActor(m_actor_list[i]->Name);
					}
					m_actor_list->Clear();
					BindActor(nullptr);
					ret = true;
					break;
				default:
					m_shift_down=false;
				}
				break;
			}
		case WM_KEYUP:
			{
				m_shift_down=false;			//赋值总比判断+跳转+赋值效率高
				m_enable_shift_copy=true;	//按下shift并且赋值Actor以后，必须松开并再次按下shift，才能进行下一次的复制
				break;
			}
			break;
		}

		return ret;
	}

	void NActorEditorBase::PlaceActorToGround()
	{
		bool s=false;
		for (int i=0;i<m_actor_list->Count;++i)
		{
			NActor^ actor=m_actor_list[i];
			if(!actor) continue;
			if(actor->Frozen) continue;

			Vector3 pos = actor->Location;
			Vector3 chkEnd(pos.x, pos.y-5000, pos.z);
			NCheckResult chk;
			bool find = false;
			if( m_level->LineCheck(chk, pos, chkEnd,LineCheckType::Terrain) )
			{
				find = true;			
			}
			else
			{
				// 反向测试一次
				Vector3 chkEnd2(pos.x, pos.y+5000, pos.z);
				if( m_level->LineCheck(chk, pos, chkEnd2,LineCheckType::Terrain) )
				{
					find = true;
				}
			}

			ObjectSpace sp = actor->SpaceData;
			sp.location = chk.location;
			align(sp.location,m_move_align);
			NActorMoveCommand^ cmd = gcnew NActorMoveCommand(actor, sp);
			if( cmd->Execute() )
			{
				s=true;
				NEditorEngine::Instance()->CommandMgr->PushCommand(cmd);
			}
		}
		
		ActorMoved();
	}

	void NActorEditorBase::DrawWidgets(NRenderPrimitiveDrawInterface^ PDI)
	{
		PDI->BeginLineBatch();
		for (int i=0;i<m_actor_list->Count;++i)
		{
			NActor^ actor=m_actor_list[i];
			if( actor != nullptr )
			{
				Color4f clr(0.95f, 0.95f, 1.0f);
				if(actor->Frozen)
					clr = Color4f(0.65f, 0.75f, 0.85f);
				actor->DrawBounds(PDI, clr);
			}
		}		
		PDI->EndLineBatch();

		DrawActorHandles(PDI, false);
	}

	void NActorEditorBase::OnMouseMove(int x, int y, bool ctrlDown, NCamera^ cam)
	{
		if( m_level == nullptr )
			return;

		NMultiActorMoveCommand^ newCmd = gcnew NMultiActorMoveCommand();

		if( m_leftDrag )
		{
			int deltaX = x-m_cursorPos.X;
			int deltaY = y-m_cursorPos.Y;			

			if(m_actor != nullptr)
			{
				if (ctrlDown)
				{
					// 按照"地面方式"移动
					nexus::ray mray;
					cam->NativePtr->get_mouse_ray(x, y, mray);

					nexus::nlevel* lv = m_level->NativePtr;
					nexus::ncheck_result cr;

					float testDist = cam->NativePtr->get_zfar();
					if( lv->line_check(cr, mray.origin, mray.origin+mray.dir*testDist, ELCT_Terrain) )
					{						
						ObjectSpace sp = m_actor->SpaceData;
						Vector3 new_loc= Vector3::FromNative(cr.location);
						new_loc-=sp.location;
						for(int i=0;i<m_actor_list->Count;++i)
						{
							NActor^ actor=m_actor_list[i];
							if(actor==nullptr || actor->Frozen) continue;
							sp = actor->SpaceData;
							sp.location+=new_loc;
							align(sp.location,m_move_align);
							newCmd->AddCommand(actor, sp);
						}
					}
				}
				else // 按"移动轴手柄方式"移动
				{
					// 按照屏幕空间转换到world空间计算
					Vector3 wCursor = cam->ScreenToWorld(m_cursorPos.X, m_cursorPos.Y);
					Vector3 wNow = cam->ScreenToWorld(x, y);

					Vector3 deltaPos = wNow-wCursor;
					deltaPos.y = 0;
					deltaPos.Normalize();
					deltaPos *= (m_moveFactor*10);

					Vector3 dpos(0,0,0),drot(0,0,0);

					bool badHandle = false;
					switch(m_hotHandle)
					{
					case EActorEditHandle::MoveX:
						{							
							deltaPos.z = 0;
							m_move_states+=deltaPos;
						}
						break;
					case EActorEditHandle::MoveY:
						{							
							m_move_states.y-=deltaY*m_moveFactor;
						}
						break;
					case EActorEditHandle::MoveZ:
						{
							deltaPos.x = 0;
							m_move_states+=deltaPos;
						}
						break;
					case EActorEditHandle::RotateX:
						{
							m_rotate_states.x += deltaY*m_rotateFactor;	
						}
						break;
					case EActorEditHandle::RotateY:
						{
							m_rotate_states.y += deltaY*m_rotateFactor;							
						}
						break;
					case EActorEditHandle::RotateZ:
						{
							m_rotate_states.z += deltaY*m_rotateFactor;							
						}
						break;
					default:
						badHandle = true;
						break;
					}// end of switch

					
					if( !badHandle )
					{
						if(fabs(m_move_states.x)>=m_move_align)
						{
							float last=fmodf(m_move_states.x,m_move_align);
							dpos.x+=(m_move_states.x-last);
							m_move_states.x=last;
						}
						if(fabs(m_move_states.y)>=m_move_align)
						{
							float last=fmodf(m_move_states.y,m_move_align);
							dpos.y+=(m_move_states.y-last);
							m_move_states.y=last;
						}
						if(fabs(m_move_states.z)>=m_move_align)
						{
							float last=fmodf(m_move_states.z,m_move_align);
							dpos.z+=(m_move_states.z-last);
							m_move_states.z=last;
						}

						if(fabs(m_rotate_states.x)>=m_rotate_align)
						{
							float last=fmodf(m_rotate_states.x,m_rotate_align);
							drot.x+=(m_rotate_states.x-last);
							m_rotate_states.x=last;
						}
						if(fabs(m_rotate_states.y)>=m_rotate_align)
						{
							float last=fmodf(m_rotate_states.y,m_rotate_align);
							drot.y+=(m_rotate_states.y-last);
							m_rotate_states.y=last;
						}
						if(fabs(m_rotate_states.z)>=m_rotate_align)
						{
							float last=fmodf(m_rotate_states.z,m_rotate_align);
							drot.z+=(m_rotate_states.z-last);
							m_rotate_states.z=last;
						}

						for(int i=0;i<m_actor_list->Count;++i)
						{
							if(m_shift_down && m_enable_shift_copy)
							{
								NActor^ actor=m_level->CloneActor(m_actor_list[i]);
								if(actor==nullptr) continue;
								if(m_actor_list[i]==m_actor)
								{
									m_actor=actor;
									if( m_actor != nullptr && m_actorHandles )
										m_actorHandles->set_actor_space( m_actor->NativePtr->get_space() );
								}
								m_actor_list[i]=actor;
								if(actor==nullptr || actor->Frozen || !actor->Serializable) continue;
								ObjectSpace sp = actor->SpaceData;
								sp.location+=dpos;
								sp.rotation+=drot;
								align(sp.location,m_move_align);
								align(sp.rotation,m_rotate_align);
								newCmd->AddCommand(actor, sp);
							}
							else
							{
								NActor^ actor=m_actor_list[i];
								if(actor==nullptr || actor->Frozen) continue;
								ObjectSpace sp = actor->SpaceData;
								sp.location+=dpos;
								sp.rotation+=drot;
								align(sp.location,m_move_align);
								align(sp.rotation,m_rotate_align);
								newCmd->AddCommand(actor, sp);
							}
						}
						if(m_shift_down)
						{
							m_shift_down=false;
							m_enable_shift_copy=false;
						}
					}
				}

				ActorMoved();
			}// end of if			

			m_cursorPos.X = x;
			m_cursorPos.Y = y;
		}
		
		if( newCmd->Execute() )
		{
			ActorMoved();
			newCmd->MouseMove = true;

			NEditorCommandManager^ cmdMgr = NEditorEngine::Instance()->CommandMgr;
			NEditorEngine::Instance()->CommandMgr->PushCommand(newCmd);

			//以下代码将来可能需要参考着重新实现，所以暂时保留

			////-- 如果上一个命令也是鼠标拖动，则更新上一个命令，而不是新加入一个命令
			//bool append = true;
			//NEditorCommandManager^ cmdMgr = NEditorEngine::Instance()->CommandMgr;
			//NEditorCommandBase^ lastCmd = cmdMgr->UndoStackTop;
			//if(lastCmd != nullptr && lastCmd->GetType() == NActorMoveCommand::typeid)
			//{
			//	NActorMoveCommand^ lastMoveCmd = safe_cast<NActorMoveCommand^>(lastCmd);
			//	if(lastMoveCmd->MouseMove
			//		&& lastMoveCmd->GetTargetActor()==newCmd->GetTargetActor())
			//	{
			//		lastMoveCmd->Update(newCmd->GetNewLoc());
			//		append = false;
			//	}
			//}
			//
			//if(append)
			//{
			//	NEditorEngine::Instance()->CommandMgr->PushCommand(newCmd);
			//}			
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
		m_actorHandles = new nexus::nactor_handles;
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
		m_actor_list->Clear();
		if(m_actor!=nullptr)
		{
			m_actor_list->Add(m_actor);
		}
		if( m_actor != nullptr
			&& m_actorHandles )
			m_actorHandles->set_actor_space( ac->NativePtr->get_space() );
	}

	void NActorEditorBase::AddSelectedActor( NActor^ actor )
	{
		if(actor==nullptr) return;
		if(m_actor_list->IndexOf(actor)==-1) 
		{
			m_actor_list->Add(actor);
		}
		m_actor=actor;

		if( m_actor != nullptr
			&& m_actorHandles )
			m_actorHandles->set_actor_space( m_actor->NativePtr->get_space() );
	}

	void NActorEditorBase::DeleteSelectedActors()
	{
		for(int i=0;i<m_actor_list->Count;++i)
		{
			if(m_actor_list[i]->Frozen) continue;
			m_level->DestroyActor(m_actor_list[i]->Name);
		}
		m_actor_list->Clear();
		m_actor=nullptr;
	}

	void NActorEditorBase::SelectSameTypeActors()
	{
		if(m_actor==nullptr) return;
		ActorList^ actors=gcnew ActorList();
		NActor^ actor=m_level->FirstActor();
		while(actor!=nullptr)
		{
			if(actor->Layer!=m_level->GetActiveLayer()) 
			{
				actor=m_level->NextActor();
				continue;
			}
			//检测是否引用了相同的模型
			if(!actor->IsSameTypeOf(m_actor)) 
			{
				actor=m_level->NextActor();
				continue;
			}
			actors->Add(actor);
			actor=m_level->NextActor();
		}
		BindActor(nullptr);
		for(int i=0;i<actors->Count;++i)
		{
			AddSelectedActor(actors[i]);
		}
	}

	void NActorEditorBase::SelectAllActorsInLayer()
	{
		BindActor(nullptr);
		NActor^ actor=m_level->FirstActor();
		while(actor!=nullptr)
		{
			if(actor->Layer!=m_level->GetActiveLayer()) 
			{
				actor=m_level->NextActor();
				continue;
			}
			AddSelectedActor(actor);
			actor=m_level->NextActor();
		}
	}

	void NActorEditorBase::SelectOtherActors()
	{
		ActorList^ actors=gcnew ActorList();
		NActor^ actor=m_level->FirstActor();
		while(actor!=nullptr)
		{
			if(actor->Layer!=m_level->GetActiveLayer()) 
			{
				actor=m_level->NextActor();
				continue;
			}
			if (m_actor_list->IndexOf(actor)!=-1) 	//是否已经被选中
			{
				actor=m_level->NextActor();
				continue;
			}
			actors->Add(actor);
			actor=m_level->NextActor();
		}
		BindActor(nullptr);
		for(int i=0;i<actors->Count;++i)
		{
			AddSelectedActor(actors[i]);
		}
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
		if(m_actor == nullptr
			|| m_actor->Destroyed)
			return;

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

	void NActorEditorBase::VisibleAllActors( bool v )
	{
		NActor^ actor=m_level->FirstActor();
		while(actor!=nullptr)
		{
			actor->Visible=v;
			actor=m_level->NextActor();
		}
		BindActor(nullptr);
	}

	void NActorEditorBase::VisibleSelectedActorsOnly()
	{
		NActor^ actor=m_level->FirstActor();
		while(actor!=nullptr)
		{
			bool v=(m_actor_list->IndexOf(actor)!=-1);
			actor->Visible=v;
			actor=m_level->NextActor();
		}
	}

	void NActorEditorBase::CopySelectedActors(bool cut)
	{
		m_copy_list->Clear();
		//记录要复制的actor，并且计算其中心
		Vector3 vec(0,0,0);
		for (int i=0;i<m_actor_list->Count;++i)
		{
			m_copy_list->Add(m_actor_list[i]->Name);
			vec+=m_actor_list[i]->Location;
		}
		vec/=(float)m_actor_list->Count;
		m_copy_center=vec;
		m_cut_mode=cut;
	}

	void NActorEditorBase::PasteCopyedActors(NCamera^ cam,int x,int y)
	{
		nexus::ray mray;
		cam->NativePtr->get_mouse_ray(x, y, mray);

		nexus::nlevel* lv = m_level->NativePtr;
		nexus::ncheck_result cr;

		float testDist = cam->NativePtr->get_zfar();
		if( lv->line_check(cr, mray.origin, mray.origin+mray.dir*testDist, ELCT_Terrain) )
		{
			BindActor(nullptr);
			Vector3 new_loc= Vector3::FromNative(cr.location);
			Vector3 vec=new_loc-m_copy_center;
			for(int i=0;i<m_copy_list->Count;++i)
			{
				NActor^ actor=m_level->FindActor(m_copy_list[i]);
				if(actor==nullptr || actor->Frozen || !actor->Serializable)
				{
					continue;
				}
				
				NActor^ new_actor=m_level->CloneActor(actor);
				if(new_actor==nullptr)
				{
					continue;
				}
				
				ObjectSpace sp = actor->SpaceData;
				sp.location+=vec;
				align(sp.location,m_move_align);
				new_actor->SpaceData=sp;
				AddSelectedActor(new_actor);
				if(m_cut_mode)
				{
					m_level->DestroyActor(m_copy_list[i]);
				}
			}
		}
		if(m_cut_mode)
		{
			m_copy_list->Clear();
		}
	}
}//namespace NexusEngine