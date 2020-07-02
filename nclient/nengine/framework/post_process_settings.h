/**
*	Nexus Engine - 后期处理特效设置
*
*	Author: Neil
*	Date:	Nov, 2008
*/

#ifndef _NEXUS_POST_PROCESS_SETTINGS_H_
#define _NEXUS_POST_PROCESS_SETTINGS_H_

namespace nexus
{
	struct post_process_effect
	{};

	struct motion_blur_effect : public post_process_effect
	{};

	struct bloom_effect : public post_process_effect
	{};

	// depth of field
	struct DOF_effect : public post_process_effect
	{};

	struct tone_map_effect : public post_process_effect
	{};

	class post_process_settings
	{
	public:
		post_process_settings(void);
		~post_process_settings(void);
	};
}//namespace nexus

#endif //_NEXUS_POST_PROCESS_SETTINGS_H_