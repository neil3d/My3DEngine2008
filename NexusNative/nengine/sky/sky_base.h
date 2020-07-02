/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/

#ifndef _NEXUS_SKY_BASE_H_
#define _NEXUS_SKY_BASE_H_
#include "../framework/nactor.h"
#include "../actor/nprimitive_component.h"
#include "../renderer/nrender_proxy.h"

namespace nexus
{
	class ndynamic_sky_dome;
	/**
	 *	天空的管理包装类
	*/
	class nAPI nsky_actor
		: public nactor
	{
	public:
		typedef boost::shared_ptr<nsky_actor> ptr;

		nsky_actor(const nstring& name_str);
		virtual ~nsky_actor(void);

		/**	创建简单的天空盒*/
		void create_sky_box(const resource_location& cube_map_loc);

		/** 创建动态天穹 */
		boost::shared_ptr<ndynamic_sky_dome> create_sky_dome(float radius, int tesselation);		
	private:		

		nDECLARE_NAMED_CLASS(nsky_actor)
	};

	/**
	 *	天空组建的积累
	*/
	class nAPI nsky_component
		: public nprimitive_component
	{
	public:
		nsky_component(const nstring& name_str);
		virtual ~nsky_component(void);

		virtual void _init(nactor* owner);		
		virtual void render(const nviewport& view);

		virtual nmaterial_base* get_material(int lod, int mtl_id) = 0;		
		virtual nrender_mesh* get_render_mesh(int lod)  = 0;		

		transparent_flag get_transparent_flag() const	{	return m_trans_flag; }
		virtual enum EDepthGroup get_depth_group() const	{	return EDG_Background; }		

		virtual void serialize(narchive& ar);		
	private:
		struct sky_proxy : public primitive_proxy
		{
			nsky_component* get_owner() const
			{
				return (nsky_component*)m_comp;
			}

			virtual int get_render_lod() const	{	return 0; }
			virtual enum EDepthGroup get_depth_group() const	{	return get_owner()->get_depth_group(); }		
			virtual nrender_mesh* get_render_mesh(int lod) const	{	return get_owner()->get_render_mesh(lod); }
			virtual nmaterial_base* get_material(int lod, int mtl_id) const	{	return get_owner()->get_material(lod, mtl_id);}
			virtual transparent_flag get_transparent_flag() const	{	return get_owner()->get_transparent_flag();}
		};

		transparent_flag	m_trans_flag;
		sky_proxy			m_render_obj;

		nDECLARE_VIRTUAL_CLASS(nsky_component)
	};
}//namespace nexus
#endif //_NEXUS_SKY_BASE_H_