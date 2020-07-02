/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NRESOURCE_MANAGER_H_
#define _NRESOURCE_MANAGER_H_
#include "ncore.h"
#include "nresource.h"
#include "nresource_cache.h"
#include "nresource_io.h"

#include "nresource_static_mesh.h"
#include "nresource_anim_mesh.h"
#include "nresource_skeletal_mesh.h"
#include "nresource_skeletal_anim_set.h"

#include "../material/nmaterial_script.h"
#include "../material/nmaterial.h"

#include "../speed_tree/nspeed_base_tree.h"

namespace nexus
{
	

	class nAPI nresource_manager :
		public nobject
	{
	public:
		typedef shared_ptr<nresource_manager> ptr;

		nresource_manager(void);
		virtual ~nresource_manager(void);

		static nresource_manager* instance();

		void init();

		nresource_static_mesh::ptr		new_static_mesh(const nstring& res_name);
		nresource_anim_mesh::ptr		new_anim_mesh(const nstring& res_name);
		nresource_skeletal_mesh::ptr	new_skeletal_mesh(const nstring& res_name);
		nresource_skeletal_anim_set::ptr	new_skeletal_anim(const nstring& res_name);
		
		nresource_static_mesh::ptr	load_static_mesh(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_anim_mesh::ptr	load_anim_mesh(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nresource_texture::ptr		load_texture_2d(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		nmaterial_template::ptr		load_material_template_script(const resource_location& loc, 
			enum EResourceIOMode mode=EIO_Auto, enum EResourceIOPriority pri=EIOP_Normal);

		void free_resource(nresource* res);
		nresource::ptr find_resource_by_name(const nstring& res_name);

		nmaterial* get_default_material()	{	return &m_default_mtl;}
		size_t get_num_resource_cached()	{	return m_cache->get_num_resource(); }

		//-- speed tree
		//! 只支持阻塞模式的加载
		nspeed_base_tree::ptr load_speed_tree(const resource_location& loc);

	private:
		template<typename T>
		boost::intrusive_ptr<T> load_resource_T(const resource_location& loc, 
			enum EResourceIOMode mode, enum EResourceIOPriority pri);
		
		template<typename T>
		boost::intrusive_ptr<T> create_empty_resource_T(const nstring& res_name);

	private:
		friend class nspt_resource_manager;//speed tree特殊资源管理器，公用cache，io对象

		scoped_ptr<nresource_cache>	m_cache;
		scoped_ptr<nresource_io>	m_io;
		
		nmaterial		m_default_mtl;
		nmaterial		m_default_mtl_two_sided;
	};
}//namespace nexus
#endif //_NRESOURCE_MANAGER_H_