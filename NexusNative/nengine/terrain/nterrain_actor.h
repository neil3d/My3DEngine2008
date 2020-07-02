/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_TERRAIN_ACTOR_H_
#define _NEXUS_TERRAIN_ACTOR_H_
#include <boost/pool/object_pool.hpp>
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../renderer/nrender_resource.h"
#include "nheight_map.h"
#include "nterrain_mtl.h"
#include "../framework/world_define.h"

namespace nexus
{
	template<typename T>
	inline T lerp(const T& a, const T& b, const T& alpha)
	{
		return (T)(a + alpha*(b-a));
	}

	template<typename T>
	inline T quad_lerp(const T& p00, const T& p10, const T& p01, const T& p11,
		float u, float v)
	{
		if( u > v )
		{
			if( v<1.0f )
				return lerp( lerp(p00, p11, v),	lerp(p10, p11, v),
							(u-v)/(1.0f-v)	);
			else
				return p11;
		}
		else
		{
			if( v>0.0f )
				return lerp( lerp(p00, p01, v), lerp(p00, p11, v),
								u/v );
			else
				return p00;
		}
	}

	template<typename T, typename U>
	inline void _clip_rect(rect<T>& rc,  U width, U height)
	{
		_clip_rect(rc.left, rc.top, rc.right, rc.bottom, width, height);
	}

	template<typename T, typename U>
	inline void _clip_rect(T& left, T& top, T& right, T& bottom, U width, U height)
	{
		if( right > width)
			right = width;
		if( bottom > height)
			bottom = height;

		if(left < 0)
			left = 0;
		if(top < 0)
			top = 0;
	}

	class nquad_tree_terrain;
	class nterrain_water;

	/**
	 *	地形对象，管理height map以及渲染用的component
	*/
	class nAPI nterrain_actor
		: public nactor
	{
	public:
		typedef shared_ptr<nterrain_actor> ptr;

		nterrain_actor(const nstring& name_str);
		virtual ~nterrain_actor(void);

		// 不支持rotation
		virtual void move(const vector3& pos, const vector3& rot, const vector3& scale)
		{
			(void)rot;
			nactor::move(pos, vector3::zero, scale);
		}
		void move(const vector3& pos, const vector3& scale)
		{			
			nactor::move(pos, vector3::zero, scale);
		}

		void create(size_t w, size_t h, unsigned short init_h, size_t chunk_size);		
		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);
		void import_heightmap(const nstring& img_file_name);

		void create_material_basic(const resource_location& texture_loc);
		ntexture_splatting::ptr create_texture_splatting(size_t alpha_w, size_t alpha_h);
		nmaterial_base* get_material();

		void create_water(int water_h/*unscaled*/, size_t chunk_size);
		void create_water_material_basic(const resource_location& texture_loc);
		void create_water_material_natural(int render_target_w, int render_target_h,
			const resource_location& detail_map, const resource_location& bump_map);

		virtual bool line_check(ncheck_result& ret, const vector3& start, const vector3& end);

		npoint world2tile(float wx, float wz) const
		{
			const vector3& scale = m_space.scale;
			const vector3& pos = m_space.location;

			wx -= pos.x;
			wz -= pos.z;
			return npoint( long(wx/scale.x), long(wz/scale.z) );
		}

		vector3 tile2world(int tx, int tz) const
		{
			const vector3& scale = m_space.scale;
			const vector3& pos = m_space.location;
			return vector3(tx*scale.x+pos.x, 
				get_height(tx, tz)+pos.y,
				tz*scale.z+pos.z);
		}

		float get_height(float wx, float wz) const;
		float get_height(int tx, int tz) const
		{
			if( m_height_map.is_valid() 
				&& m_height_map.is_valid_index(tx,tz) ) 
				return m_height_map.get_value(tx,tz)*m_space.scale.y;
			else
				return 0;
		}

		int get_raw_height(int tx, int tz) const
		{
			if( m_height_map.is_valid() 
				&& m_height_map.is_valid_index(tx,tz) ) 
				return m_height_map.get_value(tx,tz);
			else
				return 0;
		}

		const vector3& get_scale() const	{	return m_space.scale;}

		/**	测试一个点是在地面以上还是以下
			@return 1 == up, -1 == low */
		int classify_side(const vector3& wpt);

		int get_map_width() const	{	return m_height_map.get_width();}
		int get_map_height() const	{	return m_height_map.get_height();}
		size_t get_chunk_size() const {	return m_chunk_size;}
		nheight_map16* get_height_map()	{	return &m_height_map;}
		nrender_heightmap* get_height_map_texture()	{	return m_heightmap_tex.get();}

		void post_heightmap_change(const nrect& region);

		virtual void _destroy();
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nlevel* level_ptr);
	protected:
		nheight_map16	m_height_map;
		render_res_ptr<nrender_heightmap>
						m_heightmap_tex;

		size_t			m_chunk_size;
				
		shared_ptr<nquad_tree_terrain>	m_trn_component;
		shared_ptr<nterrain_water>		m_water_component;

		nDECLARE_NAMED_CLASS(nterrain_actor)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_ACTOR_H_