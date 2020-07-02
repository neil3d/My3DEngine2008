#ifndef _NEXUS_RESOURCE_ASYNC_IO_H_
#define _NEXUS_RESOURCE_ASYNC_IO_H_
#include "nresource.h"
#include "nresource_io.h"

namespace nexus
{
	class nresource_async_io
		: public nresource_io
	{
		enum ERequestType
		{
			ECmd_None,
			ECmd_LoadResource,	// 加载一个资源			
		};

		struct request
		{
			enum ERequestType	req_type;
			enum EResourceIOPriority	priority;
			nresource*			res_ptr;
			nfile_system*		file_sys;
			nstring				pkg_name;
			nstring				file_name;

			request():req_type(ECmd_None), res_ptr(NULL), file_sys(NULL),priority(EIOP_Normal)
			{}
		};
	public:
		nresource_async_io(void);
		virtual ~nresource_async_io(void);

		void block_till_all_requests_finish();

	private:

	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_ASYNC_IO_H_