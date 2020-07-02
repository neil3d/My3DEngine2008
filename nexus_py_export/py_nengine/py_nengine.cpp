#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"
#include "engine_editor.h"

#include "py_nengine.h"

#include "terrain_exp.h"
#include "camera_controller_exp.h"

#include "framework_api.h"
#include "render_api.h"
#include "actor_api.h"
#include "resource_api.h"
#include "material_api.h"
#include "editor_api.h"

void py_nengine_export()
{
	//-- export framework
	export_event();
	export_actor();
	export_terrain();
	export_nengine();
	export_level();

	//-- export actor components
	export_actor_component();
	export_primitive_component();
	export_mesh_component();
	export_static_mesh_component();
	export_shape_component();
	export_light_component();

	//-- export util classes
	export_camera_controller();

	//-- export renderer
	export_render_base();
	export_render_element();
	export_render_light();

	//-- export resource system
	export_nresource();
	export_nresource_static_mesh();
	export_nresource_manager();

	//-- export material classes
	export_material_parameter();
	export_material();
	export_material_template();

	//-- export editor
	export_editor_engine();
	export_terrain_editor();
	
}