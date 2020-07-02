#pragma once
#include "NTerrainActor.h"
#include "NRenderElement.h"
#include "NResource.h"
#include "NTerrainBrush.h"

namespace NexusEngine
{
	using namespace System::ComponentModel;
	
	/**
	* 导航图编辑器,对nav_map_editor的包装，尝试过和地形编辑器放在一起，不过感觉很混乱
	* 和地形编辑器分开会有部分重复代码，但是逻辑会清晰很多
	*/
	public ref class NavigateMapEditor
	{
	public:
		NavigateMapEditor(void);
		virtual ~NavigateMapEditor(void);

		// 显示或者隐藏导航图
		property bool NavigationMapVisible
		{
			bool get() { return m_nav_map_editor->get_nav_map_visible(); }
			void set(bool value) 
			{ 
				m_nav_map_editor->set_nav_map_visible(value); 
			}
		}

		// 显示或者隐藏可行走区域位图
		property bool WalkableMapVisible
		{
			bool get() { return m_nav_map_editor->get_walkable_map_visibale();; }
			void set(bool value) 
			{ 
				m_nav_map_editor->set_walkable_map_visibale(value); 
			}
		}

		// 高度图颜色
		property Color4f NavigationHeightMapColor
		{
			Color4f get() { return Color4f::FromNative(m_nav_map_editor->get_height_nav_color()); }
			void set(Color4f value) 
			{ m_nav_map_editor->set_height_nav_color(value.ToNative()); }
		}

		// 可行走区域颜色
		property Color4f NavigationWalkableColor
		{
			Color4f get() { return Color4f::FromNative(m_nav_map_editor->get_walkable_nav_color()); }
			void set(Color4f value) 
			{ 
				m_nav_map_editor->set_walkable_nav_color(value.ToNative());
			}
		}

		property float CullDistance
		{
			float get() { return m_nav_map_editor->get_cull_dist(); }
			void set(float value) { m_nav_map_editor->set_cull_dist(value); }
		}

		// 开始导航图编辑，创建nav_map_actor
		void BeginNavigateMapEditor(NLevel^ level);
		// 开始导航图编辑，隐藏nav_map_actor
		void EndNavigateMapEditor();

		//!	是否绑定了一个有效的nav_map_actor对象
		bool Empty();

		//! 关闭, 执行清理工作
		void Close();

		//! 设定Brush对象
		void SetBrush(NTerrainBrush brush);

		//! 导入磁盘中指定的图像文件作为高度图
		void ImportHeightmap(System::String^ imgFileName);

		//-- 导航图接口
		void MoveNavigationMap(Vector3 loc, Vector3 scale);
		void ResizeNavigationMap(UInt32 width, UInt32 heigth);
		void ResetNavigationMap(UInt32 width, UInt32 heigth);
		// 重新生成导航图
		void RebuildNavigationMap(float maxHeight, float stepHegiht, bool use_line_check);

		/** 
		*	处理Windows窗口操作消息
		*	@return 返回是否处理了这个消息
		*/
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam);

		//! 绘制笔刷光标等界面元素
		void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);

	private:
		// native实现
		nav_map_editor* m_nav_map_editor;
	};
}
