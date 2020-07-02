#include "StdAfx.h"
#include "nterrain_base.h"
#include "../util/perlin.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_VIRTUAL_CLASS(nterrain_base, nprimitive_component)

	nterrain_base::nterrain_base(const nstring& name_str)
		: nprimitive_component(name_str)
	{
		m_trans_flag.set(ETrans_Opaque);
	}

	nterrain_base::~nterrain_base(void)
	{
	}

	void nterrain_base::create(size_t w, size_t h, short init_h, const terrain_cfg& cfg)
	{
		m_cfg = cfg;

		// create height
		m_height_map.create(w, h, init_h);
		
		// create layer alpha maps
		m_layer_map[0].create(w, h, 255);
		for(int i=1; i<MAX_ALPHA_LAYER; i++)
			m_layer_map[i].create(w, h, 0);

		//--
		m_bounds.box_extent.x = w*cfg.scale.x;
		m_bounds.box_extent.y = 0;
		m_bounds.box_extent.z = h*cfg.scale.z;
		m_bounds.sphere_radius = vec_length(m_bounds.box_extent);
		m_bounds.box_extent.y = 512*1024;

		_on_actor_move();
	}

	void nterrain_base::destroy()
	{
		m_height_map.destroy();

		for(int i=0; i<MAX_ALPHA_LAYER; i++)
			m_layer_map[i].destroy();
	}

	void nterrain_base::generate_noise(nrect rc, int numOctaves, float amplitude, float frequency)
	{
		if( rc.left < 0 )
			rc.left = 0;
		if( rc.top <0 )
			rc.top = 0;
		if( rc.right > m_height_map.get_width() )
			rc.right = m_height_map.get_width();
		if( rc.bottom > m_height_map.get_height() )
			rc.bottom = m_height_map.get_height();

		if( rc.get_width() <= 0
			|| rc.get_height() <= 0)
			return;

		Perlin pn(numOctaves, frequency, amplitude, rand());

		int ix,iy;
		float xStep = 1.0f/(rc.right-rc.left);
		float yStep = 1.0f/(rc.bottom-rc.top);

		for(iy=rc.top; iy<rc.bottom; iy++)
		{
			for(ix=rc.left; ix<rc.right; ix++)
			{
				float n = pn.Get(ix*xStep, iy*yStep);
				m_height_map.set_value(ix, iy, short(n));
			}
		}//end of for	

		post_height_changed(rc);
	}

	nmaterial* nterrain_base::get_material(int lod, int mtl_id)
	{
		(void)lod;	(void)mtl_id;

		if( m_material )
		{
			return m_material.get();
		}
		else
		{
			return nresource_manager::instance()->get_default_material();
		}
	}
}//namespace nexus