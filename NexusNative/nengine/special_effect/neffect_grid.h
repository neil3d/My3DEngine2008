#ifndef _NEXUS_EFFECT_GRID_H_
#define _NEXUS_EFFECT_GRID_H_
#pragma once
#include "neffect_element.h"
#include "quad_face.h"
#include "neffect_vertex_data.h"
#include "../renderer/nrender_dynamic_mesh.h"

namespace nexus
{
	//飘带特效
	//可控元素：飘带节点的轨迹、飘带受重力的影响、飘带受风力的影响
	//-----------------------------------------------------------------------------------
	//飘带面片的类型
	enum ESFX_GridType
	{
		neffect_grid_plane,				//平面
		neffect_grid_sphere_billboard,	//球形公告板
		neffent_grid_cylinder_billboard,	//圆柱公告板
	};

	class neffect_grid_instance:
		public neffect_element_instance
	{
		friend class neffect_grid;
	public:
		typedef boost::shared_ptr<neffect_grid_instance> ptr;

		virtual void init(boost::shared_ptr<neffect_element> def);

		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat);

		virtual nrender_mesh* get_render_mesh() const
		{
			return m_mesh.get();
		}

		void compute_vertex_info(int x,int y,float time,vector3& pos);
	protected:
		vector2	m_size;	//当前尺寸
		color4f	m_color;//当前色彩
		vector3	m_power;//当前振幅
		//-----------------------------------------------------------------------------------
		vector3	m_view_forward;
		vector3 m_view_up;
		vector3 m_view_right;

		matrix44 m_transform;
		//-----------------------------------------------------------------------------------
		boost::shared_ptr<neffect_grid>		m_template;
		int	m_face_count;	//产生的三角形的数量
		//-- 渲染数据
		neffect_vertex_data		m_vert_data;		
		render_res_ptr<nrender_dynamic_mesh>	m_mesh;		
	public:
		neffect_grid_instance();
		virtual ~neffect_grid_instance(){};
	};

	//-----------------------------------------------------------------------------------
	class nAPI neffect_grid:
		public neffect_element
	{
	public:
		typedef boost::shared_ptr<neffect_grid> ptr;
		virtual void init(const nstring& name);
		virtual neffect_element_instance::ptr create_instance();
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		virtual void serialize(narchive& ar);
	public:
		//飘带的类型
		ESFX_GridType	m_type;
		//飘带沿长度方向的细分数量
		int		m_length_div;
		//飘带沿宽度方向的细分数量
		int		m_width_div;
		//波动的传播速度
		float	m_wave_speed;
		//波动在各个方向的振幅
		ndistribution<vector3>::ptr		m_wave_power;
		//波动的衰减速度
		float	m_attenuation;
		//飘带的色彩随时间改变的控制
		ndistribution<color4f>::ptr		m_color;
		//飘带的尺寸随时间改变的控制
		ndistribution<vector2>::ptr		m_size;
	public:
		neffect_grid();
		~neffect_grid();
		nDECLARE_CLASS(neffect_grid)
	};
};
#endif