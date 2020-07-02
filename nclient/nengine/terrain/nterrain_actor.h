/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_TERRAIN_ACTOR_H_
#define _NEXUS_TERRAIN_ACTOR_H_
#include "../framework/nactor.h"
#include "../resource/nresource.h"
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

		void create(size_t w, size_t h, unsigned short init_h, size_t chunk_size);
		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);
		void import_heightmap(const nstring& img_file_name);

		void create_material_basic(const resource_location& texture_loc);
		ntexture_splatting::ptr create_texture_splatting(size_t alpha_w, size_t alpha_h);
		nmaterial_base* get_material();

		bool line_check(ncheck_result& ret, const vector3& start, const vector3& end);

		void set_scale(const vector3& scale);
		void set_position(const vector3& pos);
		
		npoint world2tile(float wx, float wz) const
		{
			wx -= m_pos.x;
			wz -= m_pos.z;
			return npoint( long(wx/m_scale.x), long(wz/m_scale.z) );
		}

		vector3 tile2world(int tx, int tz) const
		{
			return vector3(tx*m_scale.x+m_pos.x, 
				get_height(tx, tz)+m_pos.y,
				tz*m_scale.z+m_pos.z);
		}

		float get_height(float wx, float wz) const;
		float get_height(int tx, int tz) const
		{
			if( m_height_map.is_valid() 
				&& m_height_map.is_valid_index(tx,tz) ) 
				return m_height_map.get_value(tx,tz)*m_scale.y;
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

		/**	测试一个点是在地面以上还是以下
			@return 1 == up, -1 == low */
		int classify_side(const vector3& wpt);

		const vector3& get_scale() const	{	return m_scale; }
		int get_map_width() const	{	return m_height_map.get_width();}
		int get_map_height() const	{	return m_height_map.get_height();}
		nheight_map16* get_height_map()	{	return &m_height_map;}

		void post_heightmap_change(const nrect& region);

	protected:
		nheight_map16	m_height_map;
		vector3			m_scale,
						m_pos;
		
		shared_ptr<nquad_tree_terrain>	m_trn_component;

		nDECLARE_NAMED_CLASS(nterrain_actor)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_ACTOR_H_