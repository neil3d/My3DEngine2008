#pragma once

namespace NexusEngine
{
	public enum class ELogType : System::Int32
	{
		Info		=	ELog_Info,
		Warning		=	ELog_Warning,
		Error		=	ELog_Error,
		Exception	=	ELog_Exception,
	};

	public interface class NEditorLog
	{
	public:
		void WriteString(System::String^ txt);
		void WriteString(enum class ELogType t, System::String^ txt);
	};

	class NativeLogRedirect : public nexus::nlog_output
	{
		gcroot<NEditorLog^>			m_clrObj;
	public:
		NativeLogRedirect(NEditorLog^ log):m_clrObj(log)
		{}

		virtual void open(const nstring& param)
		{}

		virtual void write_string(const TCHAR* txt)
		{
			m_clrObj->WriteString(gcnew System::String(txt));
		}

		virtual void write_string(enum nexus::ELogType t, const TCHAR* txt)
		{
			m_clrObj->WriteString(static_cast<enum class ELogType>(t),
				gcnew System::String(txt)
				);
		}

		virtual void close()
		{}
	};
}//namespace NexusEngine