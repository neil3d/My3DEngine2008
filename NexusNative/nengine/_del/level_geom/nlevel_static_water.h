#pragma once
#include "../framework/nlevel_geom.h"

class nlevel_static_water :
	public nexus::nlevel_geom
{
public:
	nlevel_static_water(const nstring& name_str);
	virtual ~nlevel_static_water(void);
};
