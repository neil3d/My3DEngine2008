/**
 *	nexus ngame - ninfo_data
 *
 *	Copyright (C) 2010 NexusDev
 *
 *	Author: D. Zhao
 *	Date:	Jul, 2010
 */

#ifndef _NGAME_NINFO_DATA_H_
#define _NGAME_NINFO_DATA_H_

#include "ncommon.h"
#include "ngameframework.h"
#include "nsingleton.h"

#include "nspell_define.h"

namespace nexus {

	template<typename data_type>
	class ninfo_data : public gameframework::natt_info_data<data_type>
	{
	public:
		data_type const* get_info(const nname& id) const
		{
			return find_info(id).get();
		}

		data_type const* get_info(uint32 crc_id) const
		{
			return find_info(crc_id).get();
		}
	};

#define s_levelup_info			nsingleton<ninfo_data<levelup_info>>::instance()
#define s_spell_info			nsingleton<ninfo_data<nspell_info>>::instance()
#define s_spell_effect_info		nsingleton<ninfo_data<nspell_effect_info>>::instance()
#define s_spell_aura_info		nsingleton<ninfo_data<nspell_aura_info>>::instance()

	bool load_info_data();

} // namespace nexus

#endif // _NGAME_NINFO_DATA_H_
