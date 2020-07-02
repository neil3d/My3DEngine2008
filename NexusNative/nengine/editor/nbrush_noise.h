#ifndef _NEXUS_ENGINE_BRUSH_NOISE_H_
#define _NEXUS_ENGINE_BRUSH_NOISE_H_
#include "../util/perlin.h"

namespace nexus
{
	// 将全局变量 改为单件
	class nbrush_noise
	{
	public:
		nbrush_noise(){}
		~nbrush_noise(){}

		static nbrush_noise* instance()
		{
			static nbrush_noise g_brush_noise;
			return &g_brush_noise;
		}

		boost::shared_ptr<Perlin> get_perlin_brush_noise() const
		{
			return m_perlin_brush_noise;
		}


		void reset_brush_noise(float freq, int brush_strength)
		{
			m_perlin_brush_noise.reset( new Perlin(8, freq, brush_strength*0.5f, rand())
				);
		}

	private:
		// 一个全局的noise对象，只在某些时刻重新生成；用于brush operator对象
		boost::shared_ptr<Perlin> m_perlin_brush_noise;
	};
} // end of namespace nexus

#endif // end of _NEXUS_ENGINE_BRUSH_NOISE_H_