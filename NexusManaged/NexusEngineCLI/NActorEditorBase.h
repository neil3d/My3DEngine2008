#pragma once
#include "NLevel.h"
#include "NActor.h"
#include "NRenderElement.h"
#include "math/BoundingBox.h"
#include "EditorCommand.h"
#include "nactor_handles.h"

namespace NexusEngine
{
	public enum class EActorEditHandle
	{		
		MoveX = 0,
		MoveY,
		MoveZ,
		RotateX,
		RotateY,
		RotateZ,

		Invalid = 255,
	};

	public ref class NActorEditorBase
	{
	public:
		NActorEditorBase(void);
		virtual ~NActorEditorBase(void);

		void Init();
		void Close();

		void SetMainLevel(NLevel^ lv)	{	m_level = lv;}
		virtual void BindActor(NActor^ ac);
		NActor^ GetActor()				{	return m_actor;}
		
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam, EditorCommand cmd);
		
		virtual void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);
		void DrawActorHandlesHitProxy(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy);

		void SetHotActorHandle(EActorEditHandle h);
		void UpdateView(NCamera^ cam);

		int FindActorHandle(hit_id hid);
		bool IsLeftDraging() {	return m_leftDrag;}

	protected:
		virtual void OnMouseMove(int x, int y, bool ctrlDown, NCamera^ cam);
		virtual void OnMouseLButtonDown(int x, int y, NCamera^ cam, EditorCommand cmd);
		virtual void OnMouseLButtonUp();

		void PlaceActorToGround();
		void ActorMoved();
		void DrawActorHandles(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy);
	
	protected:
		NLevel^			m_level;	//ÕýÔÚ±à¼­µÄLevel
		NActor^			m_actor;		

		//-- mouse
		bool	m_leftDrag;
		System::Drawing::Point	m_cursorPos;
		float	m_moveFactor;
		float	m_rotateFactor;

	private:
		EActorEditHandle	m_hotHandle;
		nactor_handles*		m_actorHandles;
	};
}//namespace NexusEngine