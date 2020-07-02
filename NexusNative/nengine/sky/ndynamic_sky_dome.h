#ifndef _NEXUS_DYNAMIC_SKY_DOME_H_
#define _NEXUS_DYNAMIC_SKY_DOME_H_
#include "sky_base.h"
#include "../mesh/mesh_vertex_data.h"
#include "../material/nmaterial.h"
#include "../renderer/nrender_dynamic_mesh_indexed.h"

namespace nexus
{
	class nsky_dome_vertex_data
		: public nmesh_vertex_data
	{
	public:
		nsky_dome_vertex_data(void);
		virtual ~nsky_dome_vertex_data(void);

		//--
		virtual size_t get_num_stream() const	{	return 2;}
		virtual vertex_stream* get_stream(size_t stream_index)	
		{
			if(stream_index == 0)
				return &m_pos_stream;
			else
				return &m_color_stream;
		}
		virtual nstring get_vertex_type_name() const;
		virtual nstring get_vertex_factory_name() const;

		//--
		void init(float radius, int tesselation, index_buffer16& faces);		
		std::vector<vector3>& get_normal_array()	{	return m_normal_array;}
	private:
		vertex_stream			m_color_stream;
		std::vector<vector3>	m_normal_array;	// 渲染时不用，但是需要用来更新颜色
	};

	class nAPI ndynamic_sky_dome
		: public nsky_component
	{
	public:
		typedef boost::shared_ptr<ndynamic_sky_dome> ptr;

		explicit ndynamic_sky_dome(const nstring& name_str);
		virtual ~ndynamic_sky_dome(void);

		void create(float radius, int tesselation);
		void rebuild_colors()
		{
			rebuild_colors(true);
		}

		virtual enum EDepthGroup get_depth_group() const;
		virtual void update(float delta_time, const nviewport& view);
		virtual nmaterial_base* get_material(int lod, int mtl_id);		
		virtual nrender_mesh* get_render_mesh(int lod);		

		vector3 get_sun_dir() const;
		color4f	get_sun_color() const {	return m_sun_color;}
		/**
		 *	设置太阳的角度
		 *	@param theta 0~PI
		 *	@param phi0~2PI
		*/
		void set_sun_dir(float theta, float phi)
		{
			m_theta_sun = theta;
			m_phi_sun = phi;
		}

		// 0~512
		void set_turbidity(float _turbidity)
		{
			m_turbidity = _turbidity;
		}

		// 0~1
		void set_overcast(float _overcast)
		{
			m_overcast = _overcast;
		}

		void set_exposure(bool linear, float factor )
		{
			m_linear_exp_control = linear;
			m_exp_factor = factor;
		}

		void set_gama(float gama)
		{
			m_gamma_correction = gama;
		}

		virtual void _destroy();
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nactor* owner);
	private:
		nsky_dome_vertex_data	m_vert_data;
		render_res_ptr<nrender_dynamic_mesh_indexed>	m_mesh;
		nmaterial	m_mtl;

		void rebuild_colors(bool update_render_mesh);
		vector4 get_vertex_color(const vector3& vert_normal);
	private:
		float	m_radius;
		int		m_tesselation;
		// shading parametrs
		float m_theta_sun, m_phi_sun;
		float m_turbidity;
		bool m_linear_exp_control;
		float m_exp_factor;		
		float m_overcast;
		float m_gamma_correction;

		color4f	m_sun_color;

		static float s_distributionLuminance[5][2];
		static float s_distributionXcomp[5][2];
		static float s_distributionYcomp[5][2];

		static float s_zenithXmatrix[3][4];
		static float s_zenithYmatrix[3][4];

		// coloring
		void get_perez( float *perez, float distribution[5][2], const float turbidity );
		float get_zenith( float zenithMatrix[3][4], const float theta, const float turbidity );		
		float perez_functionO1( float perezCoeffs[5], const float thetaSun, const float zenithVal );
		float perez_functionO2( float perezCoeffs[5], const float iCosTheta, const float gamma, const float cosGamma2, const float zenithVal );

		nDECLARE_NAMED_CLASS(ndynamic_sky_dome)
	};
}//namespace nexus

#endif //_NEXUS_DYNAMIC_SKY_DOME_H_