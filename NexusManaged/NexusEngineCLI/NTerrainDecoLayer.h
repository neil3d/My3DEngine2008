#pragma once
#include "NativeObject.h"
#include "NResource.h"
#include "math/Vector3.h"
namespace NexusEngine
{
	ref class NTerrainDecoSetup;

	public ref class NTerrainDecoLayer : public NativeObjectOwnedBySharedPtr
	{
	public:
		[Category("属性"), Description("装饰层名称")]
		property String^ Name
		{
			String^ get()
			{
				return gcnew String(NativePtr->get_name().c_str());
			}
			void set(String^ name);
		}

		[Category("属性"), Description("装饰层引用的资源，对草叶装饰层来说，必须是材质资源；对静态模型装饰层来说，必须是静态模型资源")]
		property NResourceLoc ResourceLoc
		{
			NResourceLoc	get()	
			{
				return NResourceLoc(NativePtr->get_resource_loc());
			}
			void	set(NResourceLoc loc)
			{
				resource_location _loc;
				loc.ToNative(_loc);
				NativePtr->set_resource_loc(_loc);
			}
		}

		[Category("属性"), Description("装饰层中每一个物品的随机坐标偏移")]
		property float RandomOffset
		{
			float	get()	
			{
				return NativePtr->get_random_offset();
			}
			void	set(float v)
			{
				NativePtr->set_random_offset(v);
			}
		}

		[Category("属性"), Description("装饰层中每一个物品的在高度上的偏移，负值表示向下")]
		property float HeightOffset
		{
			float	get()	
			{
				return NativePtr->get_height_offset();
			}
			void	set(float v)
			{
				NativePtr->set_height_offset(v);
			}
		}

		[Category("属性"), Description("装饰层中每一个物品的随机旋转，取值范围0~180度，表示在-180度~180度之间取随机值")]
		property float RandomRotation
		{
			float	get()	
			{
				return NativePtr->get_random_rotation();
			}
			void	set(float v)
			{
				NativePtr->set_random_rotation(v);
			}
		}

		[Category("属性"), Description("装饰层每一个物品的最大缩放值，物品实际的缩放值将在最大缩放值和最小缩放值中随机选择")]
		property float MaxRandomScale
		{
			float	get()	
			{
				return NativePtr->get_max_random_scale();
			}
			void	set(float v)
			{
				NativePtr->set_max_random_scale(v);
			}
		}

		[Category("属性"), Description("装饰层每一个物品的最大缩放值，物品实际的缩放值将在最大缩放值和最小缩放值中随机选择")]
		property float MinRandomScale
		{
			float	get()	
			{
				return NativePtr->get_min_random_scale();
			}
			void	set(float v)
			{
				NativePtr->set_min_random_scale(v);
			}
		}

		[Category("属性"), Description("装饰层中每一个网格里面产生的物品的数量")]
		property float Density
		{
			float	get()	
			{
				return NativePtr->get_density();
			}
			void	set(float v)
			{
				NativePtr->set_density(v);
			}
		}

		[Category("属性"), Description("装饰层中网格的数量")]
		property int GridCount
		{
			int	get()	
			{
				return NativePtr->get_grid_size();
			}
		}


		[Category("属性"), Description("随机数种子")]
		property int RandomSeed
		{
			int	get()	
			{
				return NativePtr->get_random_seed();
			}
			void	set(int v)
			{
				NativePtr->set_random_seed(v);
			}
		}

	public:
		NTerrainDecoLayer(nexus::nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner);
	protected:
		NTerrainDecoSetup^	m_Owner;

		property nterrain_deco_layer* NativePtr
		{
			nterrain_deco_layer* get()
			{
				return dynamic_cast<nterrain_deco_layer*>(RawPtr);
			}
		}
	};

	public ref class NTerrainStaticMeshDecoLayer : public NTerrainDecoLayer
	{
	public:
		NTerrainStaticMeshDecoLayer(nexus::nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner);
	};

	public ref class NTerrainGrassDecoLayer : public NTerrainDecoLayer
	{
	public:
		NTerrainGrassDecoLayer(nterrain_deco_layer::ptr layer,NTerrainDecoSetup^ owner);

		[Category("属性"), Description("每一片草叶的宽度")]
		property float Width
		{
			float	get()	
			{
				return NativePtr->get_width();
			}
			void	set(float v)
			{
				NativePtr->set_width(v);
			}
		}

		[Category("属性"), Description("每一片草叶的高度")]
		property float Height
		{
			float	get()	
			{
				return NativePtr->get_height();
			}
			void	set(float v)
			{
				NativePtr->set_height(v);
			}
		}

		[Category("属性"), Description("是否使用十字形草叶面片")]
		property bool CrossShape
		{
			bool	get()	
			{
				return NativePtr->get_cross();
			}
			void	set(bool v)
			{
				NativePtr->set_cross(v);
			}
		}
	protected:
		property nterrain_grass_deco_layer* NativePtr
		{
			nterrain_grass_deco_layer* get()
			{
				return dynamic_cast<nterrain_grass_deco_layer*>(RawPtr);
			}
		}
	};
};