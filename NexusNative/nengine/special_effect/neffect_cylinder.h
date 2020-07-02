#ifndef _NEXUS_EFFECT_CYLINDER_H_
#define _NEXUS_EFFECT_CYLINDER_H_
#include "neffect_element.h"
#include "ndistribution.h"
#include "neffect_vertex_data.h"
#include "../renderer/nrender_dynamic_mesh.h"


namespace nexus
{
	class neffect_cylinder;

	class neffect_cylinder_instance
		: public neffect_element_instance
	{
	public:
		neffect_cylinder_instance(void)	{}
		virtual ~neffect_cylinder_instance(void)	{}

		virtual void init(shared_ptr<neffect_element> def);
		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat);
		virtual nrender_mesh* get_render_mesh() const;

	private:
		shared_ptr<neffect_cylinder>			m_template;

		//-- 渲染数据
		neffect_vertex_data						m_vert_data;		
		render_res_ptr<nrender_dynamic_mesh>	m_mesh;
	};
	/**
	 *	特效元素：筒形几何体
	 *	@remarks 可以形成标准的筒，也可以产生多个圈，配合半径的变化产生带状、螺旋状集合体；或者五角星、六角星等；
	*/
	class nAPI neffect_cylinder
		: public neffect_element
	{
	public:
		typedef boost::shared_ptr<neffect_cylinder> ptr;

		neffect_cylinder(void);
		~neffect_cylinder(void);

		virtual void init(const nstring& name);

		virtual neffect_element_instance::ptr create_instance();
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		virtual void serialize(narchive& ar);
	public:

		//-- 静态数据
		int		m_div;		// 将圆周分割多少次
		vector2 m_uv_scale;	// 默认是将一个贴图完整的糊到筒子上

		//-- 变化数据
		ndistribution<float>::ptr	m_num_circle;				// 包含多少个完整的360度
		ndistribution<float>::ptr	m_height;					// 高度（垂直方向长度）
		ndistribution<float>::ptr	m_top_radius;				// 顶部半径
		ndistribution<float>::ptr	m_bottom_radius;			// 底部半径
		ndistribution<float>::ptr	m_div_top_radius_inc;		// 分割时顶半径增长速度
		ndistribution<float>::ptr	m_div_bottom_radius_inc;	// 分割时底半径增长速度
		ndistribution<float>::ptr	m_div_top_radius_shrink;	// 分割时，每隔一个点，顶半径收缩程度,用来生成五角星,六芒星等
		ndistribution<float>::ptr	m_div_bottom_radius_shrink;
		ndistribution<float>::ptr	m_div_top_rise;				// 每次分割顶面高度上升速度
		ndistribution<float>::ptr	m_div_bottom_rise;			// 每次分割底面高度上升速度
		ndistribution<color4f>::ptr	m_top_color;				// 顶部顶点颜色
		ndistribution<color4f>::ptr	m_bottom_color;				// 底部顶点颜色

		nDECLARE_CLASS(neffect_cylinder)
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_CYLINDER_H_