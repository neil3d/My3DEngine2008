#ifndef _NEXUS_SPECIAL_EFFECT_INSTANCE_H_
#define _NEXUS_SPECIAL_EFFECT_INSTANCE_H_
#include "../actor/nprimitive_component.h"
#include "nresource_special_effect.h"

namespace nexus
{
	class nAPI nspecial_effect_instance
		: public nprimitive_component
	{
	public:
		typedef boost::shared_ptr<nspecial_effect_instance> ptr;

		nspecial_effect_instance(const nstring& name_str);
		virtual ~nspecial_effect_instance(void);

		void reset_resource(nresource_special_effect::ptr new_res);

		virtual void update(float delta_time, const nviewport& view);
		virtual void render(const nviewport& view);

		int get_render_lod() const {	return 0;}

		//-- 特效播放接口
		void play();
		void close(bool fade);
		bool is_closed() const	{	return !m_playing; }
		void set_visible(bool v){m_visible=v;}
		bool is_visible(){return m_visible;}
		virtual void on_event(nevent_base& evt);

	protected:
		virtual void on_resource_ready();
		bool	m_playing;		// 是否正在播放
		float	m_play_time;	// 播放了多少时间
		bool	m_visible;

		nresource_special_effect::ptr	m_res;

		nDECLARE_NAMED_CLASS(nspecial_effect_instance)
	};
}//namespace nexus

#endif //_NEXUS_SPECIAL_EFFECT_INSTANCE_H_