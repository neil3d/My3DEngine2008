#include "StdAfx.h"
#include "NDistributionColor.h"

namespace NexusEngine
{
	//-- class NDistributionColor --------------------------------------------------------------
	NDistributionColor::NDistributionColor(nexus::ndistribution<color4f>::ptr nativeObj)
	{
		m_native = new nexus::ndistribution<color4f>::ptr( nativeObj );
	}

	NDistributionColor::~NDistributionColor(void)
	{
		delete m_native;
	}

	Color4f NDistributionColor::GetValueByTime(float time)
	{
		return Color4f::FromNative( NativePtr->get_value(time) );
	}

	//-- class NDistributionColorConst --------------------------------------------------------------
	NDistributionColorConst::NDistributionColorConst(void)
	{
		nexus::ndistribution_const<color4f>::ptr nativeObj( new nexus::ndistribution_const<color4f> );
		m_native = new nexus::ndistribution_const<color4f>::ptr(nativeObj);		
	}

	//-- class NDistributionColorUniform --------------------------------------------------------------
	NDistributionColorUniform::NDistributionColorUniform(void)
	{
		nexus::ndistribution_uniform<color4f>::ptr nativeObj( new nexus::ndistribution_uniform<color4f> );
		m_native = new nexus::ndistribution_uniform<color4f>::ptr(nativeObj);		
	}

	//-- class NDistributionColorCurve --------------------------------------------------------------
	NDistributionColorCurve::NDistributionColorCurve(void)
	{
		nexus::ndistribution_curve<color4f>::ptr nativeObj( new nexus::ndistribution_curve<color4f> );
		m_native = new nexus::ndistribution_curve<color4f>::ptr(nativeObj);	

		if(((nexus::ndistribution_curve<color4f>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(Color4f(1,1,1,1),0);
			AddKey(Color4f(1,1,1,1),1.0f);
		}
	}

	NDistributionColorCurve::NDistributionColorCurve( nexus::ndistribution_curve<color4f>::ptr nativeObj ) : NDistributionColor(nativeObj)
	{
		if(((nexus::ndistribution_curve<color4f>*)nativeObj.get())->get_num_keys()==0)
		{
			AddKey(Color4f(1,1,1,1),0);
			AddKey(Color4f(1,1,1,1),1.0f);
		}
	}
}//namespace NexusEngine