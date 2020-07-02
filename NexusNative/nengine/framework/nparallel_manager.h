#ifndef _NEXUS_PARALLEL_MANAGER_H_
#define _NEXUS_PARALLEL_MANAGER_H_
#include <queue>

namespace nexus
{
	struct nparallel_task
	{
		typedef boost::shared_ptr<nparallel_task> ptr;

		nparallel_task() {}
		virtual ~nparallel_task() {}

		virtual void execute() = 0;
	};
	template<typename T>
	struct nparallel_task_T
	{
		T	m_call_back;

		virtual void execute()
		{
			m_call_back();
		}
	};

	/**
	 *	处理并行计算
	*/
	class nparallel_manager
	{
	public:
		nparallel_manager(void);
		~nparallel_manager(void);

		template<typename FUNC>
		void add_task(FUNC call_back)
		{}

	private:
		std::queue<nparallel_task::ptr>	m_task_queue;
	};
}//namespace nexus

#endif //_NEXUS_PARALLEL_MANAGER_H_