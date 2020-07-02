#include "StdAfx.h"
#include "NDistributionFloat.h"

namespace NexusEngine
{
	NDistributionFloat::NDistributionFloat(nexus::ndistribution<float>::ptr nativeObj)
	{
		m_native = new nexus::ndistribution<float>::ptr( nativeObj );
	}

	NDistributionFloat::~NDistributionFloat(void)
	{
		delete m_native;
	}

	float NDistributionFloat::GetValue(float time)
	{
		return NativePtr->get_value(time);
	}

	//-- class NDistributionFloatConst --------------------------------------------------------------
	NDistributionFloatConst::NDistributionFloatConst(void)
	{
		nexus::ndistribution_const<float>::ptr nativeObj( new nexus::ndistribution_const<float> );
		m_native = new nexus::ndistribution_const<float>::ptr(nativeObj);		
	}

	//-- class NDistributionFloatUniform --------------------------------------------------------------
	NDistributionFloatUniform::NDistributionFloatUniform(void)
	{
		nexus::ndistribution_random<float>::ptr nativeObj( new nexus::ndistribution_random<float> );
		m_native = new nexus::ndistribution_random<float>::ptr(nativeObj);		
	}

	//-- class NDistributionFloatCurve --------------------------------------------------------------
	NDistributionFloatCurve::NDistributionFloatCurve(void)
	{
		nexus::ndistribution_curve<float>::ptr nativeObj( new nexus::ndistribution_curve<float> );
		m_native = new nexus::ndistribution_curve<float>::ptr(nativeObj);
		if(((nexus::ndistribution_curve<float>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(0.0f,0);
			AddKey(0.0f,1.0f);
		}
	}

	NDistributionFloatCurve::NDistributionFloatCurve( nexus::ndistribution_curve<float>::ptr nativeObj ) : NDistributionFloat(nativeObj)
	{
		if(((nexus::ndistribution_curve<float>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(0.0f,0);
			AddKey(0.0f,1.0f);
		}
	}
}