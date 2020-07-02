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

namespace NexusEngine
{
	public enum class ETrnLerpMode : System::Int32
	{
		None	= ELM_None,
		Linear	= ELM_Linear,
		Cosine	= ELM_Cosine,
		Quadratic = ELM_Quadratic,
	};

	public enum class ETrnBrushShape : System::Int32
	{
		Circle = EBS_Circle,
		Square = EBS_Square,
	};

	public enum class ETrnBurshTarget : System::Int32
	{
		Heightmap = EBT_Heightmap,
		Alphamap = EBT_Alphamap,
		Decomap = EBT_Decomap
	};

	public enum class ETrnBrushOP : System::Int32
	{
		Paint	= EOP_Paint,		
		Flatten	= EOP_Flatten,
		Smooth	= EOP_Smooth,
		Apply	= EOP_Apply,
		Noise	= EOP_Noise,
	};

	public value struct NTerrainBrush
	{
		ETrnBrushOP		op;
		ETrnBrushShape	shape;
		ETrnLerpMode	lerpMode;
		int				innerRadius;
		int				outerRadius;
		int				strength;
		int				noiseFreq;
		ETrnBurshTarget	target;
		int				targetIndex;

		void ToNative(nexus::nterrain_brush& brush);
	};

	//! 地形编辑器, 内部使用Native代码实现
	public ref class NTerrainEditor
	{
	public:
		literal UInt32 MaxAphaLayers = 4;
		literal UInt32 MaxLayers = 5;

		System::EventHandler^ TerranActorBinded;

		NTerrainEditor(void);
		~NTerrainEditor(void);

		//!	指定当前操作的Terrain对象
		void BindTerrain(NTerrainActor^ trn);

		//!	是否绑定了一个有效的Terrain对象
		bool Empty();

		//! 关闭, 执行清理工作
		void Close();

		//! 设定Brush对象
		void SetBrush(NTerrainBrush brush);

		//!	在高度图指定的区域内产生随机地形高度
		void GenerateNoise(System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		//! 导入磁盘中指定的图像文件作为高度图
		void ImportHeightmap(System::String^ imgFileName);

		//!	创建基本类型的材质
		void CreateMaterialBasic(NResourceLoc^ textureLoc);

		//!	创建多图层混合的材质
		void CreateTextureSplatting(int alphaW, int alphaH);

		//!	设定材质图层
		void SplatSetLayer(UInt32 layerIndex, NResourceLoc^ textureLoc, Vector2 uvScale, float uvRotate);

		//!	在某个图层上产生随机的Alpha
		void SplatLayerNoise(UInt32 layerIndex, System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency);

		//!	取得某个图层的数据
		void SplatGetLayer(UInt32 layerIndex, NResourceLoc% outTex, Vector2% outScale, float% outRotate);		

		/** 
		 *	处理Windows窗口操作消息
		 *	@return 返回是否处理了这个消息
		*/
		bool InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam);

		//! 绘制地形光标等界面元素
		void DrawWidgets(NRenderPrimitiveDrawInterface^ PDI);
	private:
		nterrain_editor*	m_nativeEditor;
	};
}//namespace NexusEngine