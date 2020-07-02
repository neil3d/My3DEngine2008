#include "ninfo_data.h"
#include "ncfg.h"
#include "nlog_mt.h"

#include "file/nstd_file_system.h"

namespace nexus {

	// 包名, 文件扩展
	const tstring INFO_PKG_NAME							= _T("attribute");
	const tstring SUFFIX_NAME							= _T(".att");

	// 文件名常量
	const tstring LEVELUP_INFO_FILE_NAME				= _T("nlevelup_info_dictionary") + SUFFIX_NAME;
	const tstring SPELL_INFO_FILE_NAME					= _T("nspell_info_dictionary") + SUFFIX_NAME;
	const tstring SPELL_EFFECT_INFO_FILE_NAME			= _T("nspell_effect_info_dictionary") + SUFFIX_NAME;
	const tstring SPELL_AURA_INFO_FILE_NAME				= _T("nspell_aura_info_dictionary") + SUFFIX_NAME;

	bool load_info_data()
	{
		tstring path = sncfg.get_tstring(ECI_ResPath);

		nstd_file_system fs;
		fs.init(path);

		NLOG_STRING(_T("nexus ngame loading %s!"), LEVELUP_INFO_FILE_NAME.c_str());

		if (!s_levelup_info.serialize(&fs, INFO_PKG_NAME, LEVELUP_INFO_FILE_NAME, EFileRead))
		{
			NLOG_ERROR(_T("load %s error!"), LEVELUP_INFO_FILE_NAME.c_str());
			return false;
		}

		NLOG_STRING(_T("nexus ngame loading %s!"), SPELL_INFO_FILE_NAME.c_str());

		if (!s_spell_info.serialize(&fs, INFO_PKG_NAME, SPELL_INFO_FILE_NAME, EFileRead))
		{
			NLOG_ERROR(_T("load %s error!"), SPELL_INFO_FILE_NAME.c_str());
			return false;
		}

		NLOG_STRING(_T("nexus ngame loading %s!"), SPELL_EFFECT_INFO_FILE_NAME.c_str());

		if (!s_spell_effect_info.serialize(&fs, INFO_PKG_NAME, SPELL_EFFECT_INFO_FILE_NAME, EFileRead))
		{
			NLOG_ERROR(_T("load %s error!"), SPELL_EFFECT_INFO_FILE_NAME.c_str());
			return false;
		}

		NLOG_STRING(_T("nexus ngame loading %s!"), SPELL_AURA_INFO_FILE_NAME.c_str());

		if (!s_spell_aura_info.serialize(&fs, INFO_PKG_NAME, SPELL_AURA_INFO_FILE_NAME, EFileRead))
		{
			NLOG_ERROR(_T("load %s error!"), SPELL_AURA_INFO_FILE_NAME.c_str());
			return false;
		}

		return true;
	}

} // namespace nexus
