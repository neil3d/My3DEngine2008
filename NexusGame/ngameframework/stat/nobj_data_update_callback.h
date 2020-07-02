#ifndef _NEXUS_GAMEFRAMEWORK_NOBJ_DATA_UPDATE_CALLBACK_H
#define _NEXUS_GAMEFRAMEWORK_NOBJ_DATA_UPDATE_CALLBACK_H

namespace nexus
{
	/** 
	* 属性改变回调机制, 参数
	* 用回调而不用继承是为了能让native code调用托管代码
	*/
	struct nobj_data_update_callback
	{
		nobj_data_update_callback() {};

		virtual void on_stat_update() = 0;

		DISALLOW_COPY_AND_ASSIGN(nobj_data_update_callback);
	};

	typedef boost::shared_ptr<nobj_data_update_callback> nobj_data_update_callback_ptr;
}

#endif // end of _NEXUS_GAMEFRAMEWORK_NOBJ_DATA_UPDATE_CALLBACK_H