#ifndef _NEXUS_TERRAIN_DECO_CHUNK_BASE_H_
#define _NEXUS_TERRAIN_DECO_CHUNK_BASE_H_
#include "ncore.h"
#include "../resource/nresource.h"
#include "../../ncore/height_map/nheight_map.h"
#include "../Actor/nmesh_component.h"
#include <set>

class Perlin;
namespace nexus
{
	class nterrain_actor;
	class nterrain_chunk;
	class nterrain_deco_layer;
	class nterrain_deco_chunk_base;
	typedef std::set<nterrain_chunk*> chunk_list;

	enum nAPI deco_layer_type
	{
		deco_layer_static_mesh=0,	//静态模型层
		deco_layer_grass,			//草叶层
	};

	//-----------------------------------------------------------------------------------
	struct nAPI terrain_deco_object
	{
		typedef boost::shared_ptr<terrain_deco_object>	ptr;

		terrain_deco_object():m_pos(0,0,0),m_rotation(0,0,0),m_scaling(1.0f,1.0f,1.0f)
		{}

		void	to_matrix(matrix44& matrix)
		{
			mat_set_scaling(matrix,m_scaling.x,m_scaling.y,m_scaling.z);
			mat_rotate_Z(matrix, m_rotation.z);
			mat_rotate_X(matrix, m_rotation.x);
			mat_rotate_Y(matrix, m_rotation.y);
			mat_translate(matrix,m_pos);
		}

		//实例关联的分布图坐标
		int		m_X,m_Y;
		//实例的位置
		vector3	m_pos;
		//实例的旋转
		vector3 m_rotation;
		//实例的缩放
		vector3 m_scaling;	
	};

	//-----------------------------------------------------------------------------------
	class nAPI nterrain_deco_chunk_base
	{
		friend class nterrain_deco_layer;
	public:
		typedef boost::shared_ptr<nterrain_deco_chunk_base>	ptr;

		//创建chunk
		void create(float x, float y, float width,float height);

		//分布图发生改变时的处理
		virtual bool	on_density_map_changed()=0;
		virtual bool	on_density_map_changed(nterrain_deco_layer* layer,const nrect& region)=0;

		//在指定坐标处添加一个新的实例，参数：分布图坐标，世界空间坐标
		virtual terrain_deco_object::ptr	add_instance(int x,int z,float wx,float wz)=0;

		//将需要渲染的mesh-element传入列表
		virtual void get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp)=0;

		//设备丢失时的处理
		virtual void on_device_lost()	{}
		//设备重置时的处理
		virtual void on_device_reset()	{}
		//-----------------------------------------------------------------------------------
		nterrain_deco_layer*		get_owner()	{return m_owner;}
	protected:
		nterrain_deco_layer*		m_owner;
		nterrain_chunk*				m_terrain_chunk;
		//chunk的位置和大小，因为分布图是可变大小的，所以这里用的是相对大小
		float		m_x,m_y;
		float		m_width,m_height;
		nrect		m_region;
		//-----------------------------------------------------------------------------------
		vector<terrain_deco_object::ptr>	m_object_list;
		typedef vector<terrain_deco_object::ptr>::iterator OI;
	public:
		nterrain_deco_chunk_base(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk);
		virtual ~nterrain_deco_chunk_base();
	};

	//-----------------------------------------------------------------------------------
	typedef nheight_map<terrain_deco_object*>	ndeco_map;

	class nAPI nterrain_deco_layer:public nobject
	{
	public:
		typedef boost::shared_ptr<nterrain_deco_layer> ptr;

		//初始化
		virtual void initialize(nterrain_actor* terrain);

		//生成一个deco chunk
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height);

		//获取和设置层的名字
		const	nstring&	get_name()			{return m_name;}
		virtual void	set_name(const nstring& value)	{m_name=value;}

		//获取和设置当前层的模型资源。如果设置了不同的资源，则会更新现有的对象模型
		const	resource_location&	get_resource_loc()	{return m_resource;}
		virtual void	set_resource_loc(const resource_location& loc)=0;

		//获取和设置默认的随机旋转值，该值代表对象创建时绕Y轴的随机旋转
		float	get_random_offset()	{return m_random_offset;}
		virtual void	set_random_offset(float value)	{m_random_offset=value;rebuild();}

		//获取和设置对象创建时在高度上的位移
		float	get_height_offset()	{return m_height_offset;}
		virtual void	set_height_offset(float value)	{m_height_offset=value;rebuild();}

		//获取和设置默认的随机旋转值，该值代表对象创建时绕Y轴的随机旋转
		float	get_random_rotation()	{return m_random_rotation;}
		virtual void	set_random_rotation(float value)	{m_random_rotation=value;rebuild();}

		//获取和设置随机缩放的最大值和最小值
		float	get_max_random_scale()	{return m_max_random_scale;}
		virtual void	set_max_random_scale(float value)	{m_max_random_scale=value;rebuild();}

		float	get_min_random_scale()	{return m_min_random_scale;}
		virtual void	set_min_random_scale(float value)	{m_min_random_scale=value;rebuild();}

		//获取和设置笔刷的密度
		float	get_density()	{return m_density;}
		virtual void	set_density(float value);

		//获取分布图
		nalpha_map::ptr	get_density_map()	{return m_density_map;}

		//获取网格大小
		size_t	get_grid_size()	{return m_grid_size;}
		void	set_grid_size(size_t value);

		//获取和设置随机数种子
		int		get_random_seed()	{return m_random_seed;}
		void	set_random_seed(int value);

		//获取地形对象
		nterrain_actor*	get_terrain_actor()	{return m_terrain;}

		//-----------------------------------------------------------------------------------
		//浓度图发生更新
		virtual void	on_update_density_map(const nrect& region,chunk_list& chunks);

		//重建整个层
		virtual void	rebuild();

		//测试
		virtual void	do_test();

		//序列化
		virtual void	serialize(narchive& ar);

	protected:
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_deco_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height)=0;
	protected:
		//层的名字
		nstring		m_name;

		//该层使用的资源,对静态模型层来说是模型文件路径，对于草叶来说是文理路径
		resource_location	m_resource;

		//默认的随机旋转值界限,如果取值为30，代表在-30~30度之间旋转。注意，这里使用的是度，不是弧度。
		float	m_random_rotation;

		//默认的随机偏移值,如果取100，代表xyz坐标都在-100~100的范围内随机偏移
		float	m_random_offset;

		//高度上的位移
		float	m_height_offset;

		//默认的随机缩放值
		float	m_min_random_scale;
		float	m_max_random_scale;

		//笔刷的密度
		float	m_density;

		//分布图的尺寸
		size_t	m_grid_size;

		//随机数种子
		int		m_random_seed;

		//分布图
		nalpha_map::ptr		m_density_map;

		//地形引用
		nterrain_actor*		m_terrain;

		//当前层拥有的chunk列表
		vector<boost::shared_ptr<nterrain_deco_chunk_base> > m_chunks;

	public:
		nDECLARE_VIRTUAL_CLASS(nterrain_deco_layer)
		nterrain_deco_layer();
		virtual ~nterrain_deco_layer();
	};

	
};

#endif