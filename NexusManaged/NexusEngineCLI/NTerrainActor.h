/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NActor.h"
#include "NMath.h"
#include "NTerrainMtlSetup.h"
#include "NTerrainDecoSetup.h"

namespace NexusEngine
{
	/**
	 *	地形对象, 包装nexus::nterrain_actor
	*/
	public ref class NTerrainActor : public NActor
	{
	public:
		//!	创建高度图, 以及内部渲染对象等
		void Create(UInt32 w, UInt32 h, UInt16 initH, UInt32 chunkSize);

		//!	设置位置和Scale, 不支持旋转
		void Move(Vector3 pos, Vector3 scale);

		//!
		void GetWorldHeight(float worldX, float worldZ);
		void GetTileHeight(int tileX, int tileZ);

		//! 高度图宽度和高度
		property UInt32 HeightMapWidth
		{
			UInt32 get()
			{
				return NativePtr->get_map_width();
			}
		}

		property UInt32 HeightMapHeight
		{
			UInt32 get()
			{
				return NativePtr->get_map_height();
			}
		}

		NTerrainMtlSetup^ GetMaterial()	{	return m_mtlSetup; }

		NTerrainDecoSetup^ GetDeco()	{return m_DecoSetup;}

	private:
		NTerrainMtlSetup^	m_mtlSetup;
		NTerrainDecoSetup^	m_DecoSetup;
		//-- Native Wratoer
	public:
		NTerrainActor(nexus::nterrain_actor::ptr nativeTrn);		
		nterrain_actor::ptr GetSmartPtr();		
	protected:
		property nterrain_actor* NativePtr
		{
			nterrain_actor* get();
		}		
	};
}//namespace NexusEngine