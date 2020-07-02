#ifndef _NEXUS_HDR_FINAL_PASS_H
#define _NEXUS_HDR_FINAL_PASS_H

#include "hdr_luminance.h"
#include "post_bloom.h"

namespace nexus
{
	class hdr_final_pass
	{
	public:
		hdr_final_pass(void);
		~hdr_final_pass(void);

		void create_resources();
		void draw_process(const rt_item& hdr_scene_rt);
		const rt_item& get_result() const	{	return m_tex_final;}

	private:
		hdr_luminance	m_lum;
		post_bloom		m_bloom;
		rt_item			m_tex_final;

		d3d_ptr<ID3DXEffect>	m_effect;
	};
}//namespace nexus
#endif //_NEXUS_HDR_FINAL_PASS_H