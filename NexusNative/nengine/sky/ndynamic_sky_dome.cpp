#include "StdAfx.h"
#include "ndynamic_sky_dome.h"
#include "../framework/nengine.h"
#include "../resource/nresource_manager.h"

namespace nexus
{
	nDEFINE_NAMED_CLASS(ndynamic_sky_dome, nsky_component)
	
	float ndynamic_sky_dome::s_distributionLuminance[ 5 ][ 2 ] = 	// Perez distributions
	{
		{  0.17872f , -1.46303f },		// a = darkening or brightening of the horizon
		{ -0.35540f ,  0.42749f },		// b = luminance gradient near the horizon,
		{ -0.02266f ,  5.32505f },		// c = relative intensity of the circumsolar region
		{  0.12064f , -2.57705f },		// d = width of the circumsolar region
		{ -0.06696f ,  0.37027f }		// e = relative backscattered light
	};
	float ndynamic_sky_dome::s_distributionXcomp[ 5 ][ 2 ] = 
	{
		{ -0.01925f , -0.25922f },
		{ -0.06651f ,  0.00081f },
		{ -0.00041f ,  0.21247f },
		{ -0.06409f , -0.89887f },
		{ -0.00325f ,  0.04517f }
	};
	float ndynamic_sky_dome::s_distributionYcomp[ 5 ][ 2 ] = 
	{
		{ -0.01669f , -0.26078f },
		{ -0.09495f ,  0.00921f },
		{ -0.00792f ,  0.21023f },
		{ -0.04405f , -1.65369f },
		{ -0.01092f ,  0.05291f }	
	};

	float ndynamic_sky_dome::s_zenithXmatrix[ 3 ][ 4 ] = 
	{
		{  0.00165f, -0.00375f,  0.00209f,  0.00000f },
		{ -0.02903f,  0.06377f, -0.03202f,  0.00394f },
		{  0.11693f, -0.21196f,  0.06052f,  0.25886f }
	};
	float ndynamic_sky_dome::s_zenithYmatrix[ 3 ][ 4 ] = 
	{
		{  0.00275f, -0.00610f,  0.00317f,  0.00000f },
		{ -0.04214f,  0.08970f, -0.04153f,  0.00516f },
		{  0.15346f, -0.26756f,  0.06670f,  0.26688f }
	};


	////////////////////////////////////////////////////////////////////////////
	//	class nsky_dome_vertex_data
	////////////////////////////////////////////////////////////////////////////
	nsky_dome_vertex_data::nsky_dome_vertex_data(void)
	{		
	}

	nsky_dome_vertex_data::~nsky_dome_vertex_data(void)
	{}

	nstring nsky_dome_vertex_data::get_vertex_type_name() const
	{
		return _T("sky_dome_vertex");
	}

	nstring nsky_dome_vertex_data::get_vertex_factory_name() const
	{
		return _T("vfactory_sky_dome");
	}
	
	vector3 _set_vector(float alpha, float beta)
	{
		float x,y,z;
		sin_cos(alpha, y, z);
		x = z;

		float s, c;
		sin_cos(beta, s, c);
		x *= c;
		z *= s;

		return vector3(x,y,z);
	}

	void nsky_dome_vertex_data::init(float radius, int tesselation, index_buffer16& faces)
	{
		size_t num_vert = (tesselation+1)*(tesselation+1)+2;

		//-- 分配内存
		init_pos_stream(num_vert);

		vertex_element_define color_def = {0, 0, EVET_FLOAT4, EVEU_Color, 0};
		element_array color_def_array;
		color_def_array.push_back(color_def);
		m_color_stream.init(color_def_array, num_vert);

		m_normal_array.resize(num_vert);

		//-- 填充pos，normal
		vector3* pos_stream = (vector3*)(m_pos_stream.get_data());
		const float delta_uv = 1.0f / static_cast<float>( tesselation );
		float u, v;
		int s  = 0;

		// vertices				
		for ( int i=0; i<=tesselation; ++i ) // radial		
		{
			u = delta_uv * static_cast<float>( i );
			const float alpha = u * nPI * 2.0f;	

			for ( int j=0; j<=tesselation; ++j ) // vertical
			{
				v = delta_uv * static_cast<float>( j );
				const float beta = nPI * (v - 0.5f);
				vector3 normal = _set_vector( beta, alpha );

				m_normal_array[s] = normal;
				pos_stream[s] = normal * radius;				
				s++;
			}
		}// end of for

		vector3 normal( 0.0f, -1.0f, 0.0f );
		m_normal_array[s] = normal;
		pos_stream[s] = normal * radius;
		s++;

		normal = vector3( 0.0f, 1.0f, 0.0f );
		m_normal_array[s] = normal;
		pos_stream[s] = normal * radius;

		//-- 填充triangle list数组
		triangle_face16 face;
		const int line = tesselation+1;

		for ( int i=0; i<tesselation; ++i ) 
		{
			int base = i * line;
			for ( int j=0; j<tesselation; ++j ) 
			{
				if ( j<tesselation-1 ) 
				{
					face.index[ 0 ] = base;
					face.index[ 1 ] = base+1;
					face.index[ 2 ] = base+line+1;
					faces.append_face(face);					
				}			
				if ( j>0 ) 
				{
					face.index[ 0 ] = base+line+1;
					face.index[ 1 ] = base+line;			
					face.index[ 2 ] = base;			
					faces.append_face(face);
				}
				base++;
			}
		}// end of for
	}

	vector4 _convert_XYZ2RGB(float cX, float cY, float cZ)
	{
		return vector4(3.240479f * cX - 1.537150f * cY - 0.498535f * cZ,
			-0.969256f * cX + 1.875992f * cY + 0.041556f * cZ,
			0.055648f * cX - 0.204043f * cY + 1.057311f * cZ,
			1);  
	}

	// minimum of 3 elements
	template <class Type>
	inline Type _get_min( const Type &a, const Type &b, const Type &c ) 
	{
		if ( a < b ) {
			if ( a < c ) return a; else return c;
		} else {
			if ( b < c ) return b; else return c;
		}
	}

	// maximum of 3 elements
	template <class Type>
	inline Type _get_max( const Type &a, const Type &b, const Type &c ) {
		if ( a > b ) {
			if ( a > c ) return a; else return c;
		} else {
			if ( b > c ) return b; else	return c;
		}
	}

	vector4 _convert_RGB2HSV(float cR, float cG, float cB) 
	{
		vector4 result;
		
		const float minCol = _get_min( cR, cG, cB );
		const float maxCol = _get_max( cR, cG, cB );
		const float delta = maxCol - minCol;

		float cHue, cSaturation;
		float cValue = maxCol;				// Value			
		if ( !is_zero( maxCol ) ) 
		{
			cSaturation = delta / maxCol;	// Saturation
		} 
		else 							// r = g = b = 0							
		{
			cSaturation = 0;			// Saturation = 0
			cHue = -1;				// Hue = undefined
			return vector4(cHue, cSaturation, cValue, 1);
		}

		if ( is_zero(cR - maxCol) ) 
			cHue = ( cG - cB ) / delta;	// between yellow & magenta
		else if ( is_zero(cG - maxCol) ) 
			cHue = 2.0f + ( cB - cR ) / delta;	// between cyan & yellow
		else 
			cHue = 4.0f + ( cR - cG ) / delta;		// between magenta & cyan
		cHue *= 60.0f;								// degrees
		if ( cHue < 0.0f ) 
			cHue += 360.0f;	// positive

		return vector4(cHue, cSaturation, cValue, 1);
	}


	vector4 _convert_HSV2RGB(float cHue, float cSaturation, float cValue) 
	{
		if( is_zero( cSaturation ) ) 		// achromatic (grey)		
		{
			return vector4(cValue, cValue, cValue, 1);
		}

		cHue /= 60.0f;							// sector 0 to 5
		const int sector = floor( cHue );

		const float f = cHue - static_cast<float>( sector );	// factorial part of hue
		const float p = cValue * ( 1.0f - cSaturation );
		const float q = cValue * ( 1.0f - cSaturation * f );
		const float t = cValue * ( 1.0f - cSaturation * ( 1.0f - f ) );
		switch( sector ) 
		{
		case 0:
			return vector4( cValue, t, p , 1);			
		case 1:
			return vector4( q, cValue, p , 1);			
		case 2:
			return vector4( p, cValue, t, 1 );			
		case 3:
			return vector4( p, q, cValue, 1 );			
		case 4:
			return vector4( t, p, cValue, 1 );			
		default:		// case 5:
			return vector4( cValue, p, q, 1);			
		}
	}

	vector3 ndynamic_sky_dome::get_sun_dir() const
	{		
		return vec_normalize(
			_set_vector( nHalfPI - m_theta_sun, m_phi_sun )
			);
	}

	void ndynamic_sky_dome::rebuild_colors(bool update_render_mesh)
	{
		const unsigned int vertexCount = m_vert_data.get_num_vert();
		vector4* vert_color = (vector4*)m_vert_data.get_stream(1)->get_data();
		std::vector<vector3>& normal_array = m_vert_data.get_normal_array();

		// get vertex color
		for ( int i = 0; i<vertexCount; ++i ) 
		{			
			vert_color[i] = get_vertex_color(normal_array[i]);
		}
		vector4 sc = get_vertex_color(get_sun_dir());
		m_sun_color = color4f(sc.x, sc.y, sc.z, sc.w);

		//--
		if (update_render_mesh)
		{
			m_mesh->update_vertices(&m_vert_data);
		}
	}

	vector4 ndynamic_sky_dome::get_vertex_color(const vector3& vert_normal)
	{
		// get zenith luminance
		const float chi = ( (4.0f / 9.0f) - (m_turbidity / 120.0f) ) * ( nPI - (2.0f * m_theta_sun) );
		float zenithLuminance = ( (4.0453f * m_turbidity) - 4.9710f ) * tanf( chi ) - (0.2155f * m_turbidity) + 2.4192f; 

		// get x / y zenith
		float zenithX = get_zenith( s_zenithXmatrix, m_theta_sun, m_turbidity );
		float zenithY = get_zenith( s_zenithYmatrix, m_theta_sun, m_turbidity );

		// get perez function parametrs
		float perezLuminance[5], perezX[5], perezY[5];  
		get_perez( perezLuminance, s_distributionLuminance, m_turbidity );
		get_perez( perezX, s_distributionXcomp, m_turbidity );
		get_perez( perezY, s_distributionYcomp, m_turbidity );

		// make some precalculation
		zenithX = perez_functionO1( perezX, m_theta_sun, zenithX );
		zenithY = perez_functionO1( perezY, m_theta_sun, zenithY );
		zenithLuminance = perez_functionO1( perezLuminance, m_theta_sun, zenithLuminance );

		// build sun direction vector
		vector3 sunDirection = _set_vector( nHalfPI - m_theta_sun, m_phi_sun );
		sunDirection = vec_normalize(sunDirection);
	
		vector4 color, colorTemp;
		vector3 vertex = vert_normal;

		// angle between sun and vertex
		const float gamma = acosf( vec_dot( vertex, sunDirection ) );

		// warning : major hack!!! .. i had to do something with values under horizon
		//vertex.y = Clamp<float>( vertex.y, 0.05f, 1.0f );
		if ( vertex.y < 0.05f ) 
			vertex.y = 0.05f;

		/*
		from paper:
		const float theta = arccos( vertex.y );
		const float iCosTheta = 1.0f / cosf( theta );
		optimized:
		iCosTheta = 
		= 1.0f / cosf( arccos( vertex.y ) );
		= 1.0f / vertex.y;
		*/
		const float iCosTheta = 1.0f / vertex.y;
		float cosGamma2 = cosf( gamma ) ;
		cosGamma2 *= cosGamma2;

		// Compute x,y values  
		const float x = perez_functionO2( perezX, iCosTheta, gamma, cosGamma2, zenithX );
		const float y = perez_functionO2( perezY, iCosTheta, gamma, cosGamma2, zenithY );

		// luminance(Y) for clear & overcast sky
		const float Yclear = perez_functionO2( perezLuminance, iCosTheta, gamma, cosGamma2, zenithLuminance );
		const float Yover = ( 1.0f + 2.0f * vertex.y ) / 3.0f;

		const float Y = linear_lerp<float>( Yclear, Yover, m_overcast );
		const float X = (x / y) * Y;  
		const float Z = ((1.0f - x - y) / y) * Y;

		colorTemp = vector4( X, Y, Z, 1 );
		color = _convert_XYZ2RGB(X, Y, Z);						

		colorTemp = _convert_RGB2HSV(color.x, color.y, color.z);
		if ( m_linear_exp_control ) 	// linear scale
		{
			colorTemp.z *= m_exp_factor;
		} 
		else 					// exp scale
		{
			colorTemp.z = 1.0f - exp( -m_exp_factor * colorTemp.z );  
		}
		color = _convert_HSV2RGB(colorTemp.x, colorTemp.y, colorTemp.z);

		// gamma control		
		color.x = powf( color.x, m_gamma_correction );
		color.y = powf( color.y, m_gamma_correction );
		color.z = powf( color.z, m_gamma_correction );
		color.w = 1;


		return color;
	}



	////////////////////////////////////////////////////////////////////////////
	//	class ndynamic_sky_dome
	////////////////////////////////////////////////////////////////////////////
	ndynamic_sky_dome::ndynamic_sky_dome(const nstring& name_str):nsky_component(name_str)
		,m_mtl(_T("sky_dome_mtl"))
	{
		m_theta_sun = 75*nPI/180.0f;
		m_phi_sun = 0;
		m_turbidity = 5;
		m_linear_exp_control = true;
		m_exp_factor = 1.0f/18;
		m_gamma_correction = 1.0f/2.5f;
		m_overcast = 0;

		m_radius = 0;
		m_tesselation = 0;
	}

	ndynamic_sky_dome::~ndynamic_sky_dome(void)
	{
	}

	nmaterial_base* ndynamic_sky_dome::get_material(int lod, int mtl_id)
	{
		(void)lod;
		(void)mtl_id;
		return &m_mtl;		
	}

	nrender_mesh* ndynamic_sky_dome::get_render_mesh(int lod)
	{
		return m_mesh.get();
	}

	void ndynamic_sky_dome::create(float radius, int tesselation)
	{
		index_buffer16 faces;
		m_vert_data.init(radius, tesselation, faces);
		//-- 初始化计算颜色
		rebuild_colors(false);

		//-- create render mesh		
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh.reset( rres_mgr->alloc_dynamic_mesh_indexed() );
		m_mesh->create(EDraw_TriangleList, &m_vert_data, &faces, 0);

		//-- create material
		nmaterial_script::ptr mtl_temp = 
			nresource_manager::instance()->load_material_template_script(
			resource_location(_T("engine_data:material/SKY_dome.hlsl"))
			);
		m_mtl.create(mtl_temp);

		//--
		m_radius = radius;
		m_tesselation = tesselation;
	}

	void ndynamic_sky_dome::get_perez( float *perez, float distribution[ 5 ][ 2 ], const float turbidity ) 
	{
		perez[ 0 ] = distribution[ 0 ][ 0 ] * turbidity + distribution[ 0 ][ 1 ];
		perez[ 1 ] = distribution[ 1 ][ 0 ] * turbidity + distribution[ 1 ][ 1 ];
		perez[ 2 ] = distribution[ 2 ][ 0 ] * turbidity + distribution[ 2 ][ 1 ];
		perez[ 3 ] = distribution[ 3 ][ 0 ] * turbidity + distribution[ 3 ][ 1 ];
		perez[ 4 ] = distribution[ 4 ][ 0 ] * turbidity + distribution[ 4 ][ 1 ];
	}

	float ndynamic_sky_dome::get_zenith( float zenithMatrix[ 3 ][ 4 ], const float theta, const float turbidity ) 
	{
		const float theta2 = theta*theta;
		const float theta3 = theta*theta2;

		return	( zenithMatrix[0][0] * theta3 + zenithMatrix[0][1] * theta2 + zenithMatrix[0][2] * theta + zenithMatrix[0][3]) * turbidity * turbidity +
			( zenithMatrix[1][0] * theta3 + zenithMatrix[1][1] * theta2 + zenithMatrix[1][2] * theta + zenithMatrix[1][3]) * turbidity +
			( zenithMatrix[2][0] * theta3 + zenithMatrix[2][1] * theta2 + zenithMatrix[2][2] * theta + zenithMatrix[2][3]);

	}

	float ndynamic_sky_dome::perez_functionO1( float perezCoeffs[ 5 ], const float thetaSun, const float zenithVal ) 
	{
		const float val = ( 1.0f + perezCoeffs[ 0 ] * exp( perezCoeffs[ 1 ] ) ) *
			( 1.0f + perezCoeffs[ 2 ] * exp( perezCoeffs[ 3 ] * thetaSun ) + perezCoeffs[ 4 ] * sqare( cosf( thetaSun ) ) );
		return zenithVal / val;
	}

	float ndynamic_sky_dome::perez_functionO2( float perezCoeffs[ 5 ], const float iCosTheta, const float gamma, const float cosGamma2, const float zenithVal ) 
	{
		// iCosTheta = 1.0f / cosf(theta)
		// cosGamma2 = SQR( cosf( gamma ) )
		return zenithVal * ( 1.0f + perezCoeffs[ 0 ] * exp( perezCoeffs[ 1 ] * iCosTheta ) ) * 
			( 1.0f + perezCoeffs[ 2 ] * exp( perezCoeffs[ 3 ] * gamma ) + perezCoeffs[ 4 ] * cosGamma2 );
	}

	void ndynamic_sky_dome::update(float delta_time, const nviewport& view)
	{
		(void)delta_time;
		mat_set_translation(m_mat_world, view.camera.get_eye_pos());
	}

	enum EDepthGroup ndynamic_sky_dome::get_depth_group() const	
	{
		return EDG_Foreground; 
	}

	void ndynamic_sky_dome::serialize(narchive& ar)
	{
		nsky_component::serialize(ar);

		nSERIALIZE(ar, m_radius);
		nSERIALIZE(ar, m_tesselation);		
		nSERIALIZE(ar, m_theta_sun);
		nSERIALIZE(ar, m_phi_sun);
		nSERIALIZE(ar, m_turbidity);
		nSERIALIZE(ar, m_linear_exp_control);
		nSERIALIZE(ar, m_exp_factor);		
		nSERIALIZE(ar, m_overcast);
		nSERIALIZE(ar, m_gamma_correction);
	}

	void ndynamic_sky_dome::_level_loaded(nactor* owner)
	{
		nsky_component::_level_loaded(owner);

		create(m_radius, m_tesselation);
	}

	void ndynamic_sky_dome::_destroy()
	{
		m_mesh.reset();
	}
}//namespace nexus