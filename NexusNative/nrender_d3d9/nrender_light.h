/**
*	Nexus Engine - 渲染模块使用的灯光接口
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_RENDER_LIGHT_H_
#define _NEXUS_RENDER_LIGHT_H_

#include "d3d_ptr.h"

namespace nexus
{
	class nlight_component;
	class npoint_light_component;
	class ndirectional_light_component;
	class nspot_light_component;

	static const int TEX_SIZE_X = 1024;
	static const int TEX_SIZE_Y = 1024;
	static const int JITTER_SIZE = 32;
	static const int JITTER_SAMPLES = 8;
	static const int SPLIT_NUM = 1;

	/**
	*	渲染系统使用的灯光接口
	*	@remark 向渲染系统传递Light的数据，主要包括灯光类型、颜色等自身参数、以及判断Light是否影响场景中某个对象；
	*/
	class nrender_light_proxy
	{
	public:
		nrender_light_proxy()
		{}

		virtual ~nrender_light_proxy()
		{}
		
		virtual void render(class nrenderer_d3d9* renderer) = 0;
		virtual void draw_setup_effect(class d3d9_shading_effect* effect_ptr) const = 0;
	};

	class npoint_light_proxy
		: public nrender_light_proxy
	{
	public:
		npoint_light_proxy() {};
		npoint_light_proxy(npoint_light_component* owner);
		virtual ~npoint_light_proxy();
		
		virtual void render(class nrenderer_d3d9* renderer);
		virtual void draw_setup_effect(d3d9_shading_effect* effect_ptr) const;

	protected:
		class drawing_policy*	m_drawing_policy[3];;
	private:
		npoint_light_component*	m_owner;
	};

	class ndirectional_light_proxy
		: public nrender_light_proxy
	{
	public:
		ndirectional_light_proxy() {};
		ndirectional_light_proxy(ndirectional_light_component* owner);
		virtual ~ndirectional_light_proxy();

		virtual void render(class nrenderer_d3d9* renderer);
		virtual void draw_setup_effect(d3d9_shading_effect* effect_ptr)	const;
	private:
		void calculate_split_distances(float camera_near,float camera_far,int split_num);
		void calculate_light_for_frustum(const vector<vector3>& corners,const nview_info& camera_view);

		ndirectional_light_component*	m_owner;
		drawing_policy*	m_drawing_policy[3];
		
		vector<float> split_pos;

		class nview_info m_lgt_view;
	};

	class nspot_light_proxy
		: public npoint_light_proxy
	{
	public:
		nspot_light_proxy()  {};
		nspot_light_proxy(nspot_light_component* owner);
		virtual ~nspot_light_proxy();
		
		virtual void render(class nrenderer_d3d9* renderer);
		virtual void draw_setup_effect(d3d9_shading_effect* effect_ptr) const;
	private:
		nspot_light_component*	m_owner;
	};
}//namespace nexus

#endif //_NEXUS_RENDER_LIGHT_H_