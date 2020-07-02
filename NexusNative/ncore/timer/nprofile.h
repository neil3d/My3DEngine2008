#ifndef _NEXUS_CORE_PROFILE_H_
#define _NEXUS_CORE_PROFILE_H_
#include "../ncore.h"
#include "ntimestamp.h"

namespace nexus
{
	/**
	* profile time value
	*/
	class nCoreAPI profile_value
	{
	public:
		profile_value();

		// start profile
		void start();

		// stop profile
		// 返回m_deltatime
		float stop();
		
		float get_sumtime() const { return m_sumtime; }
		float get_deltatime() const { return m_deltatime; }
		float get_maxdeltatime() const { return m_maxdeltatime; }
		unsigned int get_count() const { return m_count; }

		float get_average() { return m_sumtime/m_count; } 

		nstring to_string();
	
	private:
		// 上一次调用start的时间
		float			m_lasttime;
		// 最近一次调用的间隔时间
		float			m_deltatime;
		// 最长的一次调用间隔时间
		float			m_maxdeltatime;
		// 总时间
		float			m_sumtime;
		// 总计调用的次数
		unsigned int	m_count;
		// 校验当前profile是否start 
		int				m_progress;
	};

	/**
	* 维护一组profile_value的性能分析单件实例
	*/
	class nCoreAPI profile_manager : public singleton<profile_manager>
	{
	public:
		typedef std::map<nname, profile_value> profile_pair_type;

		void add_profile(const nname& name)
		{
			profile_pair_type::const_iterator it = m_profiles.find(name);
			if(it == m_profiles.end())
			{
				m_profiles[name] = profile_value();
			}
		}

		size_t remove_profile(const nname& name)
		{
			return m_profiles.erase(name);
		}

		bool start_profile(const nname& name)
		{
			profile_pair_type::iterator it = m_profiles.find(name);
			if(it != m_profiles.end())
			{
				it->second.start();
				return true;
			}
			return false;
		}

		bool stop_profile(const nname& name)
		{
			profile_pair_type::iterator it = m_profiles.find(name);
			if(it != m_profiles.end())
			{
				it->second.stop();
				return true;
			}
			return false;
		}

		profile_value* find_profile(const nname& name)
		{
			profile_pair_type::iterator it = m_profiles.find(name);
			if(it != m_profiles.end())
			{
				return 	&(it->second);
			}
			else
			{
				return NULL;
			}
		}
	
		// 迭代器
		size_t get_num_profile() const { return m_profiles.size(); }

		profile_value* first_profile()
		{
			m_iter = m_profiles.begin(); 
			if(m_iter!=m_profiles.end())
			{
				return 	&(m_iter->second);
			}
			return NULL;
		}

		profile_value* next_profile()
		{
			if( ++m_iter != m_profiles.end())
			{
				return 	&(m_iter->second);
			}
			return NULL;
		}

		const profile_pair_type& get_profiles() const { return m_profiles; }

	private:
		profile_pair_type m_profiles;
		profile_pair_type::iterator m_iter;
	};

	#define START_PROFILE_COUNTER(name) \
		profile_manager* manager = profile_manager::instance();	\
		nname profile_name = nname(name);						\
		manager->add_profile(profile_name);						\
		manager->start_profile(profile_name);

	#define END_PROFILE_COUNTER(name) \
		profile_manager::instance()->stop_profile(nname(name));
	
}// end of namespace nexus

#endif //_NEXUS_CORE_PROFILE_H_
