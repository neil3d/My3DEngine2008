#include <boost/python.hpp>
#include "ncore.h"
#include "engine_all.h"
#include "engine_editor.h"

void export_terrain_editor()
{
	using namespace boost::python;
	using namespace nexus;	


	enum_<ETrnLerpMode>("ETrnLerpMode")
		.value("ELM_None", ELM_None)
		.value("ELM_Linear", ELM_Linear)
		.value("ELM_Cosine", ELM_Cosine)
		.value("ELM_Quadratic", ELM_Quadratic)
		.export_values()
		;


	enum_<ETrnBurshTarget>("ETrnBurshTarget")
		.value("EBT_Heightmap", EBT_Heightmap)
		.value("EBT_Alphamap", EBT_Alphamap)		
		.value("EBT_Decomap", EBT_Decomap)		
		.export_values()
		;

	enum_<ETrnBrushShape>("ETrnBrushShape")
		.value("EBS_Circle", EBS_Circle)
		.value("EBS_Square", EBS_Square)		
		.export_values()
		;

	enum_<ETrnBrushOP>("ETrnBrushOP")
		.value("EOP_Paint", EOP_Paint)
		.value("EOP_Flatten", EOP_Flatten)
		.value("EOP_Smooth", EOP_Smooth)
		.value("EOP_Apply", EOP_Apply)
		.value("EOP_Noise", EOP_Noise)
		.export_values()
		;

	class_<nterrain_brush>("nterrain_brush")
		.def_readwrite("op", &nterrain_brush::op)		
		.def_readwrite("shape", &nterrain_brush::shape)		
		.def_readwrite("lerp_mode", &nterrain_brush::lerp_mode)		
		.def_readwrite("inner_radius", &nterrain_brush::inner_radius)		
		.def_readwrite("outer_radius", &nterrain_brush::outer_radius)		
		.def_readwrite("strength", &nterrain_brush::strength)
		.def_readwrite("noise_freq", &nterrain_brush::noise_freq)
		.def_readwrite("target", &nterrain_brush::target)
		.def_readwrite("target_index", &nterrain_brush::target_index)
		;


	class_<nterrain_editor>("nterrain_editor")
		.def("bind_terrain", &nterrain_editor::bind_terrain)
		.def("set_brush", &nterrain_editor::set_brush)
		.def("draw_widgets", &nterrain_editor::draw_widgets)
		.def("close", &nterrain_editor::close)
		.def("generate_noise", &nterrain_editor::generate_noise)
		.def("import_heightmap", &nterrain_editor::import_heightmap)
		.def("create_material_basic", &nterrain_editor::create_material_basic)
		.def("create_texture_splatting", &nterrain_editor::create_texture_splatting)
		.def("splat_set_texture", &nterrain_editor::splat_set_texture)		
		.def("splat_layer_noise", &nterrain_editor::splat_layer_noise)		

		.def("on_mouse_left_down", &nterrain_editor::on_mouse_left_down)
		.def("on_mouse_left_up", &nterrain_editor::on_mouse_left_up)
		.def("on_mouse_right_down", &nterrain_editor::on_mouse_right_down)
		.def("on_mouse_right_up", &nterrain_editor::on_mouse_right_up)
		.def("on_mouse_wheel", &nterrain_editor::on_mouse_wheel)
		.def("on_mouse_move", &nterrain_editor::on_mouse_move)
		;
}