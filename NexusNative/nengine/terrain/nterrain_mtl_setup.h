/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	April, 2010
*/
#ifndef _NEXUS_TERRAIN_MTL_SETUP_H_
#define _NEXUS_TERRAIN_MTL_SETUP_H_
#include "ncore.h"
#include "../resource/nresource.h"
#include "nterrain_mtl.h"

namespace nexus
{
	class nterrain_actor;

	const int TERRAIN_MTL_MAX_LAYER = 16;

	/**
	 *	地形材质设置、编辑数据管理
	 *	@remarks 这是一种数据与表现层的material分开的设计
	*/
	struct layer_data_editor
	{
		// 可通过编辑器编辑的数据
		vector2					uv_scale;
		float					uv_rotate;		
		float					spec;
		float					spec_power;
		nresource_texture::ptr	diffuse_tex;
		nresource_texture::ptr	normal_tex;

		layer_data_editor()
			: uv_scale(vector2(1,1)),
			uv_rotate(0),
			spec(0),
			spec_power(32)
		{
		}


		nstring get_name() const	// 使用diffuse贴图的location作为名称
		{
			if( diffuse_tex )
				return diffuse_tex->get_location().to_string();
			else
				return nstring(_T("EmptyLayer"));
		}
	};

	struct layer_data : public layer_data_editor
	{
		nalpha_map::ptr		alpha_map;			

		layer_data()
			:layer_data_editor()
		{}

		void copy_from(const layer_data_editor& other)
		{
			this->uv_scale		= other.uv_scale;
			this->uv_rotate		= other.uv_rotate;
			this->spec			= other.spec;
			this->spec_power	= other.spec_power;
			this->diffuse_tex	= other.diffuse_tex;
			this->normal_tex	= other.normal_tex;
		}
	};

	class nAPI nterrain_mtl_setup :	public nobject
	{
	public:
		nterrain_mtl_setup(void);
		virtual ~nterrain_mtl_setup(void);

		//!	清空内部数据，并绑定父对象
		void reset(nterrain_actor* owner);		

		//!	创建：设置基本贴图、参数等
		void create(const resource_location& basic_texture_loc, int alpha_map_w, int aplha_map_h);
				
		//! 总共有多少个图层
		size_t get_num_layers() const	{	return m_layers.size();}

		//! 取得图层除了alpha map外的其他数据
		const layer_data& get_layer_data(size_t i);
		void set_layer_data(size_t i,const layer_data_editor& ld );

		//! 根据所有取得图层的名称
		nstring get_layer_name(size_t i) const;

		//! 根据图层的名称取得图层的index
		int get_layer_index(const nstring& layer_name) const;

		//! 根据图层名称取得对应的alpha map
		nalpha_map::ptr get_layer_alpha(const nstring& layer_name) const;

		//! 根据图层index取得对应的alpha map
		nalpha_map::ptr get_layer_alpha(size_t i) const;

		/** 新增一个图层
		 *	@return 返回新增图层的index；-1表示添加失败
		*/
		int add_layer(const layer_data_editor& ld);

		//! 更新一个图层的参数设置
		void update_layer();

		//! 删除一个图层
		void remove_layer(size_t i);

		//! 把一个图层向上移动
		void shift_up_layer(size_t i);

		//! 把一个图层向下移动
		void shift_down_layer(size_t i);

		//! 导入图层的alpha map
		void import_layer_alpha(const nstring& layer_name, const nstring& img_file_name);

		//! 对图层的指定区域产生随机的alpha值
		void generate_noise(const nstring& layer_name, nrect rc, int numOctaves, float amplitude, float frequency);

		//! 编辑器通知更新图层的alpha map
		void post_layer_alpha_change(const nstring& layer_name, nrect rc);

		//!	计算chunk对应的rect是否与某个图层相关（区域内任意像素的alpha大于阀值）
		bool calulate_alpha_relative(size_t layer_index, const nrect& rc);
		
		//! 根据内部数据创建基本地形材质对象
		nterrain_mtl_basic::ptr setup_render_mtl_low();

		//! 根据内部数据创建分层地形材质对象
		ntexture_splatting::ptr setup_render_mtl_high(int chunk_size);

		//! 序列化将各种值数据存入文档，每个alpha map单独存成一个raw文件
		virtual void serialize(narchive& ar);
	private:
		//! 根据图层名称查找
		int find_layer_by_name(const nstring& layer_name) const;		

		//! 计算alpha map单独采取时对应的文件名
		nstring get_serialize_alpha_map_name(narchive& ar, int layer_index) const;

	private:
		typedef std::vector<layer_data> st_layer_array;
		
		int				m_alpha_w,	// alpha map的尺寸
						m_alpha_h;
		st_layer_array	m_layers;	// 图层数据

		resource_location	m_basic_texture;	// 基本模式材质使用的贴图
		
		nterrain_actor*	m_owner;
	};
}//namespace nexus
#endif //_NEXUS_TERRAIN_MTL_SETUP_H_