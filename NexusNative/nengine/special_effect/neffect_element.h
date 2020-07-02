#ifndef _NEXUS_EFFECT_ELEMENT_H_
#define _NEXUS_EFFECT_ELEMENT_H_
#include "ncore.h"
#include "../renderer/nrender_mesh.h"
#include "../material/nmtl_static.h"
#include "../framework/view_info.h"
#include "ndistribution.h"
#include "nsub_uv.h"

namespace nexus
{
	class neffect_element;

	/**
	 *	管理单个特效元素的实例数据，由speclial effect instance管理
	 *	@负责管理一个特效元素的当前时间的状态，以及渲染用的资源等
	 *	@see class nspecial_effect_instance
	*/
	class neffect_element_instance
	{
	public:
		typedef boost::shared_ptr<neffect_element_instance> ptr;

		neffect_element_instance(void):m_pos(0,0,0),m_rot(0,0,0)
		{			
			m_active = false;
		}

		virtual ~neffect_element_instance(void)	{}

		virtual void init(shared_ptr<neffect_element> def);
		
		//-- 计算内部Mesh等渲染数据；外部确认对象可见（需要渲染）时调用。
		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat) = 0;

		virtual nrender_mesh* get_render_mesh() const = 0;
		virtual nmtl_base* get_material(int mtl_id) const;		

	public:
		bool				m_active;		// 是否已经被激活（正在播放）		
		nmtl_base::ptr		m_mtl;

		vector3		m_pos;
		vector3		m_revolution;
		vector3		m_rot;
	};

	/**
	 *	特效元素定义基类
	 *	@remarks 定义特效元素的静态数据，可以由编辑器编辑生成；有resource对象管理；
	 *	@see class nresource_special_effect
	*/
	class nAPI neffect_element
		: public nobject
	{
	public:
		typedef boost::shared_ptr<neffect_element> ptr;

		neffect_element(void);
		virtual ~neffect_element(void);

		virtual void init(const nstring& name);

		virtual neffect_element_instance::ptr create_instance() = 0;

		// 只更新特效内部状态，不更新渲染资源
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time) = 0;

		//获取材质
		nmtl_base::ptr get_material() const
		{
			return m_mtl;
		}

		//设置材质纹理
		void set_material_texture(resource_location loc)
		{
			m_Texture=loc;
			if(m_mtl.get()!=NULL && m_Texture.is_valid())
			{
				m_mtl->set_texture2d_parameter("MTL_DiffuseMap",m_Texture);
			}
		}

		//是否显示
		bool	is_visible(){return m_visible;}

		//设置是否显示
		void	set_visible(bool v){m_visible=v;}

		//获取名字
		const nstring& get_name() const	{	return m_name;}

		//序列化
		virtual void serialize(narchive& ar);

	public:
		//-- 静态数据
		nmtl_static::ptr	m_mtl;

		//粒子的材质纹理
		resource_location	m_Texture;

		//细分纹理
		nsub_uv		m_sub_uv;

		nstring		m_name;
		float		m_start_time;	// 在整个特效播放了多久之后才播放自己这个元素
		vector3		m_init_pos;
		vector3		m_init_revolution;		// 公转；todo：是否加入自转？
		vector3		m_init_rot;	//自转，在公告板模式下，自转只有一个转轴起作用

		//是否显示
		bool		m_visible;

		//-- 变化数据
		//----位置、旋转速度
		ndistribution<vector3>::ptr	m_vel;			//	移动速度		
		ndistribution<vector3>::ptr	m_revolution_vel;	//	公转速度
		ndistribution<vector3>::ptr	m_rotate_vel;	//	自转速度
		float	m_gravity;		//重力加速度，0表示不受重力影响

	protected:
		void init_instance_base(neffect_element_instance::ptr inst);
		bool update_instance_base(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		nDECLARE_VIRTUAL_CLASS(neffect_element)
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_ELEMENT_H_