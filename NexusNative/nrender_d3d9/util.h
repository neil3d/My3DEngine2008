#pragma once
#include "d3d_ptr.h"

namespace nexus
{
	void load_shader_source(const nstring& file_name, std::string& shader_source);

	void d3d_copy_vertex_buffer(d3d_vb_ptr dest_vb, const vertex_stream* src_stream);

	struct sphere 
	{
		vector<vector3> positions;
		vector<UINT> indexes;

		sphere()
		{
			const float PI = 3.14159265f;

			//------------------------------------------------------------------------------------------------
			float x, y, z, v, rsintheta; // Temporary variables
			unsigned short  i, j, n, m;            // counters
			vector3 vPoint;

			//Generate space for the required triangles and vertices.
			float fRadius = 1.0f/cos(PI/16.0f);
			WORD wNumRings = 16;
			WORD wNumSections = 16;
			unsigned short       wNumTriangles = (wNumRings + 1) * wNumSections * 2;
			unsigned int      dwNumIndices   = wNumTriangles*3;
			unsigned int      dwNumVertices  = (wNumRings + 1) * wNumSections + 2;

			positions.resize(dwNumVertices);
			indexes .resize(3*wNumTriangles);

			// Generate vertices at the top and bottom points.
			vector3 vTopPoint  = vector3( 0.0f, +fRadius, 0.0f);
			vector3 vBotPoint  = vector3( 0.0f, -fRadius, 0.0f);
			vector3 vNormal = vector3( 0.0f, 1.0f, 0.0f );

			positions[0]               = vTopPoint;
			positions[dwNumVertices-1] = vBotPoint;

			// Generate vertex points for rings
			float dtheta = (float)(PI / (wNumRings + 2));     //Angle between each ring
			float dphi   = (float)(2*PI / wNumSections); //Angle between each section
			float theta  = dtheta;
			n = 1; //vertex being generated, begins at 1 to skip top point

			for( i = 0; i < (wNumRings+1); i++ )
			{
				y = fRadius * (float)cos(theta); // y is the same for each ring
				v = theta / PI;     // v is the same for each ring
				rsintheta = fRadius * (float)sin(theta);
				float phi = 0.0f;

				for( j = 0; j < wNumSections; j++ )
				{
					x = rsintheta * (float)sin(phi);
					z = rsintheta * (float)cos(phi);

					positions[n] = vector3( x, y, z );
					phi += dphi;
					++n;
				}

				theta += dtheta;
			}

			// Generate triangles for top and bottom caps.
			for( i = 0; i < wNumSections; i++ )
			{
				indexes[3*i+0] = 0;
				indexes[3*i+1] = i + 1;
				indexes[3*i+2] = 1 + ((i + 1) % wNumSections);

				indexes[3*(wNumTriangles - wNumSections + i)+0] = ( dwNumVertices - 1 );
				indexes[3*(wNumTriangles - wNumSections + i)+1] = ( dwNumVertices - 2 - i );
				indexes[3*(wNumTriangles - wNumSections + i)+2] = ( dwNumVertices - 2 - ((1 + i) % wNumSections) );
			}

			// Generate triangles for the rings
			m = 1;            // first vertex in current ring,begins at 1 to skip top point
			n = wNumSections; // triangle being generated, skip the top cap 

			for( i = 0; i < wNumRings; i++ )
			{
				for( j = 0; j < wNumSections; j++ )
				{
					indexes[3*n+0] = m + j;
					indexes[3*n+1] = m + wNumSections + j;
					indexes[3*n+2] = m + wNumSections + ((j + 1) % wNumSections);

					indexes[3*(n+1)+0] = indexes[3*n+0];
					indexes[3*(n+1)+1] = indexes[3*n+2];
					indexes[3*(n+1)+2] = m + ((j + 1) % wNumSections);

					n += 2;
				}
				m += wNumSections;
			}
		}
	};

	extern sphere g_sphere;

	const UINT cube_index[36] = {
		0,1,3,
		1,2,3,
		3,2,7,
		2,6,7,
		0,7,4,
		0,3,7,
		0,4,5,
		0,5,1,
		4,7,5,
		5,7,6,
		1,5,2,
		5,6,2
	};
}//namespace nexus
