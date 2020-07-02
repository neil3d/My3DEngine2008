#include "StdAfx.h"
#include <boost/algorithm/string.hpp>
#include <sstream>
#include "ncore.h"
#include "../framework/nengine.h"
#include "texture_atlas_creator.h"
#include "../resource/nresource_texture_atlas.h"

namespace nexus
{
	//-- class texture_packer --------------------------------------------------------------------------
	texture_packer::texture_packer(void)
	{}

	texture_packer::~texture_packer(void)
	{}

	void texture_packer::init(int size_w, int size_h)
	{
		m_merge_img.create(size_w, size_h, EPF_A8R8G8B8);
		m_used_regions.clear();
		m_free_pixels = size_w*size_h;
	}

	//-----------------------------------------------------------------------------
	// Name: insert_image()
	// Desc: Insert passed-in texture into atlas.  If no free spot is found, 
	//       return false.  If a free spot is found, copy bits from texture 
	//       to atlas, update texture to point to proper sub-region in atlas
	//       add the newly filled region to the used region vector, then
	//       return true.
	//-----------------------------------------------------------------------------
	bool texture_packer::insert_image(const nstring& item_name, const nimage& img)
	{
		size_t img_w = img.get_width();
		size_t img_h = img.get_height();

		// check for trivial non-fit
		if (img_w*img_h > m_free_pixels)
			return false;

		// find a free spot for this texture
		nrect rc_test;

		long u, v;
		// *** Optimization ***
		// right now this insertion algorithm creates wide horizontal
		// atlases.  It seems it would be more optimal to create more
		// 'square' atlases; or even better insert in such a way as
		// to minimize the total number of used regions, i.e., 
		// always try to attach to one or more edges of existing regions.
		//
		// loop in v: slide test-region vertically across atlas
		for (v = 0; (v+1)*img_h <= m_merge_img.get_height(); ++v)
		{
			rc_test.top    = v     * img_h;
			rc_test.bottom = (v+1) * img_h;

			// loop in u: slide test-region horizontally across atlas
			for (u = 0; (u+1)*img_w <= m_merge_img.get_width(); ++u)
			{
				rc_test.left  = u     * img_w;
				rc_test.right = (u+1) * img_w;

				// go through all Used regions and see if they overlap
				nrect intersection;
				if (_intersects(rc_test, intersection))
				{
					// found an intersecting used region: try next position
					// but actually advance position by the larger of pTexture's width
					// and this intersection
					float ratio =   static_cast<float>(intersection.right)
						/ static_cast<float>(rc_test.right);
					ratio = ceilf(max(0.0f, ratio-1.0f));  
					u += static_cast<long>(ratio);
				}
				else
				{
					// no intersection found: 
					// merge this region into the used region's vector
					_merge(rc_test);

					m_free_pixels -= img_w*img_h;
					nASSERT(m_free_pixels >= 0);
					
					_copy_bits(rc_test, img);

					// also update the Texture2D object to set correct 
					// atlas pointers and offsets
					tex_atlas_item new_item;
					new_item.name = item_name;
					new_item.offset_rc = rc_test;
					m_item_array.push_back(new_item);
					return true;
				}

			}// end of for(u)
		}// end of for(v)

		return false;
	}

	void texture_packer::shrink()
	{}

	void texture_packer::save_to_file(int index, const resource_location& out_folder)
	{
		resource_location loc = out_folder;
		if(index != -1)
		{
			std::wostringstream oss;
			oss << loc.file_name << index;
			loc.file_name = oss.str();
		}
		
		//-- 写入图像文件		
		nstring img_file_name = nengine::instance()->get_file_sys()->get_full_path(loc.pkg_name, loc.file_name);
		img_file_name += _T(".tga");
		m_merge_img.save_to_file(img_file_name);

		//-- 写入资源对象数据
		float img_w = (float)m_merge_img.get_width();
		float img_h = (float)m_merge_img.get_height();

		nresource_texture_atlas txa(loc.to_string());
		for (st_item_array::iterator iter = m_item_array.begin();
			iter != m_item_array.end();
			++iter)
		{
			const tex_atlas_item& item = *iter;
			txa.add_item(item.name,
				vector2(item.offset_rc.left/img_w, item.offset_rc.top/img_h),
				vector2(item.offset_rc.right/img_w, item.offset_rc.bottom/img_h)
				);			
		}
		
		resource_location tex_loc = loc;
		tex_loc.file_name += _T(".tga");
		txa.set_internal_texture(tex_loc);

		resource_location out_loc = loc;
		out_loc.file_name += _T(".txa");
		txa.save_to_file(out_loc, true);
	}

	bool texture_packer::_intersects(const nrect& rc_test, nrect& rc_inter) const
	{
		for (st_rect_array::const_iterator iter = m_used_regions.begin();
			iter != m_used_regions.end();
			++iter)
		{
			if( iter->intersect(rc_test) )
			{
				rc_inter = *iter;
				return true;
			}
		}

		return false;
	}

	void texture_packer::_copy_bits(const nrect& rc, const nimage& img)
	{
		m_merge_img.blit(img, rc.left, rc.top, 0, 0, rc.get_width(), rc.get_height());
	}

	void texture_packer::_merge(const nrect& new_rc)
	{
		m_used_regions.push_back(new_rc);
	}

	//-- class texture_atlas_creator -------------------------------------------------------------------
	texture_atlas_creator::texture_atlas_creator(void)
	{
	}

	texture_atlas_creator::~texture_atlas_creator(void)
	{
	}

	int texture_atlas_creator::make_atlas(const resource_location& source_folder,const resource_location& target_folder, int max_w, int max_h)
	{
		nstring source_path = 
			nengine::instance()->get_file_sys()->get_full_path(source_folder.pkg_name, source_folder.file_name);
		if(source_path.empty())
			return 0;

		m_max_w = max_w;
		m_max_h = max_h;		

		if ( boost::ends_with(source_path, _T("\\"))
			|| boost::ends_with(source_path, _T("/")) )
		{
			m_source_path = source_path.substr(0, source_path.length()-1);
		}
		else
		{
			m_source_path = source_path;			
		}

		//-- 搜索目录中的所有TGA文件(不包含子目录), 并执行贴图合并
		boost::filesystem::wpath base_path(source_path);
		boost::filesystem::wdirectory_iterator end_iter;
		for(boost::filesystem::wdirectory_iterator iter(base_path);
			iter != end_iter;
			++iter)
		{
			boost::filesystem::wpath cur_path = *iter;
			if(boost::filesystem::is_regular( iter->status() ) )
			{
				nstring file_name = cur_path.file_string();
				size_t file_name_len = file_name.length();
				if(file_name_len > 4)
				{
					nstring file_ext = file_name.substr(file_name_len-4);
					std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), tolower);					

					if(file_ext == _T(".tga")
						|| file_ext == _T(".dds")
						|| file_ext == _T(".bmp")
						|| file_ext == _T(".png")
						)
					{
						nstring item_name = filename_no_extension(cur_path.filename());
						insert_image(item_name, file_name);
					}

				}// end of if
			}// end of if
		}// end of for

		//-- 优化并存盘
		int num = m_packers.size();
		int i = 0;
		for(st_packer_array::iterator iter = m_packers.begin();
			iter != m_packers.end();
			++iter)
		{
			texture_packer::ptr packer = *iter;
			packer->shrink();

			//-- 生成图片文件名
			nstring img_file_name;
			std::wostringstream ostr;
			if( num>0 )				
				i++;			
			else
				i = -1;

			packer->save_to_file(i, target_folder);
		}


		return (int)m_packers.size();
	}


	void texture_atlas_creator::insert_image(const nstring& item_name, const nstring& file_name)
	{
		nimage img;
		img.load_from_file(file_name);

		for(st_packer_array::iterator iter = m_packers.begin();
			iter != m_packers.end();
			++iter)
		{
			texture_packer::ptr packer = *iter;
			if( packer->insert_image(item_name, img) )
				return;
		}// end of for

		//-- create new packer
		texture_packer::ptr new_packer( new texture_packer );
		new_packer->init(m_max_w, m_max_h);
		if( new_packer->insert_image(item_name, img) )
		{
			m_packers.push_back( new_packer );
		}
		else
		{
			nLog_Error(_T("texture_atlas_creator : texture pack failed."), file_name.c_str());
		}
	}
}//namespace nexus