#include "StdAfx.h"
#include "NDistribution.h"

using namespace System::Globalization;

namespace NexusEngine
{
	//-- class NDistributionConverter ------------------------------------------------------
	bool NDistributionConverter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
	{
		if( destinationType == String::typeid )
			return true;
		else
			return ExpandableObjectConverter::CanConvertTo(context, destinationType);
	}

	Object^ NDistributionConverter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
	{
		if( destinationType == nullptr )
			throw gcnew ArgumentNullException( "destinationType" );

		NDistribution^ dist = dynamic_cast<NDistribution^>( value );

		if( destinationType == String::typeid && dist != nullptr )
		{
			return dist->GetType()->Name;
		}

		return ExpandableObjectConverter::ConvertTo(context, culture, value, destinationType);
	}
}//namespace NexusEngine