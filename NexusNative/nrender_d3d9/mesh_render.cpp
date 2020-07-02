#include "StdAfx.h"
#include "mesh_render.h"
#include "../nengine/actor/nmesh_component.h"
#include "drawing_policy.h"
#include "shading_effect.h"
#include "d3d9_mesh_section.h"

namespace nexus
{
	mesh_instance_renderer::~mesh_instance_renderer()
	{
		clear();
	}

	void mesh_instance_renderer::clear()
	{
		for (instance_map::iterator iter = instances.begin();
			iter != instances.end();
			++iter)
		{
			delete iter->second;
		}

		instances.clear();
	}

	void mesh_instance_renderer::add_mesh(const mesh_element* element)
	{
		std::pair<nrender_mesh_section*, nmtl_base*> mesh_key(element->sec,element->mtl);
		instance_map::iterator iter = instances.find( mesh_key );
		if(iter != instances.end())
		{
			mesh_instance_adapter* adapter = iter->second;
			adapter->add_instance(element);
		}
		else
		{
			mesh_instance_adapter* adapter = new mesh_instance_adapter(element->mesh,element->sec,element->mtl);
			adapter->add_instance(element);
			instances.insert(pair<std::pair<nrender_mesh_section*, nmtl_base*>, mesh_instance_adapter*>(mesh_key,adapter));
		}
	}	
	
	void	mesh_instance_renderer::init()
	{
		for (instance_map::iterator iter = instances.begin();
			iter != instances.end();
			++iter)
		{
			iter->second->init();
		}
	}

	void mesh_instance_renderer::draw_all(const nview_info* view,drawing_policy* dp)
	{
		for (instance_map::iterator iter = instances.begin();
			iter != instances.end();
			++iter)
		{
			mesh_instance_adapter* adapter = iter->second;
			dp->draw(view,mesh_element(NULL,adapter,adapter->m_mesh_section,adapter->m_mtl,true));
		}
	}
}