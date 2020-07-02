/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_PRE_PASS_H_
#define _NEXUS_DRAWING_PRE_PASS_H_
#include "drawing_policy.h"

namespace nexus
{
	class drawing_pre_pass :
		public drawing_policy
	{
	public:
		drawing_pre_pass(void);
		virtual ~drawing_pre_pass(void);

		static void create_type(dp_type_list& type_list);
	protected:
		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif //_NEXUS_DRAWING_PRE_PASS_H_