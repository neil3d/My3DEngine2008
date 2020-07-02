/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	Oct, 2008
*/

#ifndef _NEXUS_RESOURCE_IO_H_
#define _NEXUS_RESOURCE_IO_H_
#include "nresource.h"

namespace nexus
{

	enum EResourceIOMode
	{
		EIO_Auto,	// 自动模式：根据系统创建时使用的io class来觉得是否异步
		EIO_Block,	// 强制为阻塞模式
	};

	enum EResourceIOPriority
	{
		EIOP_Min = 0,
		EIOP_Low,
		EIOP_Normal,
		EIOP_High,
		EIOP_Max
	};

	/**
	 *	resource io策略的基类
	*/
	class nresource_io
		: public nobject
	{
	public:
		nresource_io(void)	{}
		virtual ~nresource_io(void)	{}

		virtual void load_resource(nresource* res_ptr,
			nfile_system* fs, const nstring& pkg_name, const nstring& file_name, enum EResourceIOMode mode, enum EResourceIOPriority pri) = 0;
		
		virtual void cancel_resource_load(nresource* res_ptr) = 0;

	private:
		nDECLARE_VIRTUAL_CLASS(nresource_io)
	};
}//namespace nexus

#endif //_NEXUS_RESOURCE_IO_H_