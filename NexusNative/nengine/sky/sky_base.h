/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	July, 2009
*/

#ifndef _NEXUS_SKY_BASE_H_
#define _NEXUS_SKY_BASE_H_
#include "../framework/nactor.h"
#include "../actor/nstatic_mesh_component.h"

namespace nexus
{
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
	private:		

		nDECLARE_NAMED_CLASS(nsky_actor)
	};

	/**
	 *	天空组件的基类
	*/
	class nAPI nsky_component
		: public nstatic_mesh_component
	{
	public:
		typedef shared_ptr<nsky_component> ptr;
		nsky_component(const nstring& name_str);
		virtual ~nsky_component(void);	

		virtual const resource_location&	get_material_loc()
		{
			return m_material;
		}

		virtual void	set_material_loc(const resource_location& loc)
		{
			m_material=loc;
			nmtl_base::ptr mtl=nmtl_base::create_from_file(m_material);
			replace_material(0,0,mtl);
		}		

		virtual bool accept_dynamic_light() const	{	return false;	}
		virtual bool cast_dynamic_shadow() const {	return false;}

		virtual void serialize(narchive& ar);		
	private:
		resource_location	m_material;

		nDECLARE_NAMED_CLASS(nsky_component)
	};
}//namespace nexus
#endif //_NEXUS_SKY_BASE_H_