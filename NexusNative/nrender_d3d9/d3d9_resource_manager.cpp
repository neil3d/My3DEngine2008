#include "StdAfx.h"
#include <boost/bind.hpp>
#include "d3d_device_manager.h"
#include "d3d9_resource_manager.h"
#include "d3d9_static_mesh.h"
#include "d3d9_static_mesh_indexed.h"
#include "d3d9_anim_mesh.h"
#include "d3d9_cpu_skin_mesh.h"
#include "d3d9_texture.h"
#include "shading_effect.h"
#include "d3d9_dynamic_mesh_indexed.h"
#include "d3d9_dynamic_mesh.h"
#include "d3d9_simple_mesh.h"
#include "d3d9_render_target.h"
#include "d3d9_hit_proxy_hash.h"
#include "d3d9_render_font.h"
#include "d3d9_ui_canvas.h"
#include "FFTWave.h"

namespace nexus
{
	nDEFINE_CLASS(d3d9_resource_manager, nrender_resource_manager)

	d3d9_resource_manager::d3d9_resource_manager(void)
	{		
	}

	d3d9_resource_manager::~d3d9_resource_manager(void)
	{
	}

	void d3d9_resource_manager::destory()
	{
	}

	nrender_static_mesh* d3d9_resource_manager::alloc_static_mesh()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_static_mesh* ret = nNew d3d9_static_mesh;
		m_resources.insert(ret);
		return ret;
	}

	nrender_static_mesh_indexed* d3d9_resource_manager::alloc_static_mesh_indexed()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_static_mesh_indexed* ret = nNew d3d9_static_mesh_indexed;
		m_resources.insert(ret);
		return ret;
	}

	nrender_anim_mesh* d3d9_resource_manager::alloc_anim_mesh_indexed()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_anim_mesh_indexed* ret = nNew d3d9_anim_mesh_indexed;
		m_resources.insert( ret );
		return ret;
	}

	nrender_cpu_skin_mesh* d3d9_resource_manager::alloc_cup_skin_mesh()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_cpu_skin_mesh* ret = nNew d3d9_cpu_skin_mesh;
		m_resources.insert(ret);

		return ret;
	}

	nrender_texture2D* d3d9_resource_manager::alloc_texture_2d()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_texture2D* ret = nNew d3d9_texture2D;
		m_resources.insert( ret );
		return ret;
	}

	nrender_cube_map* d3d9_resource_manager::alloc_cube_map()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_cube_map* ret = nNew d3d9_cube_map;
		m_resources.insert(ret);
		return ret;
	}

	void d3d9_resource_manager::free_resource(nrender_resource* res)
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		if (m_resources.find(res) != m_resources.end())
		{
			delete res;
			m_resources.erase(res);
		}
	}

	nrender_dynamic_mesh_indexed* d3d9_resource_manager::alloc_dynamic_mesh_indexed()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_dynamic_mesh_indexed* ret = nNew d3d9_dynamic_mesh_indexed;
		m_resources.insert(ret);
		return ret;
	}

	nrender_dynamic_mesh* d3d9_resource_manager::alloc_dynamic_mesh()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_dynamic_mesh* ret = nNew d3d9_dynamic_mesh;
		m_resources.insert(ret);
		return ret;
	}

	nrender_simple_mesh* d3d9_resource_manager::alloc_simple_mesh()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_simple_mesh* ret = nNew d3d9_simple_mesh;
		m_resources.insert(ret);
		return ret;
	}

	nrender_font* d3d9_resource_manager::alloc_render_font()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		//std::wstringstream stream;
		//stream << face_name << width << height << weight << italic;
		//nname font_name( stream.str() );

		//for ( int i=0; i<m_font_cache.size(); i++ )
		//{
		//	if( m_font_cache[i]->get_font_name() == font_name )
		//	{
		//		return m_font_cache[i];
		//	}
		//}

		//d3d9_render_font* new_font =  nNew d3d9_render_font();
		//new_font->create(font_name,face_name,width,height,weight,italic);
		//m_font_cache.push_back(new_font);
		//return new_font;
		d3d9_render_font* ret = nNew d3d9_render_font();
		m_resources.insert(ret);
		return ret;
	}

	nui_canvas* d3d9_resource_manager::alloc_ui_canvas()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_ui_canvas* ret = nNew d3d9_ui_canvas;
		m_resources.insert(ret);
		return ret;
	}

	nrender_heightmap* d3d9_resource_manager::alloc_heightmap()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_heightmap* ret = nNew d3d9_heightmap;
		m_resources.insert(ret);
		return ret;
	}

	nrender_alphamap* d3d9_resource_manager::alloc_alphamap()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_alphamap* ret = nNew d3d9_alphamap;
		m_resources.insert(ret);
		return ret;
	}

	nrender_target* d3d9_resource_manager::alloc_render_target()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_render_target* ret = nNew d3d9_render_target;
		m_resources.insert(ret);
		return ret;
	}

	nhit_proxy_hash* d3d9_resource_manager::alloc_hit_proxy_hash()
	{
		boost::recursive_mutex::scoped_lock lock(m_mutex);

		d3d9_hit_proxy_hash* ret = nNew d3d9_hit_proxy_hash;
		m_resources.insert(ret);
		return ret;
	}
}//namespace nexus