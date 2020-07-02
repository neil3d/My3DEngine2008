/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NTerrainActor.h"
#include "NRenderElement.h"
#include "NResource.h"
#include "NTerrainBrush.h"
#include "NTerrainMtlSetup.h"

namespace NexusEngine
{
	using namespace System::ComponentModel;

	//! 地形编辑器, 内部使用Native代码实现
	public ref class NTerrainEditor
	{
	public:
		literal UInt32 MaxAphaLayers = 4;
		literal UInt32 MaxLayers = 5;

		System::EventHandler^ TerranActorBinded;

		NTerrainEditor(void);
		~NTerrainEditor(void);

		NTerrainMtlSetup^ GetMaterial();

		//!	指定当前操作的Terrain对象
		void BindTerrain(NTerrainActor^ trn);

		//!	是否绑定了一个有效的Terrain对象
		bool Empty();

		void ShowChunkEdge(bool s);

		//! 关闭, 执行清理工作
		void Close();

		//! 设定Brush对象
		void SetBrush(NTerrainBrush brush);

		//!	在高度图指定的区域内产生随机地形高度
		void GenerateNoise(System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		//! 导入磁盘中指定的图像文件作为高度图
		void ImportHeightmap(System::String^ imgFileName);

		/** 
		 *	处理Windows窗口操作消息
		 *	@return 返回是否处理了这个消息
		*/
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam);

		//! 绘制地形光标等界面元素
		void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);

		NTerrainActor^ GetTerrainBinded()	{	return m_trn;}
	private:
		nterrain_editor*	m_nativeEditor;
		NTerrainActor^		m_trn;
	};
}//namespace NexusEngine