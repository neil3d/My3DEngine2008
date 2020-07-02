#ifndef _NEXUS_WATER_MTL_H_
#define _NEXUS_WATER_MTL_H_
#include "../material/nmaterial.h"
#include "../framework/nscene_capture.h"
#include "../scene_capture/nscene_capture_reflect.h"
#include "../scene_capture/nscene_capture_refract.h"

namespace nexus
{
	class nterrain_water;
	/**
	 *	水面材质的基类
	*/
	class nwater_mtl
		: public nmaterial_base
	{
	public:
		typedef shared_ptr<nwater_mtl> ptr;

		explicit nwater_mtl(const nstring& name_str);
		virtual ~nwater_mtl(void);

		virtual enum ETransparentType get_trans_type() const
		{
			return ETrans_Opaque;
		}

		virtual void _level_loaded(nterrain_water* owner)
		{
			(void)owner;
		}
	protected:
		nDECLARE_VIRTUAL_CLASS(nwater_mtl)
	};

	/**
	 *	不产生反射、折射效果的基本水面材质
	*/
	class nwater_mtl_basic
		: public nwater_mtl
	{
	public:
		explicit nwater_mtl_basic(const nstring& name_str);
		virtual ~nwater_mtl_basic(void)	{}

		void create(const resource_location& texture_loc);

		virtual void draw_effect_param(nshading_effect* effect_ptr) const;
		virtual void serialize(narchive& ar);
	private:		
		nresource_texture::ptr	m_diffuse_map;

		nDECLARE_NAMED_CLASS(nwater_mtl_basic)
	};

	/**
	 *	使用反射/折射效果来模拟自然水面效果
	*/
	class nwater_mtl_natural
		: public nwater_mtl
	{
	public:
		explicit nwater_mtl_natural(const nstring& name_str);
		virtual ~nwater_mtl_natural(void)	{	destroy(); }

		void create(nterrain_water* owner, int render_target_w, int render_target_h,
			const resource_location& detail_map, const resource_location& bump_map);
		void destroy();

		virtual void draw_effect_param(nshading_effect* effect_ptr) const;	
		
		virtual void serialize(narchive& ar);
		virtual void _level_loaded(nterrain_water* owner);
	private:
		nterrain_water*		m_owner;
		
		nscene_capture_reflect::ptr	m_reflection;
		nscene_capture_refract::ptr	n_refraction;

		nresource_texture::ptr	m_detail_map,
								m_bump_map;		

		//-- for serialize
		nsize				m_rt_size;
		resource_location	m_detail_res,
							m_bump_res;

		nDECLARE_NAMED_CLASS(nwater_mtl_natural)
	};
}//namespace nexus
#endif //_NEXUS_WATER_MTL_H_