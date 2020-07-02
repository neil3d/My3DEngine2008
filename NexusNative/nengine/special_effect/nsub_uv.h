#ifndef _NEXUS_SUB_UV_H_
#define _NEXUS_SUB_UV_H_

namespace nexus
{
	/**
	 *	作为一个UV的包装，外部统一使用0~1的UV范围，sub uv根据自己设置的“子图像”的个数来进行uv转换
	*/
	struct nsub_uv
	{
		int	num_images_h;	// 整个texture中横向有几个图片
		int	num_images_v;	// 整个texture中纵向有几个图片
		int	image_index;	// 当前正在使用哪个图片

		nsub_uv(void):num_images_h(1), num_images_v(1), image_index(0)
		{
		}

		nsub_uv(int h_count,int v_count,int index):num_images_h(h_count), num_images_v(v_count), image_index(index)
		{
		}

		vector2 get_sub_uv(float u, float v) const
		{
			return vector2( get_sub_u(u), get_sub_v(v) );
		}

		float get_sub_u(float u) const
		{
			float su = u/num_images_h;

			int y = image_index/num_images_h;
			int x = image_index-y*num_images_h;
			float offset_u = x*(1.0f/num_images_h);
			
			return su+offset_u;
		}

		float get_sub_v(float v) const
		{
			float sv = v/num_images_v;

			int y = image_index/num_images_h;
			float offset_v = y*(1.0f/num_images_v);

			return sv+offset_v;
		}
	};
}//namespace nexus
#endif //_NEXUS_SUB_UV_H_