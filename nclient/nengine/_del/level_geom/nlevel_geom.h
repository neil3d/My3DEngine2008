/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_LEVEL_GEOM_H_
#define _NEXUS_LEVEL_GEOM_H_
#include "ncore.h"

namespace nexus
{
	/**
	*	level中不可交互的对象的基类
	*	@remark 可能是static mesh, BSP mesh, Terrain等.
	*/
	class nAPI nlevel_geom
		: public nobject
	{
	public:
		typedef shared_ptr<nlevel_geom> ptr;

		nlevel_geom(const nstring& name_str):m_name(name_str)
		{	
			mat_set_identity(m_mat_world);
		}
		virtual ~nlevel_geom(void)	{	}

		virtual void render() = 0;		
		virtual const box_sphere_bounds& get_bounds() const = 0;

		void set_world_matrix(const matrix44& mat)	{	m_mat_world = mat;}
		const nname& get_name() const	{	return m_name; }
	protected:
		nname		m_name;
		matrix44	m_mat_world;

		nDECLARE_VIRTUAL_CLASS(nlevel_geom)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_GEOM_H_