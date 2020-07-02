#include "nunit.h"
#include "nlog_mt.h"

namespace nexus {

	nunit::nunit() : nworld_object()
	{
	}

	nunit::~nunit()
	{
	}

	bool nunit::init()
	{
		return true;
	}

	void nunit::destroy()
	{
		// TODO: D. Zhao add unit destroy


		nworld_object::destroy();
	}

	void nunit::update(uint32 elapse)
	{
		nworld_object::update(elapse);
		// TODO: D. Zhao add unit update


	}

} // namespace nexus
