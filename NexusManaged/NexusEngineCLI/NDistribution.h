#pragma once
#include "NMath.h"

namespace NexusEngine
{
	enum DistributionType
	{
		DistributionConst=0,	//常量
		DistributionUniform,	//随机值
		DistributionCurve,		//曲线
	};
	/*
	 * 提供NDisribution对象转换为string；使得PropertyGrid控件能够展开对象
	*/
	public ref class NDistributionConverter : public ExpandableObjectConverter
	{
	public:
		virtual bool CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType) override;		
		virtual Object^ ConvertTo(ITypeDescriptorContext^ context, Globalization::CultureInfo^ culture, Object^ value, Type^ destinationType) override;
	};

	/**
	 *	一个Distribution基类，作为一个标志物
	*/
	//[TypeConverter(NDistributionConverter::typeid)]
	public ref class NDistribution abstract
	{
	};

}//namespace NexusEngine