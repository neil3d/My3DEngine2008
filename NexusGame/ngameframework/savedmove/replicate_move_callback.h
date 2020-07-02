#ifndef _NEXUS_GAMEFRAMEWORK_REPLICATE_MOVE_CALLBACK_H_
#define _NEXUS_GAMEFRAMEWORK_REPLICATE_MOVE_CALLBACK_H_

namespace nexus
{
	/** 
	* saved move replicate 回调机制
	* 用回调而不用继承是为了能让native code调用托管代码
	*/
	struct replicate_move_callback
	{
		replicate_move_callback() {};

		virtual void on_replicate_move() = 0;

		DISALLOW_COPY_AND_ASSIGN(replicate_move_callback);
	};

	typedef boost::shared_ptr<replicate_move_callback> replicate_move_callback_ptr;
}

#endif // end of _NEXUS_GAMEFRAMEWORK_REPLICATE_MOVE_CALLBACK_H_