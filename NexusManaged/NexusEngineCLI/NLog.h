/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NativeLogRedirect.h"

namespace NexusEngine
{
	public enum class LogType : System::Int32
	{
		Info		=	ELog_Info,
		Warning		=	ELog_Warning,
		Error		=	ELog_Error,
		Exception	=	ELog_Exception,
		Debug		=	ELog_Debug,

		MAX			=	ELog_MAX,
	};

	public ref class NLogOutput
	{
	protected:
		explicit NLogOutput(nlog_output* output) : m_NativeOutput(output)
		{
		}

	public:
		NLogOutput() {}

		virtual ~NLogOutput()
		{
			if(m_NativeOutput != NULL)
			{
				delete m_NativeOutput;
				m_NativeOutput = NULL;
			}
		}

		property nlog_output* NativePtr
		{
			nlog_output* get()
			{
				return m_NativeOutput;
			}
			void set(nlog_output* output)
			{
				if(m_NativeOutput != NULL)
				{
					delete m_NativeOutput;
					m_NativeOutput = NULL;
				}
				m_NativeOutput = output;
			}
		}

		virtual void WriteString(LogType t, System::String^ message);

	private:
		nlog_output* m_NativeOutput;
	};

	public ref class NFileLogOutput
		: public NLogOutput
	{
	public:
		explicit NFileLogOutput(System::String^ fileName);
	};

	public ref class NStderrLogOutput
		: public NLogOutput
	{
	public:
		NStderrLogOutput();
	};

	public ref class NConsoleLogOutput
		: public NLogOutput
	{
	public:
		NConsoleLogOutput();
	};

	/**
	 * native log对象包装
	*/
	public ref class NLogger
	{
	private:
		NLogger(void);

		static NLogger^ s_instance;
	public:
		static NLogger(void)
		{
			s_instance = gcnew NLogger;
		}

		virtual ~NLogger()
		{
			Clear();
		}

		static property NLogger^ Instance
		{
			NLogger^ get()
			{
				return s_instance;
			}
		}

		void AddLogOutput(NLogOutput^ output)
		{
			m_LogOutpurs->Add(output);
			nlog::instance()->add_log_output(log_output_ptr(output->NativePtr));
		}

		void AddLogListener(ILogListener^ listener)
		{
			log_output_ptr logRedirect( new NativeLogRedirect(listener) );
			nlog::instance()->add_log_output(logRedirect);
			//m_LogOutpurs->Add(logRedirect);
		}

		void Clear()
		{
			m_LogOutpurs->Clear();
			nlog::instance()->clear_log_output();
		}

		void WriteString(LogType t, System::String^ message);

	private:
		static System::Collections::Generic::List<NLogOutput^>^	m_LogOutpurs = gcnew System::Collections::Generic::List<NLogOutput^>();
	};
}//namespace NexusEngine