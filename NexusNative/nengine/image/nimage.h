#ifndef _NEXUS_IMAGE_H_
#define _NEXUS_IMAGE_H_
#include "image_def.h"

namespace nexus
{
	class nAPI nimage
	{
	public:
		nimage(void);
		virtual ~nimage(void);

		static void init_image_sdk();
		static void close_image_sdk();

		void create(size_t w, size_t h, EPixelFormat fmt);

		void blit(const nimage& source, int dest_x, int dest_y, int src_x, int src_y, size_t src_w, size_t src_h);

		void load_from_file(const nstring& file_name);
		void save_to_file(const nstring& file_name);
		void destroy();

		void convert_format(EPixelFormat fmt);

		EPixelFormat get_format() const;
		size_t get_width() const;
		size_t get_height() const;
		
		void* get_pixel() const;
		size_t _get_image_id() const;
	private:
		struct	impl;
		impl*	m_impl;
	};
}//namespace nexus
#endif //_NEXUS_IMAGE_H_