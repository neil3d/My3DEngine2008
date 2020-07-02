#ifndef _NEXUS_POST_BLOOM_H_
#define _NEXUS_POST_BLOOM_H_
#include "post_process.h"

namespace nexus
{
	class post_bloom
	{
	public:
		post_bloom(void);
		virtual ~post_bloom(void);

		void create_resources();
		void draw_process(const rt_item& source_rt) const; 

		const rt_item& get_result() const	{	return m_bloom_vertical_rt;}
	private:
		rt_item		m_bright_pass_rt;
		rt_item		m_down_sampled_rt;
		rt_item		m_bloom_horizontal_rt;
		rt_item		m_bloom_vertical_rt;

		d3d_ptr<ID3DXEffect>	m_effect;
	};

	extern float ComputeGaussianValue( float x, float mean, float std_deviation );
}//nexus

#endif //_NEXUS_POST_BLOOM_H_