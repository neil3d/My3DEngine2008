

#include "ntime_regular.h"


namespace nexus{

const uint32 STORE_INTER_TIME	= 5 * 60 * 1000;	//存储间隔时间(暂时为5分钟间隔)
const uint32 MAX_SLICE_TIME		= 100;				//保存数据时最大时间切片

ntime_regular::ntime_regular(void)
{
	m_inter_time = STORE_INTER_TIME;
}


ntime_regular::~ntime_regular(void)
{
}

void ntime_regular::update(uint32 elapse)
{
	m_elapse_time += elapse;
	if( m_elapse_time > m_inter_time )
	{
		store();
		m_elapse_time = 0;
	}
}

void ntime_regular::shut_dowm(void)
{
	store();
}

void ntime_regular::start_slice(void)
{
	m_slice_time = ::timeGetTime();
}

// 时间切片是否结束
bool ntime_regular::end_slice(void)
{
	return ( (::timeGetTime() - m_slice_time) > MAX_SLICE_TIME );
}

void nrole_obj_base::_init(void)
{
	m_cache_policy_type = EROCPolicyType_Update;
	m_unload_store		= false;
}

} //namespace nexus