#ifndef _NEXUS_DRAWING_EXTEND_H_
#define _NEXUS_DRAWING_EXTEND_H_
#include "drawing_policy.h"

namespace nexus
{
	class drawing_extend :
		public drawing_policy
	{
	public:
		drawing_extend(void);
		virtual ~drawing_extend(void);

		static void create_type(dp_type_list& type_list);

		//virtual void draw_atom(const nview_info* view, const drawing_atom& atom);
	private:
		static nstring	s_dp_name_str;
	};
}//namespace nexus
#endif//_NEXUS_DRAWING_EXTEND_H_