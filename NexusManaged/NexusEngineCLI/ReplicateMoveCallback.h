#pragma once
#include "savedmove/replicate_move_callback.h"

namespace NexusEngine
{
	public interface class IReplicateMoveCallback
	{
	public:
		void OnReplicateMove();
	};


	class ReplicateMoveCallback 
		: public nexus::replicate_move_callback
	{
	private:
		ReplicateMoveCallback(void){} 

	public:
		explicit ReplicateMoveCallback(IReplicateMoveCallback^ callback)
			: m_clrObj(callback)
		{}

		virtual void on_replicate_move()
		{
			m_clrObj->OnReplicateMove();
		}

	private:
		gcroot<IReplicateMoveCallback^>			m_clrObj;
	};
}
