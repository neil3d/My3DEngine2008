#ifndef _NEXUS_TEXTURE_ATLAS_CREATOR_H_
#define _NEXUS_TEXTURE_ATLAS_CREATOR_H_
#include "nimage.h"
#include "../resource/nresource.h"

namespace nexus
{
	class texture_packer
	{
		struct tex_atlas_item
		{
			nstring name;
			nrect	offset_rc;
		};

	public:
		typedef boost::shared_ptr<texture_packer> ptr;

		texture_packer(void);
		~texture_packer(void);

		void init(int size_w, int size_h);
		bool insert_image(const nstring& item_name, const nimage& img);
		void shrink();
		void save_to_file(int index, const resource_location& out_folder);

	private:
		bool _intersects(const nrect& rc_test, nrect& rc_inter) const;
		void _copy_bits(const nrect& rc_test, const nimage& img);	
		void _merge(const nrect& new_rc);

	private:
		typedef std::vector<nrect> st_rect_array;
		typedef std::vector<tex_atlas_item> st_item_array;

		st_rect_array	m_used_regions;
		int				m_free_pixels;
		
		nimage			m_merge_img;
		st_item_array	m_item_array;
	};

	class nAPI texture_atlas_creator
	{
	public:
		texture_atlas_creator(void);
		~texture_atlas_creator(void);

		/**
		 *	返回Atlas的个数(如果最大尺寸的一张无法放下目录内所有贴图时, 可能创建多张)
		*/
		int make_atlas(const resource_location& source_folder,const resource_location& target_folder, int max_w, int max_h);

	private:
		void insert_image(const nstring& item_name, const nstring& file_name);
	private:
		typedef std::vector<texture_packer::ptr> st_packer_array;
		
		nstring			m_source_path;
		int				m_max_w,
						m_max_h;
		st_packer_array	m_packers;
	};
}//namespace nexus
#endif //_NEXUS_TEXTURE_ATLAS_CREATOR_H_