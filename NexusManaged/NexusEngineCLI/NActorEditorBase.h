#pragma once
#include "NLevel.h"
#include "NActor.h"
#include "NRenderElement.h"
#include "math/BoundingBox.h"
#include "EditorCommand.h"
#include "../NexusNative/nengine/editor/nactor_handles.h"

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

	typedef System::Collections::Generic::List<NActor^> ActorList;
	public ref class NActorEditorBase
	{		
	public:
		NActorEditorBase(void);
		virtual ~NActorEditorBase(void);

		void Init();
		void Close();

		void				SetMainLevel(NLevel^ lv)			{	m_level = lv;	}

		// 将指定的actor添加到被选中actor列表
		virtual void		AddSelectedActor(NActor^ actor);

		// 将指定actor设置为唯一被选中的actor，如果actor为null，则清空所有选择
		virtual void		BindActor(NActor^ actor);

		// 获取当前拥有操作手柄的被选中actor
		virtual NActor^		GetActor()							{	return m_actor;	}

		// 获取被选中的actor列表
		virtual ActorList^	GetSelectedActors()					{	return m_actor_list;	}

		// 删除所有被选中的actor
		virtual void		DeleteSelectedActors();

		// 删除与拥有操作手柄的actor相同类型的actor，该功能目前尚未完善，因为还没能有效的判断两个actor是否是相同类型
		virtual void		SelectSameTypeActors();

		// 选中当前激活层内的所有actor
		virtual void		SelectAllActorsInLayer();

		// 反选当前层内的actor
		virtual void		SelectOtherActors();

		// 显示或者隐藏所有的actor
		virtual void		VisibleAllActors(bool v);

		// 隐藏没有被选中的所有actor
		virtual void		VisibleSelectedActorsOnly();

		// 复制选中的actor，参数：是否剪切
		virtual void		CopySelectedActors(bool cut);

		// 粘贴被复制的所有actor到屏幕的指定坐标处
		virtual void		PasteCopyedActors(NCamera^ cam,int x,int y);

		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam, EditorCommand cmd);
		
		virtual void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);
		void DrawActorHandlesHitProxy(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy);

		void SetHotActorHandle(EActorEditHandle h);
		void UpdateView(NCamera^ cam);

		int FindActorHandle(hit_id hid);
		bool IsLeftDraging() {	return m_leftDrag;}

		void ActorMoved();

		property float MouseMoveFactor
		{
			float get()	{	return m_moveFactor;}
			void set(float val)	{	m_moveFactor = val;}
		}

		property float MouseRotateFactor
		{
			float get()	{	return m_rotateFactor;}
			void set(float val)	{	m_rotateFactor = val;}
		}

		property float MoveAlign
		{
			float get()	{	return m_move_align;}
			void set(float val)	{	m_move_align = val>0?val:1.0f;}
		}

		property float RotateAlign
		{
			float get()	{	return m_rotate_align;}
			void set(float val)	{	m_rotate_align = val>0?val:0.001f;}
		}

		property float ScaleAlign
		{
			float get()	{	return m_scale_align;}
			void set(float val)	{	m_scale_align = val>0?val:0.001f;}
		}
	protected:
		virtual void OnMouseMove(int x, int y, bool ctrlDown, NCamera^ cam);
		virtual void OnMouseLButtonDown(int x, int y, NCamera^ cam, EditorCommand cmd);
		virtual void OnMouseLButtonUp();

		void PlaceActorToGround();		
		void DrawActorHandles(NRenderPrimitiveDrawInterface^ PDI, bool hitProxy);
	
	protected:
		NLevel^			m_level;	//正在编辑的Level
		NActor^			m_actor;		
		ActorList^		m_actor_list;

		//复制-剪切-粘贴相关变量
		System::Collections::Generic::List<String^>^		m_copy_list;	//要复制或者剪切的actor列表
		Vector3			m_copy_center;	//actors的中心
		bool			m_cut_mode;		//是否是剪切模式，如果是，则在粘贴后自动删除原来的物体

		//-- mouse
		bool	m_leftDrag;
		System::Drawing::Point	m_cursorPos;
		float	m_moveFactor;
		float	m_rotateFactor;
		//-----------------------------------------------------------------------------------
		//位置对齐
		float	m_move_align;
		float	m_rotate_align;
		float	m_scale_align;

		Vector3	m_move_states;
		Vector3 m_rotate_states;

		bool	m_shift_down;
		bool	m_enable_shift_copy;

	private:
		EActorEditHandle	m_hotHandle;
		nexus::nactor_handles*		m_actorHandles;
	};

	//虽然这里可以使用模板，但是考虑到对float和vector3都需要特化，并且目前只需要用到float和vector3，所以直接使用函数重载较为妥当。
	/*template<typename T>
	void inline align(T& value,T align_size)
	{}*/

	void inline align(float& value,float align_size)
	{
		float mod=fmodf(value,align_size);
		value=(mod>=align_size*0.5f)?(value+align_size-mod):(value-mod);	//利用四舍五入实现“吸引”效果
	}

	void inline align(Vector3& vec,float align_size)
	{
		align(vec.x,align_size);
		align(vec.y,align_size);
		align(vec.z,align_size);
	}

}//namespace NexusEngine