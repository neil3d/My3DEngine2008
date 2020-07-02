#pragma once
#include "NDistributionFloat.h"
#include "NDistributionVector2.h"
#include "NDistributionVector3.h"
#include "NDistributionColor.h"

#define CREATE_DISTRIBUTION(member,var,type1,type2,type3)\
	{\
	String^ _name=gcnew String(var->get_type().c_str());\
	if(_name==L"ndistribution_const")\
		{\
		member=gcnew type1(var);\
		}\
		else if(_name==L"ndistribution_uniform")\
		{\
		member=gcnew type2(var);\
		}\
		else if (_name==L"ndistribution_curve")\
		{\
		member=gcnew type3(var);\
		}\
	}