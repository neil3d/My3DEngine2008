#ifndef _NEXUS_EFFECT_QUAD_H_
#define _NEXUS_EFFECT_QUAD_H_
#include "neffect_element.h"
#include "quad_face.h"
#include "neffect_vertex_data.h"
#include "../renderer/nrender_dynamic_mesh.h"

namespace nexus
{
	class neffect_quad;

	class neffect_quad_instance
		: public neffect_element_instance
	{
	public:
		typedef boost::shared_ptr<neffect_quad_instance> ptr;

		neffect_quad_instance(void):m_active_face(1),
			m_tail_delay(0),m_tail_index(0),m_tail_fade_speed(1)
		{}
		virtual ~neffect_quad_instance(void)	{}

		virtual void init(shared_ptr<neffect_element> def);		
		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat);

		virtual nrender_mesh* get_render_mesh() const
		{
			return m_mesh.get();
		}

		st_quad_array& get_faces()	{	return m_faces;}
		
		void update_tail(neffect_quad* def, float delta_time);
	private:
		ESFX_FaceType			m_face_type;
		neffect_vertex_data		m_vert_data;		
		
		st_quad_array			m_faces;
		unsigned int			m_active_face;	// 有几个面是有效的？

		float	m_tail_delay;	// 控制产生拖尾片的间隔时间
		unsigned int m_tail_index;	// 最后一个尾巴quad在数组中的下标
		float	m_tail_fade_speed; // 拖尾片alpha降低的速度

		render_res_ptr<nrender_dynamic_mesh> m_mesh;
	};

	/**
	 *	特效元素：片
	 *	@remarks 可以做普通片，billborad等；自带残影功能。
	*/
	class nAPI neffect_quad :
		public neffect_element
	{
	public:
		typedef boost::shared_ptr<neffect_quad> ptr;

		neffect_quad(void);
		virtual ~neffect_quad(void);

		virtual void init(const nstring& name);
		virtual neffect_element_instance::ptr create_instance();
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		virtual void serialize(narchive& ar);
	public:		
		//-- 静态数据
		ESFX_FaceType	m_face_type;
		int				m_num_tail_faces;	// 总共产生多少个残影面片
		float			m_tail_interval;	// 残影面片的间隔时间

		//-- 变化数据
		ndistribution<color4f>::ptr		m_color;
		ndistribution<vector2>::ptr		m_size;

		nDECLARE_CLASS(neffect_quad)
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_QUAD_H_