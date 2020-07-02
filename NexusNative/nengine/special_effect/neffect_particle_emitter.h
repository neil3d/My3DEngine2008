#ifndef _NEXUS_EFFECT_PARTICLE_EMITTER_H_
#define _NEXUS_EFFECT_PARTICLE_EMITTER_H_
#include "neffect_element.h"
#include "quad_face.h"
#include "neffect_vertex_data.h"
#include "../renderer/nrender_dynamic_mesh.h"

namespace nexus
{
	class neffect_particle_emitter;

	//粒子发射器的形状，有发射器形状参数vector3(x,y,z)控制，对各形状的控制如下：
	//ESFX_SphereShape：	x=X Radius;		y=Y Radius;		z=Z Radius
	//ESFX_BoxShape:	x=Width;		y=Height;		z=Depth
	//ESFX_ConeShape:	x=Top Radius;	y=Height;		z=Bottom Radius;
	enum ESFX_EmitterShape
	{
		ESFX_SphereShape=0,	//球形
		ESFX_BoxShape,		//长方体
		ESFX_ConeShape,		//圆锥/圆台体
	};

	//粒子的运动形式
	enum ESFX_MotionType
	{
		ESFX_FreeMotion=0,		//自由粒子，其运动状态由初始状态决定，不能编辑其运动过程
		ESFX_TrackMotion,		//轨道运动，其任意时刻的运动状态由用户编辑的轨道决定		
		ESFX_ExplodeMotion,		//中心爆炸粒子，粒子在初始时刻随机的获得一个速度，其后的运动由初始状态决定，类似于自由粒子
		ESFX_AssembleMotion,	//聚合运动，与中心爆炸有着相反的形态：粒子在一定空间内随机诞生后向中心聚合的运动形态
	};

	//粒子对象
	class neffect_particle
	{
	public:
		float		m_life_time;	//粒子的生存时间，粒子诞生时为粒子的最大生存时间，每次更新状态时相应的时长，<=0时表示粒子死亡

		vector3		m_position;		//粒子的当前位置

		float		m_distance;		//当前距离摄像机的距离，该数据用来对粒子进行排序，如果有需要的话

		vector3		m_vel;			//当前速度

		vector3		m_rot;			//粒子的自转

		vector3		m_revo;			//粒子的公转

		vector2		m_size;			//粒子的当前尺寸

		color4f		m_color;		//当前粒子的色彩

		float		m_drift;		//粒子的随机扰动附加值，用来让每个粒子的扰动更具随机性

	public:
		neffect_particle();
		~neffect_particle();
	};

	//粒子系统的配置数据
	struct neffect_particle_config
	{
		neffect_particle_config();
		void serialize(narchive& ar);

		//-----------------------------------------------------------------------------------
		//粒子的属性
		//-----------------------------------------------------------------------------------

		// 面片的类型
		ESFX_FaceType	m_face_type;

		//每个粒子的生命期,单位秒
		float		m_particle_life;

		//粒子的行为方式
		ESFX_MotionType	m_motion;

		//粒子所受的重力加速度,0表示不受重力影响。
		float	m_gravity;

		//粒子所受的空气阻尼系数,该参数用于计算空气主力，近似的空气阻力f=m_damp*speed^2;
		float m_damp;

		//粒子的初始位置
		vector3 m_init_position;

		//粒子的初始旋转,该旋转是粒子的自转
		vector3	m_init_rot;

		//粒子的初始旋转，该旋转是粒子的公转
		vector3 m_init_revo;

		//粒子的诞生时的初始速度，对于爆炸式粒子来说，该速度表示的方向无意义，仅获得其速率，即取速度的模
		vector3 m_init_speed;

		//粒子是否存在于发射器空间,如果是，则粒子会产生跟随发射器运动的行为,当前该参数未实装
		bool	m_is_emitter_space;

		//粒子运动过程中的随机漂移控制
		float m_max_drift;
		float m_min_drift;

		//粒子诞生时的颜色
		color4f m_init_color;

		//粒子速度随时间变化的轨迹，只对轨迹模式粒子有效
		ndistribution<vector3>::ptr	m_vel;

		//粒子自转随时间变化的轨迹
		ndistribution<vector3>::ptr	m_rotate_vel;

		//粒子公转随时间变化的轨迹，只对轨迹模式粒子有效
		ndistribution<vector3>::ptr	m_revolution_vel;

		//粒子色彩随时间变化的轨迹
		ndistribution<color4f>::ptr		m_color;

		//粒子尺寸随时间变化的轨迹
		ndistribution<vector2>::ptr		m_size;

		//-----------------------------------------------------------------------------------
		//粒子发射器的属性
		//-----------------------------------------------------------------------------------

		//发射器形状，默认是球体
		ESFX_EmitterShape	m_emitter_shape;

		//发射器形状参数，默认是(0,0,0)，即退化成点
		vector3		m_emitter_shape_parameter;

		//最大粒子个数
		int			m_max_particle_count;

		//初始化时产生多少个粒子
		int			m_init_particle_count;

		//每秒产生多少个粒子
		int			m_emit_speed;

		//粒子发射器的初始位置
		vector3		m_emitter_init_pos;
		//-----------------------------------------------------------------------------------		
	};

	//粒子发射器实例，负责根据粒子的配置数据更新粒子状态，并且维护粒子的渲染数据
	class neffect_particle_emitter_instance
		: public neffect_element_instance
	{
		friend class neffect_particle_emitter;
	public:
		typedef boost::shared_ptr<neffect_particle_emitter_instance> ptr;

		neffect_particle_emitter_instance(void);
		virtual ~neffect_particle_emitter_instance(void)	{}

		virtual void init(shared_ptr<neffect_element> def);

		virtual bool update_render_data(const nviewport& view, float play_time, const matrix44& owner_mat);

		virtual nrender_mesh* get_render_mesh() const
		{
			return m_mesh.get();
		}

		virtual void	update_particle( const nviewport& view,neffect_particle_emitter* def, float delta_time);

		st_quad_array& get_faces()	{	return m_faces;}

		//获取一个可用的空白粒子，如果没有可用粒子，返回NULL
		neffect_particle*	get_free_particle();

	private:
		//粒子发射器的模板
		shared_ptr<neffect_particle_emitter>	m_template;

		//粒子面片数组,数组中的元素按照距离摄像机的远近排序
		st_quad_array			m_faces;

		//粒子数组，保存了所有粒子的状态
		std::vector<neffect_particle>	m_particles;

		//存活粒子的数量
		int		m_active_face_count;
		
		//发射器的位置
		vector3	m_emitter_pos;

		//发射器的自转
		vector3 m_emitter_rot;

		//发射器公转
		vector3 m_emitter_revo;

		//发射器的变换矩阵
		matrix44	m_Transform;

		//-- 渲染数据
		neffect_vertex_data		m_vert_data;		
		render_res_ptr<nrender_dynamic_mesh>	m_mesh;		
	};

	//粒子发射器对象，负责保存粒子的配置数据、粒子的轨迹控制和粒子发射器的轨迹控制
	class nAPI neffect_particle_emitter:
		public neffect_element
	{
	public:
		virtual void init(const nstring& name);
		virtual neffect_element_instance::ptr create_instance();
		virtual void update_instance(neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time);

		virtual void serialize(narchive& ar);
	public:		
		//粒子系统的配置数据
		neffect_particle_config			m_config;

		//粒子的色彩随时间变化的控制
		ndistribution<color4f>::ptr		m_color;

		//粒子的尺寸随时间变化的控制
		ndistribution<vector2>::ptr		m_size;

		//粒子发射器的速度变化控制
		ndistribution<vector3>::ptr		m_emitter_vel;

		//粒子发射器的自转控制
		ndistribution<vector3>::ptr		m_emitter_rot;

		//粒子发射器的公转控制
		ndistribution<vector3>::ptr		m_emitter_revo;
	public:
		neffect_particle_emitter();
		~neffect_particle_emitter();

		nDECLARE_CLASS(neffect_particle_emitter)
	};
}//namespace nexus

#endif //_NEXUS_EFFECT_CYLINDER_H_