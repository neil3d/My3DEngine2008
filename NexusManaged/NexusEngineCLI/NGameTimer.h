#pragma once

namespace NexusEngine
{
	public ref class NGameTimer
	{
	public:
		NGameTimer(void);

		void Reset();
		void Tick();		
		
		property float ElapsedTime
		{
			float get()
			{
				return (m_currentTime-m_startTime)*0.001f;
			}
		}

		property float DeltaTime
		{
			float get()
			{
				return m_deltaTime*0.001f;
			}
		}

	private:
		unsigned int m_startTime;
		unsigned int m_currentTime;
		unsigned int m_deltaTime;
	};
}//namespace NexusEngine