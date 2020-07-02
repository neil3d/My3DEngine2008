/**
*	Nexus Engine - 灯光组件
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_LIGHT_COMPONENT_H_
#define _NEXUS_LIGHT_COMPONENT_H_
#include "nprimitive_component.h"
#include "../scene/ngeneric_octree.h"

namespace nexus
{
	/**
	 *	灯光
	 *	@remark Actor space
	*/

	typedef std::set<nprimitive_component*> prim_set;

	enum ELightType
	{
		ELight_None,
		ELight_Point,
		ELight_Directional,		
		Elight_Spot,
		//ELight_SkyLight,

		ELight_Max
	};


	class nAPI nlight_component :
		public nactor_component
	{
	public:
		typedef boost::shared_ptr<nlight_component> ptr;

		nlight_component(const nstring& name_str);
		virtual ~nlight_component(void);

		virtual void _init(nactor* owner);
		virtual void render(const nviewport& view);
		virtual void update_transform(const object_space& parent_space);
		virtual void _update_transform(const struct object_space& parent_space) 
		{ 
			m_bounds.origin=m_bounds.origin*parent_space.local_to_world;
		};

		virtual const box_sphere_bounds& get_bounds() const			{	return m_bounds;}
		virtual void _remove_from_level();
		virtual void _destroy();

		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);

		virtual void add_affect_prim(nprimitive_component* prim) { (void) prim; };
		virtual void remove_affect_prim(nprimitive_component* prim) { (void) prim; };
		virtual void clear() {};
		bool get_visible() const;
		float get_radius() const { return 0; }
//	protected:
		box_sphere_bounds				m_bounds;
		ELightType							m_type;
		color4f									m_color;
		float										m_shadow_softness;
		bool										project_shadow;

	private:
		friend class nrender_light_proxy;
		nDECLARE_NAMED_CLASS(nlight_component)
	};

	class nAPI npoint_light_component :
		public nlight_component
	{
	public:
		npoint_light_component(const nstring& name_str);
		virtual ~npoint_light_component(void);

		virtual void _update_transform(const object_space& parent_space);
		virtual void serialize(narchive& ar);
		virtual void draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color);
		virtual void add_affect_prim(nprimitive_component* prim);
		virtual void remove_affect_prim(nprimitive_component* prim);

		virtual void clear()
		{
			for (int i = 0; i <FACE_MAX; i ++ )
			{
				affect_prims[i].clear();
			}

			affect_prim_set.clear();
		}

		float get_radius() const { return m_attenuation.x; }

		enum ECube_Face
		{
			POSITIVE_X     = 0,
			NEGATIVE_X     = 1,
			POSITIVE_Y     = 2,
			NEGATIVE_Y     = 3,
			POSITIVE_Z     = 4,
			NEGATIVE_Z     = 5,

			FACE_MAX = 6
		};

		vector3 m_position;
		vector3	m_attenuation;  //x为影响半径，y为衰减系数，z为强度
		prim_set affect_prims[FACE_MAX];
		prim_set affect_prim_set;
		nview_info view_infos[FACE_MAX];
		friend class npoint_light_proxy;
		nDECLARE_NAMED_CLASS(npoint_light_component)
	};

	class nAPI ndirectional_light_component :
		public nlight_component
	{
	public:
		ndirectional_light_component(const nstring& name_str);
		virtual ~ndirectional_light_component(void);

		virtual void _update_transform(const object_space& parent_space)
		{
			m_bounds.origin = vector3::zero;
			m_bounds.box_extent = vector3(HALF_WORLD_MAX,HALF_WORLD_MAX,HALF_WORLD_MAX);
			local_to_world = parent_space.local_to_world;
			nlight_component::_update_transform(parent_space);	
		}
		
		virtual void serialize(narchive& ar);

		matrix44	local_to_world;
		friend class ndirectional_light_proxy;
		nDECLARE_NAMED_CLASS(ndirectional_light_component)
	};

	class nAPI nspot_light_component :
		public nlight_component
	{
	public:
		nspot_light_component(const nstring& name_str);
		virtual ~nspot_light_component(void);

		virtual void _update_transform(const object_space& parent_space);
		virtual void serialize(narchive& ar);
		virtual void draw_bounds(nrender_primitive_draw_interface* PDI, const color4f& color);
		float get_radius() const { return m_attenuation.x; }
		virtual void add_affect_prim(nprimitive_component* prim);
		virtual void remove_affect_prim(nprimitive_component* prim);

		virtual void clear()
		{
			affect_prim_set.clear();
		}

		nview_info view_info;
		prim_set	affect_prim_set;	
		matrix44	local_to_world;
		vector3		m_attenuation;
		float			inner_cone_angle;
		float			outer_cone_angle;

		friend class nspot_light_proxy;
		nDECLARE_NAMED_CLASS(nspot_light_component)
	};
}//namespace nexus

#endif //_NEXUS_LIGHT_COMPONENT_H_