/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_DRAWING_UNLIT_H_
#define _NEXUS_DRAWING_UNLIT_H_

#include "drawing_policy.h"

namespace nexus
{
	class drawing_unlit :
		public drawing_policy
	{
	public:
		drawing_unlit(void);
		virtual ~drawing_unlit(void);

	public:
		static void create_type(dp_type_list& type_list);
	protected:
		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif //_NEXUS_DRAWING_UNLIT_H_