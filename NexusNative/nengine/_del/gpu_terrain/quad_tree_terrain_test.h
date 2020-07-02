#pragma once
#include "ntest_case.h"
#include "../../NexusNative/nengine/engine_editor.h"

class quad_tree_terrain_test :
	public nrender_test
{
public:
	quad_tree_terrain_test(void);
	virtual ~quad_tree_terrain_test(void);

	virtual void open(perspective_camera_controller* cam);	

	virtual void debug_draw(nrender_primitive_draw_interface* PDI)	
	{		
	}
private:	
	nDECLARE_CLASS(quad_tree_terrain_test)
};


class terrain_tex_splatting_test :
	public nrender_test
{
public:
	terrain_tex_splatting_test(void);
	virtual ~terrain_tex_splatting_test(void);

	virtual void open(perspective_camera_controller* cam);	

	nDECLARE_CLASS(terrain_tex_splatting_test)
};