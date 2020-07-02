/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	May, 2008
*/

#ifndef _NEXUS_TERRAIN_ACTOR_H_
#define _NEXUS_TERRAIN_ACTOR_H_
#include <boost/pool/object_pool.hpp>
#include "../../ncore/height_map/nheight_map.h"
#include "../../ncore/height_map/nbit_map.h"
#include "../framework/nactor.h"
#include "../resource/nresource.h"
#include "../renderer/nrender_resource.h"
#include "../framework/world_define.h"
#include "../physics/nphys_body_instance.h"
#include "nterrain_mtl_setup.h"
#include "nterrain_deco_setup.h"

namespace nexus
{
	class nterrain_chunk;

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

		//地形不可复制
		virtual nactor::ptr clone();

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

		//-- 高度图操作接口
		void create(size_t w, size_t h, unsigned short init_h, size_t chunk_size);		
		void generate_noise(nrect rc, int numOctaves, float amplitude, float frequency);
		void import_heightmap(const nstring& img_file_name);		

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

		virtual bool line_check(ncheck_result& ret, const vector3& start, const vector3& end, ELineCheckType check_type);
		/**	测试一个点是在地面以上还是以下
			@return 1 == up, -1 == low */
		int classify_side(const vector3& wpt);

		//-- 内部数据访问接口
		int get_map_width() const		{	return m_height_map.get_width();}
		int get_map_height() const		{	return m_height_map.get_height();}
		size_t get_chunk_size() const	{	return m_chunk_size;}
		nheight_map16* get_height_map()	{	return &m_height_map;}
		nbit_map* get_viz_map()			{	return &m_viz_map;}
		nterrain_mtl_setup* get_material()	{	return &m_mtl_setup; }
		nterrain_deco_setup* get_deco()	{	return &m_deco_setup;	}
		//获取指定世界空间坐标所属的chunk
		shared_ptr<nterrain_chunk>	get_chunk(float x,float y);
		//获取chunk的数量
		size_t	get_chunk_count()	{return m_chunks.size();}
		//获取指定索引的chunk
		shared_ptr<nterrain_chunk>	get_chunk(size_t index)	{return m_chunks[index];}

		virtual void _destroy();
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nlevel* level_ptr);

		// physics interfaces
		virtual void init_phys();
		virtual void simulate_phys(float delta_time);
		virtual void release_phys();

		//-- 引擎内部使用的public接口函数
	public:
		void _post_material_create();
		void _post_layer_alpha_change(const nstring& layer_name, const nrect& region);
		void _post_terrain_layer_change();
		void post_heightmap_change(const nrect& region);
		void post_vizmap_change(const nrect& region);
		//void post_deco_layer_change(nterrain_deco_layer* layer,const nrect& region);
	protected:
		void create_chunks();
	protected:
		nheight_map16		m_height_map;	// 地形高度图数据
		nbit_map			m_viz_map;		// 哪个格子不可见（用来在地形上挖洞）
		nterrain_mtl_setup	m_mtl_setup;	// 地形材质设置、编辑数据
		nterrain_deco_setup	m_deco_setup;	// 地形装饰层编辑
		size_t				m_chunk_size;	// 单个用来渲染的地形块的大小

		//-- 渲染数据
		typedef std::vector<boost::shared_ptr<nterrain_chunk> > st_chunk_array;
		st_chunk_array		m_chunks;

		//-- 物理系统数据
		nphys_body_instance::ptr		m_phys_body_instance;
		NxHeightField*					m_phys_height_field;

		nDECLARE_NAMED_CLASS(nterrain_actor)
	};
}//namespace nexus

#endif //_NEXUS_TERRAIN_ACTOR_H_