#include "StdAfx.h"
#include "NDistributionVector3.h"

namespace NexusEngine
{
	//-- class NDistributionVector3 --------------------------------------------------------------
	NDistributionVector3::NDistributionVector3(nexus::ndistribution<vector3>::ptr nativeObj)
	{
		m_native = new nexus::ndistribution<vector3>::ptr( nativeObj );
	}

	NDistributionVector3::~NDistributionVector3(void)
	{
		delete m_native;
	}

	Vector3 NDistributionVector3::GetValue(float time)
	{
		return Vector3::FromNative( NativePtr->get_value(time) );
	}

	//-- class NDistributionVector3Const --------------------------------------------------------------
	NDistributionVector3Const::NDistributionVector3Const(void)
	{
		nexus::ndistribution_const<vector3>::ptr nativeObj( new nexus::ndistribution_const<vector3> );
		m_native = new nexus::ndistribution_const<vector3>::ptr(nativeObj);		
	}

	//-- class NDistributionVector3Uniform --------------------------------------------------------------
	NDistributionVector3Uniform::NDistributionVector3Uniform(void)
	{
		nexus::ndistribution_random<vector3>::ptr nativeObj( new nexus::ndistribution_random<vector3> );
		m_native = new nexus::ndistribution_random<vector3>::ptr(nativeObj);		
	}

	//-- class NDistributionVector3Curve --------------------------------------------------------------
	NDistributionVector3Curve::NDistributionVector3Curve(void)
	{
		nexus::ndistribution_curve<vector3>::ptr nativeObj( new nexus::ndistribution_curve<vector3> );
		m_native = new nexus::ndistribution_curve<vector3>::ptr(nativeObj);	
		if(((nexus::ndistribution_curve<vector3>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(Vector3(0,0,0),0);
			AddKey(Vector3(0,0,0),1.0f);
		}
	}

	NDistributionVector3Curve::NDistributionVector3Curve( nexus::ndistribution_curve<vector3>::ptr nativeObj ) : NDistributionVector3(nativeObj)
	{
		if(((nexus::ndistribution_curve<vector3>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(Vector3(0,0,0),0);
			AddKey(Vector3(0,0,0),1.0f);
		}
	}
}//namespace NexusEngine