/**
 *	nexus ngame - ncreature
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Apr, 2010
 */

#ifndef _NGAME_NCREATURE_H_
#define _NGAME_NCREATURE_H_

#include "ncommon.h"
#include "nunit.h"

namespace nexus {

	class ncreature : public nunit
	{
	public:
		ncreature();
		virtual ~ncreature();

		bool init();
		void destroy();
		void update(uint32 elapse);

		virtual void make_enter_msg(void* data_ptr, uint32& size) const;

	protected:
		virtual void _update_max_health();
		virtual void _update_max_energy();
		virtual void _update_max_mana();
		virtual void _update_melee_damage();
		virtual void _update_kungfu_damage();
		virtual void _update_spell_damage();
		virtual void _update_agility();
		virtual void _update_health_regeneration();
		virtual void _update_power_Regeneration();

		virtual void _regenerate_health(uint32 elapse);
		virtual void _regenerate_energy(uint32 elapse);
		virtual void _regenerate_mana(uint32 elapse);

	private:

	};

} // namespace nexus

#endif // _NGAME_NCREATURE_H_
