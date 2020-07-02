/**
*	Nexus Engine - 渲染模块使用的灯光接口
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_RENDER_LIGHT_H_
#define _NEXUS_RENDER_LIGHT_H_
#include "ncore.h"

namespace nexus
{

	class nlight_component;

	enum ELightType
	{
		ELight_None,
		ELight_Point,
		ELight_Directional,		
		//Elight_Spot,
		//ELight_SkyLight,

		ELight_Max
	};

	/**
	*	渲染系统使用的灯光接口
	*	@remark 向渲染系统传递Light的数据，主要包括灯光类型、颜色等自身参数、以及判断Light是否影响场景中某个对象；
	*/
	struct nAPI nrender_light_proxy
	{
		nlight_component*	m_owner;
		enum ELightType		m_type;
		color4f				m_ambient_color;
		color4f				m_diffuse_color;

		nrender_light_proxy(void):m_owner(NULL),m_type(ELight_None)
		{}
		nrender_light_proxy(nlight_component* owner, enum ELightType lt)
			:m_owner(owner), m_type(lt), 
			m_ambient_color(0,0,0,0), 
			m_diffuse_color(1,1,1,1)
		{}

		virtual ~nrender_light_proxy()
		{}
	};

	struct nAPI npoint_light_proxy
		: public nrender_light_proxy
	{
		vector3	m_position;
		vector3	m_attenuation;

		npoint_light_proxy(void)
		{}
		npoint_light_proxy(nlight_component* owner):nrender_light_proxy(owner, ELight_Point),
			m_position(0,0,0), m_attenuation(1,0,0)
		{}
	};

	struct nAPI ndirectional_light_proxy
		: public nrender_light_proxy
	{
		vector3	m_direction;

		ndirectional_light_proxy(void)
		{}
		ndirectional_light_proxy(nlight_component* owner):nrender_light_proxy(owner, ELight_Directional),
			m_direction(0,-1,0)
		{}
	};
}//namespace nexus

#endif //_NEXUS_RENDER_LIGHT_H_