#ifndef _NEXUS_HDR_LUMINANCE_H_
#define _NEXUS_HDR_LUMINANCE_H_
#include "post_process.h"

namespace nexus
{
	class hdr_luminance
	{
	public:
		hdr_luminance(void);
		~hdr_luminance(void);

		void create_resources();
		void measure_luminance(const rt_item& scene_color_rt);

		size_t get_num_rt() const	{	return m_lum_tex_array.size();}
		const rt_item& get_render_item(size_t i) const {	return m_lum_tex_array[i];}
		const rt_item& get_result() const	{	return m_lum_adapted;}
	private:
		D3DFORMAT find_best_fromat();

	private:
		d3d_ptr<ID3DXEffect>	m_effect;
		std::vector<rt_item>	m_lum_tex_array;

		rt_item			m_lum_adapted;
		rt_item			m_last_adapted;
		boost::timer	m_timer;
	};
}//namespace nexus

#endif //_NEXUS_HDR_LUMINANCE_H_