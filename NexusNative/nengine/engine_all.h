#ifndef _NEXUS_ENGINE_ALL_H_
#define _NEXUS_ENGINE_ALL_H_

#include "ncore.h"

#include "engine_define.h"
#include "framework/nengine.h"
#include "framework/ngame_engine.h"
#include "framework/nlevel.h"
#include "framework/nactor.h"

//-- material
#include "material/nmtl_base.h"
#include "material/nmtl_dynamic.h"
#include "material/nmtl_static.h"
#include "material/nmtl_tech_shader_hlsl.h"

//-- mesh
#include "mesh/vertex_enum.h"
#include "mesh/mesh_index_data.h"

//-- resource
#include "resource/nresource.h"
#include "resource/nresource_manager.h"
#include "resource/nresource_static_mesh.h"
#include "resource/nresource_anim_mesh.h"
#include "resource/nresource_skeletal_mesh.h"
#include "resource/nresource_skeletal_anim_set.h"
#include "resource/nresource_texture.h"
#include "resource/nresource_texture_atlas.h"

//-- animation
#include "animation/nanim_controller.h"
#include "animation/nskel_anim_controller.h"
#include "animation/nskel_anim_play.h"

//-- actor
#include "actor/navatar_actor.h"
#include "actor/nactor_component.h"
#include "actor/nlight_component.h"
#include "actor/nprimitive_component.h"
#include "actor/nshape_component.h"
#include "actor/nmesh_component.h"
#include "actor/nstatic_mesh_component.h"
#include "actor/nanim_mesh_component.h"
#include "actor/nskeletal_mesh_component.h"
#include "actor/nfog_volume_component.h"
#include "actor/nwater_component.h"
#include "speed_tree/nspeed_tree_component.h"
#include "speed_tree/nspt_instance.h"
#include "sky/sky_base.h"

//-- terrain
#include "terrain/nterrain_actor.h"
#include "terrain/nterrain_mtl.h"
#include "terrain/nterrain_deco_chunk_base.h"
#include "terrain/nterrain_grass_deco_chunk.h"
#include "terrain/nterrain_static_mesh_deco_chunk.h"
#include "terrain/nterrain_grass_deco_chunk.h"
#include "terrain/nterrain_static_mesh_deco_chunk.h"
#include "terrain/nterrain_grass_deco_chunk.h"
#include "terrain/nterrain_static_mesh_deco_chunk.h"

//-- game
#include "game/nstart_point_actor.h"
#include "game/ntrigger_actor.h"
//-- 
#include "image/nimage.h"

//-- render
#include "renderer/renderer_framework.h"

//-- ÌØÐ§
#include "special_effect/nsub_uv.h"
#include "special_effect/nresource_special_effect.h"
#include "special_effect/nspecial_effect_instance.h"
#include "special_effect/neffect_element.h"
#include "special_effect/neffect_quad.h"
#include "special_effect/neffect_cylinder.h"
#include "special_effect/neffect_particle_emitter.h"
#include "special_effect/neffect_grid.h"

//-- util
#include "util/camera_controller.h"
#include "util/editor_camera_controller.h"
#include "util/hud_info.h"

#endif //_NEXUS_ENGINE_ALL_H_