
#include "ndb_mysql/ndb_utility.h"
#include "nlogin_utility.h"

namespace nexus{

#ifdef NULL_MUTEX
	//非线程安全的内存池
	nobject_pool<nnull_mutex>  g_pool(500 * 1024 * 1024);	//池的最大容量500M
#else
	//线程安全的内存池
	nobject_pool<nfast_mutex>  g_pool(500 * 1024 * 1024);	//池的最大容量500M
#endif

}