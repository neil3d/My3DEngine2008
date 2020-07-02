/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeObject.h"
#include "NActor.h"
#include "NMath.h"
#include "NResource.h"

namespace NexusEngine
{
	//!	射线检测的结果
	public value struct NCheckResult
	{
		Vector3	location;
		Vector3	normal;
		float	dist;
	};

	[System::Flags]
	public enum class LineCheckType  : System::Int32
	{
		Static		= ELCT_Static,	
		Dynamic		= ELCT_Dynamic,

		Terrain		= ELCT_Terrain,
		Navigation	= ELCT_Navigation,

		World		= ELCT_World,

		All			= ELCT_All
	};

	public ref class RenderSetting
	{
	public:
		RenderSetting(render_setting* ns)
		{
			m_NativePtr=ns;
		}

		property Color4f SkyLightHightColor
		{
			Color4f get()
			{
				return Color4f::FromNative(m_NativePtr->skylight_high_color);
			}
			void	set(Color4f value)
			{
				m_NativePtr->skylight_high_color=value.ToNative();
			}
		}

		property Color4f SkyLightLowColor
		{
			Color4f get()
			{
				return Color4f::FromNative(m_NativePtr->skylight_low_color);
			}
			void	set(Color4f value)
			{
				m_NativePtr->skylight_low_color=value.ToNative();
			}
		}

		property float SSAOOcclustionRadius
		{
			float get()
			{
				return m_NativePtr->ssao_occlustion_radius;
			}
			void	set(float value)
			{
				m_NativePtr->ssao_occlustion_radius=value;
			}
		}

		property float SSAOOcclustionPower
		{
			float get()
			{
				return m_NativePtr->ssao_occlustion_power;
			}
			void	set(float value)
			{
				m_NativePtr->ssao_occlustion_power=value;
			}
		}

		property float SSAOBlockerPower
		{
			float get()
			{
				return m_NativePtr->ssao_blocker_power;
			}
			void	set(float value)
			{
				m_NativePtr->ssao_blocker_power=value;
			}
		}

	protected:
		render_setting*	m_NativePtr;		
	};

	//!	包装nexus::nlevel对象
	public ref class NLevel : public NativeObjectOwnedBySharedPtr
	{
	public:				
		//!	初始化, 创建内部对象
		virtual void Init();

		//!	创建一个Actor, Actor Name要求在关卡中是唯一的
		virtual NActor^ CreateActor(System::String^ actorName, System::String^ nativeClassName);

		//!	销毁指定名称的Actor
		virtual void DestroyActor(System::String^ actorName);

		//!	根据名称查找Actor
		NActor^ FindActor(System::String^ actorName);

		//! 克隆指定的Actor
		NActor^ CloneActor(NActor^ actor);

		//!	射线检测
		bool LineCheck([Out] NCheckResult% ret, Vector3 start, Vector3 end, LineCheckType checkType);
		
		//! 使用导航图的检测ref navigation_map::line_of_sight
		bool LineOfSight([Out] NCheckResult% ret, Vector3 start, Vector3 end, bool checkWalkableMap);

		//!	清空关卡中的所有Actor
		virtual void DestroyAllActors();
		
		//!	取得关卡的名称
		property System::String^ Name
		{
			System::String^ get();
		}

		NActor^ FirstActor();
		NActor^ NextActor();

		void RenameActor(String^ oldName, String^ newName);

		//! 把一个已经创建好的NActor添加到关卡中
		void AttachActor(NActor^ actor);
		//! 把一个已经创建好的NActor从关卡中移除，但是并不删除该NActor
		bool DeattachActor(NActor^ actor);

		//! 分层编辑相关接口
		bool	IsLayerExist(String^ layerName);
		int		GetLayerCount();
		String^ GetLayerName(int index);
		void	CreateNewLayer(String^ layerName);
		void	RemoveLayer(String^ layerName);
		//重命名层
		void	RenameLayer(String^ oldlayername,String^ newLayerName);
		//设置当前被激活的层
		void	SetActiveLayer(String^ layerName);
		//获取当前被激活的层的名字
		String^ GetActiveLayer();
		//锁定或者解锁指定名字的层
		void	LockLayer(String^ layerName,bool lock);
		//指定名字的层是否被锁定
		bool	IsLayerLocked(String^ layer_name);
		//显示或者隐藏指定的层
		void	ShowLayer(String^ layerName,bool show);
		//是否显示指定层
		bool	IsLayerVisible(String^ layerName);
		//将指定名字的对象移动到当前激活的层
		void	MoveActorToActiveLayer(String^ actorName);

		// 导航图可行走区域相关接口
		bool GetWorldWalkable(float wx, float wz);
		bool GetTileWalkable(int tx, int tz);
		// 导航图高度图接口
		float GetWorldHeight(float wx, float wz);
		float GetTileHeight(int tx, int tz);

		//-- 导出Level，对Native function export_level的一个封装
		virtual int ExportLevel(NResourceLoc^ location);

		//获取当前Level的RenderSetting对象
		virtual RenderSetting^	GetRenderSetting()
		{
			return m_RenderSetting;
		}

		//天空相关接口
		/*void	SetSky(NResourceLoc loc)
		{
			resource_location nloc;
			loc.ToNative(nloc);
			NativePtr->set_sky(nloc);
		}
		void	RemoveSky()
		{
			NativePtr->remove_sky();
		}
		NResourceLoc	GetSky()
		{
			return NResourceLoc(NativePtr->get_sky());
		}*/
	public:
		//-- 包装相关接口 --
		NLevel(nexus::nlevel::ptr nativeLv);
		static NLevel^ FromNativePtr(nexus::nlevel::ptr nativeLv, System::String^ nativeClassName);
		nexus::nlevel::ptr GetNativeSmartPtr();

		void NativeLevelLoaded();
	
		property nexus::nlevel* NativePtr
		{
			nexus::nlevel* get();
		}

	public:
		// 导航图相关接口
		//-- 导航图宽度
		property UInt32 NavigationMapWidth
		{
			UInt32 get()
			{
				return NativePtr->get_navigation_map()->get_raw_width();
			}
		}

		//-- 导航图高度
		property UInt32 NavigationMapHeight
		{
			UInt32 get()
			{
				return NativePtr->get_navigation_map()->get_raw_height();
			}
		}

		//-- 导航图起始位置
		property Vector3 NavigationMapLocation
		{
			Vector3 get()
			{
				const nexus::vector3& nloc = NativePtr->get_navigation_map()->get_location();
				return Vector3(nloc.x, nloc.y, nloc.z);
			}
		}

		//-- 导航图缩放
		property Vector3 NavigationMapScale
		{
			Vector3 get()
			{
				const nexus::vector3& scale = NativePtr->get_navigation_map()->get_scale();
				return Vector3(scale.x, scale.y, scale.z);
			}
		}

	private:
		typedef System::Collections::Generic::Dictionary<System::String^, NActor^> ActorDict;
		
		ActorDict::Enumerator^	m_actorEnum;
		ActorDict^				m_actorDict;

		RenderSetting^			m_RenderSetting;
	};
}//namespace NexusEngine