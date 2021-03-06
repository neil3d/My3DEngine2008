#include "StdAfx.h"
#include "neffect_particle_emitter.h"
#include "../framework/nengine.h"
#include <deque>

namespace nexus
{
	//===============================================================================
	neffect_particle::neffect_particle():m_color(1,1,1,1)
	{
		m_life_time=0;
		m_distance=0;
		//其它数据都使用默认构造函数给出的默认值
	}

	neffect_particle::~neffect_particle()
	{

	}

	//比较粒子相对摄像机的距离
	bool greater_comp_of_particle_distance_to_camera(neffect_particle* a,neffect_particle* b)
	{
		return a->m_distance>b->m_distance;
	}
	//-----------------------------------------------------------------------------------
	neffect_particle_config::neffect_particle_config()
		:m_init_rot(0,0,0),m_init_revo(0,0,0),m_init_speed(0,0,0),m_init_position(0,0,0),
		m_max_drift(0),m_min_drift(0),m_init_color(1,1,1,1),m_emitter_init_pos(0,0,0)
	{
		m_face_type=ESFX_SphereBillboard;

		m_motion=ESFX_FreeMotion;

		m_emitter_shape=ESFX_SphereShape;

		m_max_particle_count=128;

		m_init_particle_count=1;

		m_emit_speed=32;

		m_particle_life=1.0f;

		m_gravity=0;

		m_damp=0;

		m_is_emitter_space=false;
	}

	void neffect_particle_config::serialize( narchive& ar )
	{
		nserialize_enum_as_int(ar, m_face_type, _T("m_face_type"));	
		nserialize_enum_as_int(ar, m_emitter_shape, _T("m_emitter_shape"));	
		nserialize_enum_as_int(ar, m_motion, _T("m_motion"));	

		nSERIALIZE(ar, m_emitter_shape_parameter);
		nSERIALIZE(ar, m_max_particle_count);		
		nSERIALIZE(ar, m_init_particle_count);
		nSERIALIZE(ar, m_emit_speed);
		nSERIALIZE(ar, m_particle_life);
		nSERIALIZE(ar, m_gravity);
		nSERIALIZE(ar, m_damp);
		nSERIALIZE(ar, m_init_position);
		nSERIALIZE(ar, m_init_rot);
		nSERIALIZE(ar, m_init_revo);
		nSERIALIZE(ar, m_init_speed);
		nSERIALIZE(ar, m_max_drift);
		nSERIALIZE(ar, m_min_drift);
		nSERIALIZE(ar, m_init_color);
		nSERIALIZE(ar,m_emitter_init_pos);
		nSERIALIZE(ar,m_is_emitter_space);
	}

	//-----------------------------------------------------------------------------------
	void neffect_particle_emitter_instance::init( shared_ptr<neffect_element> def )
	{
		neffect_element_instance::init(def);

		//复制系统参数，并且根据参数中提供的信息，为粒子们分配存储空间

		//--
		m_template = dynamic_pointer_cast<neffect_particle_emitter>( def );
		neffect_particle_config& m_config=m_template->m_config;
		
		m_emitter_pos=m_config.m_emitter_init_pos;

		//-- init quad array
		m_faces.resize( m_config.m_max_particle_count );		
		m_particles.resize(m_config.m_max_particle_count);
		const nsub_uv& suv = m_template->m_sub_uv;

		for (int i=0; i<m_config.m_max_particle_count; i++)
		{
			quad_face& face = m_faces[i];
			m_particles[i].m_life_time=0;
			face.uv_start.x = suv.get_sub_u(0);
			face.uv_start.y = suv.get_sub_v(0);
			face.uv_end.x = suv.get_sub_u(1);
			face.uv_end.y = suv.get_sub_v(1);
			face.color=m_config.m_init_color;
		}

		//-- create memory vertex data
		int num_triangle = m_config.m_max_particle_count*2;
		int num_vert = num_triangle*3;

		m_vert_data.init(num_vert);

		//-- create render mesh
		nrender_resource_manager* rres_mgr = nengine::instance()->get_render_res_mgr();
		m_mesh.reset( rres_mgr->alloc_dynamic_mesh() );
		m_mesh->create(EDraw_TriangleList, &m_vert_data, 0);
	}

	bool neffect_particle_emitter_instance::update_render_data( const nviewport& view, float play_time, const matrix44& owner_mat )
	{
		(void)view;		
		(void)play_time;
		
		//生成粒子的图元数据
		quad_face::build_mesh_vertices(m_vert_data, m_faces, view.camera, owner_mat, m_template->m_config.m_face_type,m_active_face_count);
		m_mesh->update_vertices(&m_vert_data);
		m_mesh->set_draw_limit(m_active_face_count*2);

		return true;
	}

	neffect_particle_emitter_instance::neffect_particle_emitter_instance( void )
	{
		m_active_face_count=1;
	}

	void neffect_particle_emitter_instance::update_particle( const nviewport& view, neffect_particle_emitter* def, float delta_time )
	{
		(void)view;		
		(void) def;
		
		neffect_particle_config& m_config=def->m_config;

		//更新例子发射器状态
		matrix44 revo,trans,rota;
		mat_set_translation(trans,m_emitter_pos);
		mat_set_rotation_yaw_pitch_roll(revo,m_emitter_revo.y,m_emitter_revo.x,m_emitter_revo.z);
		mat_set_rotation_yaw_pitch_roll(rota,m_emitter_rot.y,m_emitter_rot.x,m_emitter_rot.z);
		m_Transform=rota*trans*revo;

		//发射器的当前位置
		vector3 emitter_pos=m_config.m_init_position*m_Transform;

		//更新粒子状态
		m_active_face_count=0;
		std::deque<neffect_particle*>	sort_list;
		for(unsigned int i=0;i<m_particles.size();++i)
		{
			neffect_particle& par=m_particles[i];
			if(par.m_life_time<=0) continue;	//只处理存活的粒子

			par.m_life_time-=delta_time;
			if(par.m_life_time<=0)
			{
				//粒子死亡
				continue;
			}

			//计算重力影响
			float life=m_config.m_particle_life-par.m_life_time;
			float sg=m_config.m_gravity*life;

			switch (m_config.m_motion)
			{
			case ESFX_FreeMotion:
				{
					par.m_vel.y+=sg;
					par.m_position+=par.m_vel*delta_time;
					//简单的模拟一下空气阻力
					par.m_vel*=(1.0f-m_config.m_damp);
					break;
				}
			case ESFX_TrackMotion:
				{
					vector3 vel = m_config.m_vel->get_value(life);
					vel.y+=sg;
					//简单的模拟一下空气阻力
					vel*=(1.0f-m_config.m_damp);
					vector3 revo_vel = m_config.m_revolution_vel->get_value(life);
					par.m_position += vel*delta_time;
					par.m_revo += revo_vel*delta_time;
					par.m_vel=vel;
					break;
				}
			case ESFX_ExplodeMotion:
				{
					par.m_vel.y+=sg;
					par.m_position+=par.m_vel*delta_time;
					//简单的模拟一下空气阻力
					par.m_vel*=(1.0f-m_config.m_damp);
					break;
				}
			case ESFX_AssembleMotion:
				{
					par.m_vel.y+=sg;
					vector3 old_pos=par.m_position-emitter_pos;
					par.m_position+=par.m_vel*delta_time;
					vector3 new_pos=par.m_position-emitter_pos;
					//如果到达发射器的位置，则粒子死亡
					if(vec_dot(old_pos,new_pos)<0)	//点乘<0表示方向改变，必然经过一次中点
					{
						par.m_life_time=0;
						continue;
					}
					//简单的模拟一下空气阻力
					par.m_vel*=(1.0f-m_config.m_damp);					
					break;
				}
			}
			
			vector3 rot_vel = m_config.m_rotate_vel->get_value(life);
			par.m_rot+=rot_vel*delta_time;
			par.m_size=m_config.m_size->get_value(life);
			par.m_color=m_config.m_color->get_value(life);

			//计算随机漂移
			float drift=_get_rand(m_config.m_max_drift,m_config.m_min_drift);
			if(drift>0)
			{
				par.m_position.x+=sinf(par.m_life_time+par.m_drift)*drift*delta_time;
				par.m_position.z+=cosf(par.m_life_time+par.m_drift)*drift*delta_time;
			}

			//计算粒子相对摄像机的距离
			par.m_distance=vec_distance_sq(par.m_position,view.camera.get_eye_pos());

			++m_active_face_count;
			sort_list.push_back(&par);
		}

		//添加新粒子
		if(m_active_face_count<m_config.m_max_particle_count)
		{
			//计算理论上应该添加的最大粒子数量
			int max_count;
			if(m_active_face_count==0)
			{
				max_count=m_config.m_init_particle_count;
			}
			else
			{
				max_count=m_config.m_emit_speed*delta_time;
			}			
			//计算还能发射多少个粒子
			int resv_count=m_config.m_max_particle_count-m_active_face_count;
			//需要发射的粒子的数量
			int count=__min(max_count,resv_count);
			for(int i=0;i<count;++i)
			{
				//寻找一个未被使用的粒子
				neffect_particle* par=get_free_particle();
				if(par==0) break;

				par->m_life_time=m_config.m_particle_life;
				par->m_color=m_config.m_init_color;
				par->m_revo=m_config.m_init_revo;
				par->m_drift=_get_rand(m_config.m_min_drift,m_config.m_max_drift*2.0f);
				//初始位置与粒子发射器关联
				par->m_position=emitter_pos;

				//尺寸和色彩获取0时刻的相应数据
				par->m_size=m_config.m_size->get_value(0);
				par->m_color=m_config.m_color->get_value(0);
				
				//产生随机位置
				vector3 v=m_config.m_emitter_shape_parameter*0.5f;
				vector3 shape_pos;
				switch(m_config.m_emitter_shape)
				{
				case ESFX_SphereShape://球
				case ESFX_BoxShape://box
					{
						shape_pos.x=_get_rand(-v.x,v.x);
						shape_pos.y=_get_rand(-v.y,v.y);
						shape_pos.z=_get_rand(-v.z,v.z);
					}
					break;
				case ESFX_ConeShape://cone
					{
						float h=_get_rand(0.0f,m_config.m_emitter_shape_parameter.y);
						float maxR=_get_lerp(m_config.m_emitter_shape_parameter.x,m_config.m_emitter_shape_parameter.z,
							h/m_config.m_emitter_shape_parameter.y);
						float myR=_get_rand(0.0f,maxR);
						float angle=_get_rand(0.0f,nPI*2);

						shape_pos.x=myR*sinf(angle);
						shape_pos.y=h;
						shape_pos.z=myR*cosf(angle);
					}
					break;
				default:
					shape_pos.x=_get_rand(-v.x,v.x);
					shape_pos.y=_get_rand(-v.y,v.y);
					shape_pos.z=_get_rand(-v.z,v.z);
					break;
				}//endof switch
				
				par->m_position+=shape_pos;

				switch (m_config.m_motion)
				{
				case ESFX_FreeMotion:
					{
						//自由式，只需要获取初速度
						par->m_vel=m_config.m_init_speed;
						break;
					}
				case ESFX_TrackMotion:
					{
						//轨迹式，忽略设定的粒子初始状态，直接从轨迹控制器中获取0时刻的数据
						vector3 vel = m_config.m_vel->get_value(0);
						vector3 revo_vel = m_config.m_revolution_vel->get_value(0);
						par->m_revo += revo_vel;
						par->m_vel=vel;
						break;
					}
				case ESFX_ExplodeMotion:
					{
						//爆炸式
						par->m_vel=_get_rand(vector3::one,vector3::one*-1.0f);
						//粒子的初速度在最大初速度的30%~100%之间，可以产生层次感
						par->m_vel*=vec_length(m_config.m_init_speed)*_get_rand(0.3f,1.0f);
						break;
					}
				case ESFX_AssembleMotion:
					{
						//使用一个随机的方向
						par->m_vel=vec_normalize(shape_pos)*-1.0f;
						par->m_vel*=vec_length(m_config.m_init_speed)*_get_rand(0.3f,1.0f);
						break;
					}
				}

				++m_active_face_count;
				sort_list.push_back(par);
			}
		}

		//排序粒子。思考：是不是可以加一个开关，在不需要apha blend的场合下可以不用排序？
		sort(sort_list.begin(),sort_list.end(),greater_comp_of_particle_distance_to_camera);

		//复制face数据
		for(int i=0;i<m_active_face_count;++i)
		{
			quad_face& face = m_faces[i];
			neffect_particle* par=sort_list[i];
			face.color=par->m_color;
			face.pos=par->m_position;
			face.revolution = par->m_revo;
			face.rot=par->m_rot;
			face.half_size = par->m_size*0.5f;
		}
	}

	neffect_particle* neffect_particle_emitter_instance::get_free_particle()
	{
		for(unsigned int i=0;i<m_particles.size();++i)
		{
			if(m_particles[i].m_life_time<=0)
			{
				return &m_particles[i];
			}
		}
		return 0;
	}
	//===============================================================================

	nDEFINE_CLASS(neffect_particle_emitter, neffect_element)

	neffect_particle_emitter::neffect_particle_emitter()
	{
		m_color.reset( new ndistribution_const<color4f>(color4f(1,1,1,1)) );
		m_size.reset( new ndistribution_const<vector2>(vector2(40,40)) );

		m_emitter_vel.reset( new ndistribution_const<vector3>(vector3(0,0,0)) );
		m_emitter_rot.reset( new ndistribution_const<vector3>(vector3(0,0,0)) );
		m_emitter_revo.reset( new ndistribution_const<vector3>(vector3(0,0,0)) );

		//轨迹粒子的轨迹控制器保存在基类neffect_element里面

		m_config.m_revolution_vel=this->m_revolution_vel;
		m_config.m_rotate_vel=this->m_rotate_vel;
		m_config.m_vel=this->m_vel;
		m_config.m_color=this->m_color;
		m_config.m_size=this->m_size;
	}

	neffect_particle_emitter::~neffect_particle_emitter()
	{

	}

	void neffect_particle_emitter::init( const nstring& name )
	{
		neffect_element::init(name);
	}

	neffect_element_instance::ptr neffect_particle_emitter::create_instance()
	{
		neffect_particle_emitter_instance::ptr new_inst( new neffect_particle_emitter_instance );

		init_instance_base(new_inst);
		return new_inst;
	}

	void neffect_particle_emitter::update_instance( neffect_element_instance::ptr inst, const nviewport& view, float delta_time, float play_time )
	{
		(void)play_time;

		//-- 是否启动
		if( !inst->m_active )
		{
			if(play_time > m_start_time)
				inst->m_active = true;
		}
		if( !inst->m_active )
			return;

		neffect_particle_emitter_instance::ptr par_inst = boost::dynamic_pointer_cast<neffect_particle_emitter_instance>(inst);

		//因为每个粒子的轨迹不一样，所以粒子的轨迹控制在neffect_particle_emitter_instance中计算
		//此处计算粒子发射机的轨迹，因此不需要调用基类的相关函数，而是自己计算
		
		float my_time = play_time - m_start_time;

		vector3 vel = m_emitter_vel->get_value(my_time);
		vector3 rot_vel = m_emitter_rot->get_value(my_time);
		vector3 revo_vel = m_emitter_revo->get_value(my_time);

		par_inst->m_emitter_pos+=vel*delta_time;
		par_inst->m_emitter_revo+=revo_vel*delta_time;
		par_inst->m_emitter_rot+=rot_vel*delta_time;

		par_inst->update_particle(view,this,delta_time);
		
	}

	void neffect_particle_emitter::serialize( narchive& ar )
	{
		neffect_element::serialize(ar);

		m_config.serialize(ar);

		nSERIALIZE_DISTRIBUTION(ar, m_color);
		nSERIALIZE_DISTRIBUTION(ar, m_size);
		nSERIALIZE_DISTRIBUTION(ar, m_emitter_vel);
		nSERIALIZE_DISTRIBUTION(ar, m_emitter_rot);
		nSERIALIZE_DISTRIBUTION(ar, m_emitter_revo);

		//反序列化之后需要更新粒子配置数据中的信息
		if(ar.is_loading())
		{
			m_config.m_revolution_vel=this->m_revolution_vel;
			m_config.m_rotate_vel=this->m_rotate_vel;
			m_config.m_vel=this->m_vel;
			m_config.m_color=this->m_color;
			m_config.m_size=this->m_size;
		}
	}
}
