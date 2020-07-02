#ifndef _NEXUS_TERRAIN_DECO_SETUP_H_
#define _NEXUS_TERRAIN_DECO_SETUP_H_

#include "ncore.h"
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../renderer/nrender_resource.h"
#include "../framework/world_define.h"

namespace nexus
{
	struct terrain_deco_object
	{
		//实例的位置
		vector3	m_pos;
		//实例的旋转
		vector3 m_rotation;
		//实例的缩放
		vector3 m_scaline;
		terrain_deco_object():m_pos(0,0,0),m_rotation(0,0,0),m_scaline(1.0f,1.0f,1.0f)
		{}
		void	ToMatrix(matrix44& matrix)
		{
			mat_set_scaling(matrix,m_scaline.x,m_scaline.y,m_scaline.z);
			mat_rotate_Z(matrix, m_rotation.z);
			mat_rotate_X(matrix, m_rotation.x);
			mat_rotate_Y(matrix, m_rotation.y);
			mat_translate(matrix,m_pos);
		}
	};

	class nAPI nterrain_deco_layer : public nobject
	{
	public:
		typedef shared_ptr<nterrain_deco_layer> ptr;
		//===================================================================================
		//执行一次笔刷，根据笔刷和自身数据向场景中添加actor.参数:/笔刷中心x/笔刷中心y/笔刷的半径/
		void	do_brush(float x,float y,float radius);
		//===================================================================================
		//获取和设置层的名字
		const	nstring&	get_name()			{return m_name;}
		void	set_name(const nstring& value)	{m_name=value;}

		//获取和设置当前层的模型资源。如果设置了不同的资源，则会更新现有的对象模型
		const	resource_location&	get_resource_loc()	{return m_res;}
		void	set_resource_loc(const resource_location& loc);

		//获取和设置默认的随机旋转值，该值代表对象创建时绕Y轴的随机旋转.修改该值不影响已有的对象
		float	get_random_rotation()	{return m_random_rotation;}
		void	set_random_rotation(float value)	{m_random_rotation=value;}

		//获取和设置随机缩放的最大值和最小值，修改这些值不影响已有的对象
		float	get_max_random_scale()	{return m_max_random_scale;}
		void	set_max_random_scale(float value)	{m_max_random_scale=value;}

		float	get_min_random_scale()	{return m_min_random_scale;}
		void	set_min_random_scale(float value)	{m_min_random_scale=value;}

		//获取和设置笔刷的密度,修改该值不影响已有的对象
		float	get_brush_density()	{return m_density;}
		void	set_brush_density(float value)	{m_density=value;}

		//获取和设置随机数种子,修改该值不影响已有的对象
		float	get_random_seed()	{return m_random_seed;}
		void	set_random_seed(float value)	{m_random_seed=value;}

	private:

		//层的名字
		nstring		m_name;

		//该层使用的模型
		resource_location	m_res;

		//默认的随机旋转值界限,如果取值为30，代表在-30~30度之间旋转。注意，这里使用的是度，不是弧度。
		float	m_random_rotation;

		//默认的随机缩放值
		float	m_min_random_scale;
		float	m_max_random_scale;

		//笔刷的密度
		float	m_density;

		//实例间距，相邻两个实例之间的基本间距，计算时以实例的中心轴距离为准，因此实例之间仍可能发生碰撞
		float	m_grid_size;

		//随机数种子
		int		m_random_seed;

		//该层所有实例列表
		vector<terrain_deco_object>	m_object_list;

		//每个实例对应的actor
		vector<nactor::ptr>	m_actor_list;

	public:
		nDECLARE_CLASS(nterrain_deco_layer);
		nterrain_deco_layer(void);
		virtual ~nterrain_deco_layer(void);
	};
}//namespace nexus
#endif //_NEXUS_TERRAIN_DECO_SETUP_H_