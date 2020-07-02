#include "StdAfx.h"
#include "nphys_pch.h"
#include "nphys_util.h"
#include "nphysics_engine.h"
#include "nphys_scene.h"


namespace nexus
{
	class nphys_output_stream 
		: public NxUserOutputStream
	{
	public:
		virtual void reportError(NxErrorCode errorcode, const char* message, const char* filename, int line)
		{
			std::wstringstream  ret;
			ret << _T( "$DevPhysics$ " );
			switch( errorcode )
			{
			case NXE_INVALID_PARAMETER:
				ret << _T( "invalid parameter.");
				break;
			case NXE_INVALID_OPERATION:
				ret << _T( "invalid operation.");
				break;
			case NXE_OUT_OF_MEMORY:
				ret << _T( "out of memory.");
				break;
			case NXE_DB_INFO:
				ret << _T( "debug info.");
				break;
			case NXE_DB_WARNING:
				ret << _T( "debug warning.");
				break;
			default:
				ret << _T("unknown error.");
			}
			ret << _T( "file :" ) << nexus::conv_string(std::string(filename))
				<< _T("line : ") << line
				<< _T("message : ") << nexus::conv_string(std::string(message)) << endl;
			nLog_Warning( ret.str().c_str() );
		}

		virtual NxAssertResponse reportAssertViolation(const char* message, const char* filename, int line)
		{
			std::wstringstream ret;
			ret << _T( "$DevPhysics$ " );
			ret << _T( "assert in file :" )
				<< nexus::conv_string(std::string(filename))
				<< _T("line : ") << line
				<< _T("message : ") << nexus::conv_string(std::string(message)) << endl;
			nLog_Warning( ret.str().c_str() );

			return NX_AR_BREAKPOINT;
		}

		virtual void print(const char* message)
		{
			std::wstringstream ret;
			ret << _T( "$DevPhysics$ " );
			ret << _T("message : ") << nexus::conv_string(std::string(message))<< endl;
			nLog_Warning( ret.str().c_str() ) ;
		}
	};

	static nphys_output_stream		g_output_stream;

	nphysics_engine::nphysics_engine()
	{
		m_physx_driver = NULL;
	}

	bool nphysics_engine::init( const phys_config& config )
	{
		nASSERT( m_physx_driver == NULL );
		m_config = config;


		// Initialize PhysicsSDK
		NxPhysicsSDKDesc desc;
		NxSDKCreateError errorCode = NXCE_NO_ERROR;
		m_physx_driver = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, &g_output_stream, desc, &errorCode);
		if( m_physx_driver == NULL )
		{
			nLog_Warning( _T("$DevPhysics$ Unable to initialize the PhysX SDK. ERRORCODE = %d ...\r\n"), errorCode );
			return false;
		}
		// The settings for the VRD host and port 
		if ( config.use_remote_debugger
			&& config.remote_debugger_host.size() > 0
			)
		{
			connect_vrd( config.remote_debugger_host.c_str(),config.remote_debugger_port );
		}

		// Set the extra thickness we will use, to make more stable contact generation.
		m_physx_driver->setParameter(NX_SKIN_WIDTH, physx::phys_skin_width );
		m_physx_driver->setParameter(NX_DEFAULT_SLEEP_LIN_VEL_SQUARED, 0.15f*0.15f);
		m_physx_driver->setParameter(NX_DEFAULT_SLEEP_ANG_VEL_SQUARED, 0.14f*0.14f);

		// support creating meshes while scenes are running.
		m_physx_driver->setParameter(NX_ASYNCHRONOUS_MESH_CREATION, 1);

		m_physx_driver->setParameter(NX_VISUALIZATION_SCALE, 1.0f);
		m_physx_driver->setParameter(NX_VISUALIZE_COLLISION_AXES, 1.0f);
		m_physx_driver->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1.0f);
		m_physx_driver->setParameter(NX_VISUALIZE_CLOTH_MESH, 1.f);
		m_physx_driver->setParameter(NX_VISUALIZE_CLOTH_VALIDBOUNDS, 1.f);

		// 
		m_physx_driver->setParameter(NX_ADAPTIVE_FORCE, 0);

		// support creating meshes while scenes are running.
		m_physx_driver->setParameter(NX_ASYNCHRONOUS_MESH_CREATION, 1);

		// CCD
		enable_ccd(config.enable_ccd);

		// Init the cooker. Sets params to defaults.
		NxInitCooking(&m_physx_driver->getFoundationSDK().getAllocator(), &g_output_stream);

		// Set skin thickness for cooking to be the same as the one above.
		const NxCookingParams& Params = NxGetCookingParams();
		NxCookingParams NewParams = Params;
		NewParams.skinWidth = physx::phys_skin_width;
		NxSetCookingParams(NewParams);

		nLog_Info( _T("PhysX init ok.\r\n") );
		return true;
	}

	void nphysics_engine::destroy()
	{
		NxCloseCooking();

		if( m_physx_driver )
		{
			NxReleasePhysicsSDK(m_physx_driver);
			m_physx_driver = NULL;
		}
	}

	void nphysics_engine::update( float /*delta_time*/ )
	{

	}
	
	bool nphysics_engine::is_hardoware_simulation()
	{
		if( !m_config.hardware_simulation )
		{
			return false;
		}
		else
		{
			return m_physx_driver!=NULL && m_physx_driver->getHWVersion() != NX_HW_VERSION_NONE;
		}
	}

	void nphysics_engine::enable_ccd(bool enable)
	{
		nASSERT( m_physx_driver != NULL );
		m_physx_driver->setParameter(NX_CONTINUOUS_CD, enable );
		if( enable )
		{
			m_physx_driver->setParameter(NX_CCD_EPSILON, 0.01f);
			m_physx_driver->setParameter(NX_VISUALIZE_COLLISION_SKELETONS, 1);
			m_physx_driver->setParameter(NX_VISUALIZE_COLLISION_CCD, 1);
		}
	}

	void nphysics_engine::connect_vrd( const char* host, unsigned int port )
	{
		if ( m_physx_driver
			&& m_physx_driver->getFoundationSDK().getRemoteDebugger()
			&& !m_physx_driver->getFoundationSDK().getRemoteDebugger()->isConnected() )
		{
			m_physx_driver->getFoundationSDK().getRemoteDebugger()->connect( host, port, NX_DBG_EVENTMASK_EVERYTHING);
		}
	}

	void nphysics_engine::disconnect_vrd()
	{
		if ( m_physx_driver
			&& m_physx_driver->getFoundationSDK().getRemoteDebugger()
			&& m_physx_driver->getFoundationSDK().getRemoteDebugger()->isConnected() )
		{
			m_physx_driver->getFoundationSDK().getRemoteDebugger()->disconnect();
		}
	}

}//namespace nexus