/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Aug, 2008
*/

#ifndef _NEXUS_LEVEL_H_
#define _NEXUS_LEVEL_H_
#include "ncore.h"
#include "nevent.h"
#include "../scene/nscene_manager.h"
#include "nactor.h"
#include "view_info.h"

namespace nexus
{
	/**
	 *	关卡的基类
	 *	@remark 组合actors和level geom对象，前者主要代表可交互的，后者主要用作静态装饰（没有游戏逻辑，但可以有动画）
	*/
	class nAPI nlevel :
		public nevent_target
	{
	public:
		typedef shared_ptr<nlevel> ptr;

		nlevel(const nstring& name_str);
		virtual ~nlevel(void);

		virtual void init(const nstring& scene_class_name);
		
		virtual nactor::ptr create_actor(const nstring& actor_name, const nstring& class_name=_T("nactor"));
		virtual void free_actor(const nname& actor_name);

		template<typename NClass>
		shared_ptr<NClass> create_actor_t(const nstring& obj_name)
		{
			return dynamic_pointer_cast<NClass>( 
				create_actor(obj_name, 
					NClass::reflection_get_static_class()->get_name())
				);
		}

		virtual void update(float delta_time, const view_info& view);
		virtual void render(const view_info& view);
		virtual void on_event(nevent_base& evt);

		void clear_actor();

		nscene_manager::ptr get_scene_manager() {return m_scene_ptr;}
		const nstring& get_name() const	{	return m_name; }
	protected:
		typedef map<nname, nactor::ptr>		nactor_map;
		
		nactor_map			m_all_actor;		

		nstring				m_scene_class_name;
		nscene_manager::ptr	m_scene_ptr;

	private:
		nstring	m_name;

		nDECLARE_NAMED_CLASS(nlevel)
	};
}//namespace nexus
#endif //_NEXUS_LEVEL_H_