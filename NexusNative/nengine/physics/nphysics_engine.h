#pragma  once
#ifndef _NEXUS_PHYSICS_ENGINE_H_
#define _NEXUS_PHYSICS_ENGINE_H_
#include "ncore.h"
#include "../engine_define.h"
#include "../framework/nlevel.h"
#include "../framework/nsubsystem.h"


namespace nexus
{
	struct phys_config 
	{
		bool			hardware_simulation;
		// use VRD
		bool			use_remote_debugger;
		std::string		remote_debugger_host;
		int				remote_debugger_port;

		// CCD: Enable/disable continuous collision detection
		bool			enable_ccd;

		phys_config() :
			hardware_simulation(true),
			use_remote_debugger(true),
			remote_debugger_host("localhost"),
			remote_debugger_port(NX_DBG_DEFAULT_PORT),
			enable_ccd(false)
		{
		}
	};

	/** Physics Engine
	*/
	class nAPI nphysics_engine
		: public nsubsystem
	{
	public:
		typedef shared_ptr<nphysics_engine> ptr;

		nphysics_engine(void);
		virtual ~nphysics_engine(void)	
		{ 
			destroy();	
		}

		// 初始化，创建PhysXSDK
		virtual bool init(const phys_config& config );
		// 销毁
		virtual void destroy();
		// simulate
		virtual void update( float delta_time );

		bool is_hardoware_simulation( );
		
		// 开启或者关闭CCD:continuous collision detection
		void enable_ccd(bool enable);

		// VRD 
		void connect_vrd( const char* host, unsigned int port );
		void disconnect_vrd();

		NxPhysicsSDK* get_nx_physics_sdk()const 
		{
			return m_physx_driver;
		}

	private:
		phys_config				m_config;
		NxPhysicsSDK*			m_physx_driver;

		nDECLARE_CLASS(nphysics_engine);
	};
}//namespace nexus
#endif //_NEXUS_PHYSICS_ENGINE_H_