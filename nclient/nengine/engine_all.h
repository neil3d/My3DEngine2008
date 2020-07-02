#ifndef _NEXUS_ENGINE_ALL_H_
#define _NEXUS_ENGINE_ALL_H_

#include "ncore.h"

#include "engine_define.h"
#include "framework/nengine.h"
#include "framework/ngame_engine.h"
#include "framework/nlevel.h"
#include "framework/nactor.h"

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

//-- animation
#include "animation/nanim_controller.h"
#include "animation/nskel_anim_controller.h"
#include "animation/nskel_anim_play.h"

//-- actor
#include "actor/nactor_component.h"
#include "actor/nlight_component.h"
#include "actor/nprimitive_component.h"
#include "actor/nshape_component.h"
#include "actor/nmesh_component.h"
#include "actor/nstatic_mesh_component.h"
#include "actor/nanim_mesh_component.h"
#include "actor/nskeletal_mesh_component.h"

//-- terrain
#include "terrain/nterrain_actor.h"
#include "terrain/nterrain_mtl.h"

//-- render
#include "renderer/renderer_framework.h"

//-- util
#include "util/camera_controller.h"
#include "util/editor_camera_controller.h"

#endif //_NEXUS_ENGINE_ALL_H_