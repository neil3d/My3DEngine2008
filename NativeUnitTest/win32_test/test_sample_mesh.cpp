#include "StdAfx.h"
#include "test_sample_mesh.h"

nDEFINE_CLASS(test_sample_mesh, nphys_test)

test_sample_mesh::test_sample_mesh(void)
{
}

test_sample_mesh::~test_sample_mesh(void)
{
}

extern nresource_static_mesh::ptr _load_man_mesh();
nresource_static_mesh::ptr _load_cart_mesh()
{
	nresource_static_mesh::ptr mesh_ptr = nresource_manager::instance()->new_static_mesh(_T("cart_mesh"));

	mesh_ptr->import_lod(-1, _T("../game_data/cart/cart.obj"));
	return mesh_ptr;
}

void test_sample_mesh::open(perspective_camera_controller* cam)
{
	nphys_test::open(cam);
	add_point_light();

	//--
#if 0
	nresource_static_mesh::ptr mesh_ptr = _load_cart_mesh();
#else
	nresource_static_mesh::ptr mesh_ptr = _load_man_mesh();
#endif

	box_sphere_bounds bounds = mesh_ptr->get_bounds();

	nactor::ptr actor_ptr = m_level_ptr->create_actor(_T("cart_actor"));
	shared_ptr<nstatic_mesh_component> comp_ptr 
		= actor_ptr->create_component_t<nstatic_mesh_component>(_T("mesh_comp"));
	comp_ptr->reset_resource(mesh_ptr);

	cam->set_look_at(bounds.origin);

	m_light_anim_radius = bounds.sphere_radius*1.5f;

	//--
	mesh_ptr->generate_simple_mesh();
	m_comp_ptr = comp_ptr;
}

void test_sample_mesh::debug_draw(nrender_primitive_draw_interface* PDI)
{
	m_comp_ptr->draw_simple_mesh(PDI, color4f(1,1,0,1));
}