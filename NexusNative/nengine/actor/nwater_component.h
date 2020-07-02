#pragma once
#include "nprimitive_component.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	const float CYC_LENGTH = 256;

	class nAPI nwater_component :
		public nprimitive_component
	{
	public:
		typedef shared_ptr<nwater_component> ptr;

		nwater_component(const nstring& name_str);
		virtual ~nwater_component(void);

		virtual void _destroy();
		virtual void _init(nactor* owner);
		void set_size(unsigned int i,unsigned int j)
		{
			size_x = i;
			size_y = j;

			_destroy();
			create_render_resource();
			update_transform(m_owner->get_space());
		}

		vector2 get_size()
		{
			return vector2(size_x,size_y);
		}

		void create_render_resource();
		void _update_transform(const object_space& parent_space);
		virtual void render(class render_package_base* rpb);
		virtual void serialize(narchive& ar);

		virtual void _on_device_lost(int param);
		virtual bool _on_device_reset(int param);

		color4f  m_water_color;
		float		m_water_density;
		float     m_water_soft_interact_dist;
		float     m_foam_soft_interact_dist;
		float		m_foam_factor;
	private:	
		unsigned int size_x,size_y;
		nrender_simple_mesh* m_water_mesh;
		nresource_cube_map::ptr EnvMap;
		nresource_texture_2d::ptr foam_tex;
		nresource_texture_2d::ptr caustic_tex;

		friend class nwater_render_proxy;
        
		nDECLARE_NAMED_CLASS(nwater_component)
	};
}