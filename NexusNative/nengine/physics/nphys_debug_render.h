#ifndef __NEXUS_PHYSX_DEBUG_RENDER_H_
#define __NEXUS_PHYSX_DEBUG_RENDER_H_
#include "nphys_pch.h"
#include "nphys_util.h"
#include "..\renderer\nrender_element.h"

namespace nexus
{

	/**
	* PhysX debug render 
	*
	* @param	PDI				Draw interface.
	* @param	data			PhysX debug renderable.
	*/
	void physx_render_data(const NxDebugRenderable* data,nrender_primitive_draw_interface* PDI)
	{
		if( data == NULL )return;

		// begin line batch
		PDI->begin_line_batch();

		int num_points = data->getNbPoints();
		if(num_points > 0)
		{
			const NxDebugPoint* points = data->getPoints();
			for(int i=0; i<num_points; i++)
			{
				PDI->draw_wire_star(physx::nx_vector_conv(points->p), 10.f, physx::nx_color_conv(points->color) );
				points++;
			}
		}	

		// Add all the 'lines' from PhysX
		int num_lines = data->getNbLines();
		if (num_lines > 0)
		{
			const NxDebugLine* lines = data->getLines();
			for(int i = 0; i<num_lines; i++)
			{
				PDI->draw_line(physx::nx_vector_conv(lines->p0),physx::nx_vector_conv(lines->p1), physx::nx_color_conv(lines->color) );
				lines++;
			}
		}

		// Add all the 'triangles' from PhysX
		int num_tris = data->getNbTriangles();
		if( num_tris > 0 )
		{
			const NxDebugTriangle* triangles = data->getTriangles();
			for(int i=0; i<num_tris; i++)
			{
				PDI->draw_line(physx::nx_vector_conv(triangles->p0),physx::nx_vector_conv(triangles->p1), physx::nx_color_conv(triangles->color) );
				PDI->draw_line(physx::nx_vector_conv(triangles->p1),physx::nx_vector_conv(triangles->p2), physx::nx_color_conv(triangles->color) );
				PDI->draw_line(physx::nx_vector_conv(triangles->p2),physx::nx_vector_conv(triangles->p0), physx::nx_color_conv(triangles->color) );
				triangles++;
			}
		}

		// end line batch
		PDI->end_line_batch();
	}
}


#endif // __NEXUS_PHYSX_DEBUG_RENDER_H_