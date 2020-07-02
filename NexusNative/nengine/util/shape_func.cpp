#include "StdAfx.h"
#include "shape_func.h"
#include "../framework/nengine.h"
#include "../mesh/mesh_vertex_data.h"
#include "../mesh/mesh_vertex_data_common.h"
#include "../renderer/nrender_resource_manager.h"

namespace nexus
{
	shape_func::shape_func(void)
	{
	}

	shape_func::~shape_func(void)
	{
	}

	nrender_static_mesh* shape_func::create_box_rich(float width, float height, float depth)
	{
		nrender_static_mesh* new_mesh_ptr = NULL;

		//--
		vector3 box_max(width/2, height/2, depth/2);
		vector3 box_min(-width/2, -height/2, -depth/2);

		vector3 pos_array[8];		
		vector2 uv_array[4];

		// top
		pos_array[0] = vector3(box_min.x, box_max.y, box_min.z);
		pos_array[1] = vector3(box_max.x, box_max.y, box_min.z);
		pos_array[2] = vector3(box_max.x, box_max.y, box_max.z);
		pos_array[3] = vector3(box_min.x, box_max.y, box_max.z);
		uv_array[0] = vector2(0,0);
		uv_array[1] = vector2(1,0);
		uv_array[2] = vector2(1,1);
		uv_array[2] = vector2(0,1);

		// bottom
		pos_array[4] = vector3(box_min.x, box_min.y, box_min.z);
		pos_array[5] = vector3(box_max.x, box_min.y, box_min.z);
		pos_array[6] = vector3(box_max.x, box_min.y, box_max.z);
		pos_array[7] = vector3(box_min.x, box_min.y, box_max.z);

		//--
		nmesh_vertex_data_common vertex_data;
		vertex_data.init(6*2*3, 2, 1);	// num_faces*triangle_per_face*vert_per_triangle
		vertex_stream* pos_stream = vertex_data.get_stream(0);
		vertex_stream* sec_stream = vertex_data.get_stream(1);

		size_t vert_index = 0;
		// top face		
		sec_stream->set_element(vert_index, 0,  vector3(0,1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,1));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[0]);

		sec_stream->set_element(vert_index, 0, vector3(0,1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,1));
		sec_stream->set_element(vert_index, 2,  vector2(1,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[3] );

		sec_stream->set_element(vert_index, 0, vector3(0,1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,1));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[2] );

		sec_stream->set_element(vert_index, 0, vector3(0,1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,1));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[2] );

		sec_stream->set_element(vert_index, 0, vector3(0,1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,1));
		sec_stream->set_element(vert_index, 2,  vector2(0,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[1] );

		sec_stream->set_element(vert_index, 0, vector3(0,1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,1));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[0] );

		// bottom face
		sec_stream->set_element(vert_index, 0, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,-1));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[4] );

		sec_stream->set_element(vert_index, 0, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,-1));
		sec_stream->set_element(vert_index, 2,  vector2(1,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[5] );

		sec_stream->set_element(vert_index, 0, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,-1));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[6] );

		sec_stream->set_element(vert_index, 0, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,-1));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[6] );

		sec_stream->set_element(vert_index, 0, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,-1));
		sec_stream->set_element(vert_index, 2,  vector2(0,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[7] );

		sec_stream->set_element(vert_index, 0, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 1,  vector3(0,0,-1));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[4] );

		// front face
		sec_stream->set_element(vert_index, 0, vector3(0,0,-1));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[4] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,-1));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[0] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,-1));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[1] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,-1));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[1] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,-1));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[5] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,-1));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[4] );

		// back face
		sec_stream->set_element(vert_index, 0, vector3(0,0,1));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[2] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,1));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[3] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,1));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[7] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,1));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[7] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,1));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[6] );

		sec_stream->set_element(vert_index, 0, vector3(0,0,1));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[2] );

		// left face
		sec_stream->set_element(vert_index, 0, vector3(-1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[0] );

		sec_stream->set_element(vert_index, 0, vector3(-1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[4] );

		sec_stream->set_element(vert_index, 0, vector3(-1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[7] );

		sec_stream->set_element(vert_index, 0, vector3(-1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[7] );

		sec_stream->set_element(vert_index, 0, vector3(-1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[3] );

		sec_stream->set_element(vert_index, 0, vector3(-1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,-1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[0] );

		// right face
		sec_stream->set_element(vert_index, 0, vector3(1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[1] );

		sec_stream->set_element(vert_index, 0, vector3(1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[2] );

		sec_stream->set_element(vert_index, 0, vector3(1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[6] );

		sec_stream->set_element(vert_index, 0, vector3(1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(1,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[6] );

		sec_stream->set_element(vert_index, 0, vector3(1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,1));
		pos_stream->set_element(vert_index++, 0,  pos_array[5] );

		sec_stream->set_element(vert_index, 0, vector3(1,0,0));
		sec_stream->set_element(vert_index, 1, vector3(0,1,0));
		sec_stream->set_element(vert_index, 2,  vector2(0,0));
		pos_stream->set_element(vert_index++, 0,  pos_array[1] );

		//-- create render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		new_mesh_ptr = ( rres_mgr->alloc_static_mesh() );

		if( !new_mesh_ptr )
			nthrow(_T("component: create render static mesh object failed."));

		new_mesh_ptr->create(EDraw_TriangleList, &(vertex_data) );		

		return new_mesh_ptr;
	}

	nrender_static_mesh* shape_func::create_box(float width, float height, float depth)
	{
		nrender_static_mesh* new_mesh_ptr = NULL;

		//--
		vector3 box_max(width/2, height/2, depth/2);
		vector3 box_min(-width/2, -height/2, -depth/2);

		vector3 pos_array[8];		

		// top
		pos_array[0] = vector3(box_min.x, box_max.y, box_min.z);
		pos_array[1] = vector3(box_max.x, box_max.y, box_min.z);
		pos_array[2] = vector3(box_max.x, box_max.y, box_max.z);
		pos_array[3] = vector3(box_min.x, box_max.y, box_max.z);

		// bottom
		pos_array[4] = vector3(box_min.x, box_min.y, box_min.z);
		pos_array[5] = vector3(box_max.x, box_min.y, box_min.z);
		pos_array[6] = vector3(box_max.x, box_min.y, box_max.z);
		pos_array[7] = vector3(box_min.x, box_min.y, box_max.z);

		//--
		nmesh_vertex_data_pos_normal vertex_data;
		vertex_data.init(6*2*3);	// num_faces*triangle_per_face*vert_per_triangle
		vertex_stream& pos_stream = vertex_data.get_pos_stream();
		vertex_stream& normal_stream = vertex_data.get_normal_stream();

		size_t vert_index = 0;
		// top face		
		normal_stream.set_element(vert_index, 0,  vector3(0,1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[0]);

		normal_stream.set_element(vert_index, 0, vector3(0,1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[3] );

		normal_stream.set_element(vert_index, 0, vector3(0,1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[2] );

		normal_stream.set_element(vert_index, 0, vector3(0,1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[2] );

		normal_stream.set_element(vert_index, 0, vector3(0,1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[1] );

		normal_stream.set_element(vert_index, 0, vector3(0,1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[0] );

		// bottom face
		normal_stream.set_element(vert_index, 0, vector3(0,-1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[4] );

		normal_stream.set_element(vert_index, 0, vector3(0,-1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[7] );

		normal_stream.set_element(vert_index, 0, vector3(0,-1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[6] );

		normal_stream.set_element(vert_index, 0, vector3(0,-1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[6] );

		normal_stream.set_element(vert_index, 0, vector3(0,-1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[5] );

		normal_stream.set_element(vert_index, 0, vector3(0,-1,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[4] );

		// front face
		normal_stream.set_element(vert_index, 0, vector3(0,0,-1));
		pos_stream.set_element(vert_index++, 0,  pos_array[4] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,-1));
		pos_stream.set_element(vert_index++, 0,  pos_array[0] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,-1));
		pos_stream.set_element(vert_index++, 0,  pos_array[1] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,-1));
		pos_stream.set_element(vert_index++, 0,  pos_array[1] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,-1));
		pos_stream.set_element(vert_index++, 0,  pos_array[5] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,-1));
		pos_stream.set_element(vert_index++, 0,  pos_array[4] );

		// back face
		normal_stream.set_element(vert_index, 0, vector3(0,0,1));
		pos_stream.set_element(vert_index++, 0,  pos_array[2] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,1));
		pos_stream.set_element(vert_index++, 0,  pos_array[3] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,1));
		pos_stream.set_element(vert_index++, 0,  pos_array[7] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,1));
		pos_stream.set_element(vert_index++, 0,  pos_array[7] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,1));
		pos_stream.set_element(vert_index++, 0,  pos_array[6] );

		normal_stream.set_element(vert_index, 0, vector3(0,0,1));
		pos_stream.set_element(vert_index++, 0,  pos_array[2] );

		// left face
		normal_stream.set_element(vert_index, 0, vector3(-1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[0] );

		normal_stream.set_element(vert_index, 0, vector3(-1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[4] );

		normal_stream.set_element(vert_index, 0, vector3(-1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[7] );

		normal_stream.set_element(vert_index, 0, vector3(-1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[7] );

		normal_stream.set_element(vert_index, 0, vector3(-1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[3] );

		normal_stream.set_element(vert_index, 0, vector3(-1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[0] );

		// right face
		normal_stream.set_element(vert_index, 0, vector3(1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[1] );

		normal_stream.set_element(vert_index, 0, vector3(1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[2] );

		normal_stream.set_element(vert_index, 0, vector3(1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[6] );

		normal_stream.set_element(vert_index, 0, vector3(1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[6] );

		normal_stream.set_element(vert_index, 0, vector3(1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[5] );

		normal_stream.set_element(vert_index, 0, vector3(1,0,0));
		pos_stream.set_element(vert_index++, 0,  pos_array[1] );

		//-- create render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		 new_mesh_ptr = ( rres_mgr->alloc_static_mesh() );

		if( !new_mesh_ptr )
			nthrow(_T("component: create render static mesh object failed."));

		new_mesh_ptr->create(EDraw_TriangleList, &(vertex_data) );		

		return new_mesh_ptr;
	}

	// TODO: bug - bottom cap error
	nrender_static_mesh_indexed* shape_func::create_sphere(vector3& vCenter, float fRadius, unsigned short wNumRings, unsigned short wNumSections)
	{
		nrender_static_mesh_indexed* new_mesh_ptr = NULL;
		//-- 
		class Vertex
		{
		public:
			Vertex(const vector3& Pos, const vector3& Norm, const vector2& Tex)
				: Position(Pos), Normal(Norm), Texture(Tex)
			{
				Tangent = vec_normalize(vec_cross( vector3(0,1,0),Normal));
			}

			Vertex()
			{}

			vector3 Position;
			vector3 Normal;
			vector3 Tangent;
			vector2 Texture;
		};

		const float PI = 3.1415926f;
		float sx=1, sy =1, sz = 1;

		//------------------------------------------------------------------------------------------------
		float x, y, z, v, rsintheta; // Temporary variables
		unsigned short  i, j, n, m;            // counters
		vector3 vPoint;
		
		//Generate space for the required triangles and vertices.
		unsigned short       wNumTriangles = wNumRings * wNumSections * 2;
		unsigned int      dwNumIndices   = wNumTriangles*3;
		unsigned int      dwNumVertices  = (wNumRings+1) * (wNumSections+1);

		Vertex* pvVertices     = nNew Vertex[dwNumVertices];
		unsigned short*      pwIndices      = nNew unsigned short[3*wNumTriangles];

		// Generate vertices at the top and bottom points.
		vector3 vTopPoint  = vCenter + vector3( 0.0f, +sy*fRadius, 0.0f);
		vector3 vBotPoint  = vCenter + vector3( 0.0f, -sy*fRadius, 0.0f);
		vector3 vNormal = vector3( 0.0f, 1.0f, 0.0f );
		
		
		
		// Generate vertex points for rings
		float dtheta =nPI /  (float)wNumRings;     //Angle between each ring
		float dphi   = 2*nPI / (float)wNumSections; //Angle between each section
		float theta  = 0;
		n = 0; //vertex being generated, begins at 1 to skip top point

		for(int i = 0; i < wNumRings + 1; i++ )
		{
			y = fRadius * (float)cos(theta); // y is the same for each ring
			v = theta / PI;     // v is the same for each ring
			rsintheta = fRadius * (float)sin(theta);
			float phi = 0.0f;

			for( j = 0; j < wNumSections+1; j++ )
			{
				x = rsintheta * (float)sin(phi);
				z = rsintheta * (float)cos(phi);

				float u = (float)(1.0 - phi / (2*PI) );

				vPoint        = vCenter + vector3( sx*x, sy*y, sz*z );
				vNormal       = vector3( x/fRadius, y/fRadius, z/fRadius );
				vNormal = vec_normalize(vNormal);
				
				pvVertices[n] = Vertex( vector3(vPoint.x, vPoint.y, vPoint.z), vector3(vNormal.x, vNormal.y, vNormal.z), vector2(u, v) );

				phi += dphi;
				++n;
			}
			theta += dtheta;
		}

		for( j = 0; j < wNumSections+1; j++ )
		{
			pvVertices[j].Tangent = pvVertices[j+wNumSections+1].Tangent ;
			pvVertices[dwNumVertices - 1 - j].Tangent = pvVertices[dwNumVertices - j - wNumSections - 2 ].Tangent ;
		}

		// Generate triangles for the rings
		m = 0;            // first vertex in current ring,begins at 1 to skip top point
		n = 0; // triangle being generated, skip the top cap 

		for( i = 0; i < wNumRings; i++ )
		{
			for( j = 0; j < wNumSections; j++ )
			{
				pwIndices[3*n+0] = m + j;
				pwIndices[3*n+1] = m + wNumSections + 1+ j;
				pwIndices[3*n+2] = m + wNumSections + 1+(j + 1);

				pwIndices[3*(n+1)+0] = pwIndices[3*n+0];
				pwIndices[3*(n+1)+1] = pwIndices[3*n+2];
				pwIndices[3*(n+1)+2] = m + j + 1;

				n += 2;
			}
			m += wNumSections+1;
		}
		
		//------------------------------------------------------------------------------------------------
		nmesh_vertex_data_common vert_data;
		vert_data.init(dwNumVertices, 2, 1);
		vertex_stream* pos_stream = vert_data.get_stream(0);
		vertex_stream* second_stream = vert_data.get_stream(1);
		
		for(i=0; i<dwNumVertices; i++)
		{
			Vertex& vt = pvVertices[i];

			pos_stream->set_element(i, 0, vt.Position);
			second_stream->set_element(i, 0, vt.Normal);	
			second_stream->set_element(i,1,vt.Tangent);
			second_stream->set_element(i, 2, vt.Texture);
		}

		index_buffer16 index_data;
		index_data.data.resize(dwNumIndices);
		memcpy((void*)index_data.get_data_ptr(), pwIndices, index_data.get_data_size());

		delete [] pvVertices;
		delete [] pwIndices;

		
		//-- create render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();

		new_mesh_ptr = ( rres_mgr->alloc_static_mesh_indexed() );

		if( !new_mesh_ptr )
			nthrow(_T("component: create render static mesh object failed."));

		// TODO: material id
		new_mesh_ptr->create(EDraw_TriangleList, &vert_data, &index_data, 0);		

		return new_mesh_ptr;
	}
}//namespace nexus