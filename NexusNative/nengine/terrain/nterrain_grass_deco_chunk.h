#ifndef _NEXUS_TERRAIN_GRASS_DECO_CHUNK_H_
#define _NEXUS_TERRAIN_GRASS_DECO_CHUNK_H_

#include "nterrain_deco_chunk_base.h"
#include "../mesh/mesh_lod.h"
#include "../renderer/nrender_static_mesh_indexed.h"
#include "nterrain_deco_grass.h"

namespace nexus
{
	//静态模型装饰层的对象实例
	class nAPI grass_deco_object:public terrain_deco_object
	{
	public:
		void	move(const vector3& pos,const vector3& rot,const vector3& scale)
		{
			m_pos=pos;	m_rotation=rot;	m_scaling=scale;
			update_matrix();
		}
		virtual void update_matrix()
		{
			to_matrix(m_matrix);
		}
		grass_deco_object(nterrain_deco_layer* owner)
		{
			m_owner=owner;
		}
	public:
		nterrain_deco_layer*	m_owner;
		matrix44	m_matrix;
	};	

	//草叶装饰层在每一个chunk中的代理
	class nAPI nterrain_grass_deco_chunk:public nterrain_deco_chunk_base
	{
	public:
		//在指定坐标处添加一个新的实例
		virtual terrain_deco_object::ptr	add_instance(int x,int z,float wx,float wz);

		//将需要渲染的mesh-element传入列表
		virtual void	get_mesh_element(int lod,vector<mesh_element>& mesh_elements,nmesh_component* comp);

		//分布图发生改变时的处理
		virtual bool	on_density_map_changed();
		virtual bool	on_density_map_changed(nterrain_deco_layer* layer,const nrect& region);

		//设置材质
		virtual void	set_material(nmtl_base::ptr material);

		//设置面片类型
		virtual void	set_cross(bool value)	{m_grass_buffer->set_cross(value);}

		//设备丢失时的处理
		virtual void on_device_lost()	{m_grass_buffer->on_device_lost();}
		//设备重置时的处理
		virtual void on_device_reset()	{m_grass_buffer->on_device_reset();}

	private:
		grass_deco_buffer::ptr	m_grass_buffer;
	public:
		nterrain_grass_deco_chunk(nterrain_deco_layer* owner,nterrain_chunk* terrain_chunk);
		~nterrain_grass_deco_chunk();
	};

	//===================================================================================
	//草叶装饰层.
	//基类中的缩放的x-y分量视为草叶面片的宽度和高度，z分量忽略
	class nAPI nterrain_grass_deco_layer:public nterrain_deco_layer
	{
	public:
		typedef boost::shared_ptr<nterrain_grass_deco_layer>	ptr;

		nDECLARE_CLASS(nterrain_grass_deco_layer);
		nterrain_grass_deco_layer();
		virtual ~nterrain_grass_deco_layer();

		//生成一个deco chunk
		virtual boost::shared_ptr<nterrain_deco_chunk_base> create_deco_chunk(nterrain_chunk* terrain_chunk,int x,int y,int width,int height);

		//每个loc是一个材质
		virtual void	set_resource_loc(const resource_location& loc);

		//获取和设置面片的宽度
		virtual float	get_width()	{return m_width;}
		virtual void	set_width(float value)	{m_width=value;rebuild();}

		//获取和设置面片的高度
		virtual float	get_height()	{return m_height;}
		virtual void	set_height(float value)	{m_height=value;rebuild();}

		//获取和设置是否是十字交叉面片
		virtual bool	get_cross()		{return m_cross;}
		virtual void	set_cross(bool value);

		//序列化
		virtual void	serialize(narchive& ar);
	protected:
		nmtl_base::ptr	m_material;
		float	m_width;
		float	m_height;
		bool	m_cross;
	};
	
};

#endif