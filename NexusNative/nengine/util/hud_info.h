#pragma once

namespace nexus
{
    class nAPI hud_info
    {
    public:
		unsigned int primitive_cull_count;
		unsigned int primitive_occlude_count;
		unsigned int draw_call_count;
		unsigned int visible_primitive_count;

		void reset()
		{
			primitive_cull_count = 0;
			primitive_occlude_count = 0;
			draw_call_count = 0;
			visible_primitive_count = 0;
		}
    private:
    };

	extern nAPI hud_info g_hud_info;
}