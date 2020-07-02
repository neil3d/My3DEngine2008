/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RESOURCE_IO_BASIC_H_
#define _NEXUS_RESOURCE_IO_BASIC_H_
#include "nresource_io.h"

namespace nexus
{
	/**
	 *	以阻塞模式，在调用者线程中直接load resoruce的io策略
	*/
	class nresource_io_basic
		: public nresource_io
	{
	public:
		nresource_io_basic(void);
		virtual ~nresource_io_basic(void);

		virtual void load_resource(nresource* res_ptr,
			nfile_system* fs, const nstring& pkg_name, const nstring& file_name, enum EResourceIOMode mode, enum EResourceIOPriority pri);

		virtual void cancel_resource_load(nresource* res_ptr);

	private:
		nDECLARE_CLASS(nresource_io_basic)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_IO_BASIC_H_