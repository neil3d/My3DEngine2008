#include "StdAfx.h"
#include "NDistributionVector2.h"

namespace NexusEngine
{
	//-- class NDistributionVector2 --------------------------------------------------------------
	NDistributionVector2::NDistributionVector2(nexus::ndistribution<vector2>::ptr nativeObj)
	{
		m_native = new nexus::ndistribution<vector2>::ptr( nativeObj );
	}

	NDistributionVector2::~NDistributionVector2(void)
	{
		delete m_native;
	}

	Vector2 NDistributionVector2::GetValue(float time)
	{
		return Vector2::FromNative( NativePtr->get_value(time) );
	}

	//-- class NDistributionVector2Const --------------------------------------------------------------
	NDistributionVector2Const::NDistributionVector2Const(void)
	{
		nexus::ndistribution_const<vector2>::ptr nativeObj( new nexus::ndistribution_const<vector2> );
		m_native = new nexus::ndistribution_const<vector2>::ptr(nativeObj);		
	}

	//-- class NDistributionVector2Uniform --------------------------------------------------------------
	NDistributionVector2Uniform::NDistributionVector2Uniform(void)
	{
		nexus::ndistribution_random<vector2>::ptr nativeObj( new nexus::ndistribution_random<vector2> );
		m_native = new nexus::ndistribution_random<vector2>::ptr(nativeObj);		
	}

	//-- class NDistributionVector2Curve --------------------------------------------------------------
	NDistributionVector2Curve::NDistributionVector2Curve(void)
	{
		nexus::ndistribution_curve<vector2>::ptr nativeObj( new nexus::ndistribution_curve<vector2> );
		m_native = new nexus::ndistribution_curve<vector2>::ptr(nativeObj);	
		if(((nexus::ndistribution_curve<vector2>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(Vector2(0,0),0);
			AddKey(Vector2(0,0),1.0f);
		}
	}

	NDistributionVector2Curve::NDistributionVector2Curve( nexus::ndistribution_curve<vector2>::ptr nativeObj ) : NDistributionVector2(nativeObj)
	{
		if(((nexus::ndistribution_curve<vector2>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(Vector2(0,0),0);
			AddKey(Vector2(0,0),1.0f);
		}		
	}
}//namespace NexusEngine