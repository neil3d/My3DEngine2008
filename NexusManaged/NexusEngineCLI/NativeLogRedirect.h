#pragma once

namespace NexusEngine
{
	public interface class ILogListener
	{
	public:
		void WriteString(enum class LogType t, System::String^ txt);
	};

	class NativeLogRedirect : public nexus::nlog_output
	{
		gcroot<ILogListener^>			m_clrObj;
	public:
		NativeLogRedirect(ILogListener^ log):m_clrObj(log)
		{}

		virtual void write_string(enum nexus::ELogType t, const TCHAR* txt)
		{
			m_clrObj->WriteString(static_cast<enum class LogType>(t),
				gcnew System::String(txt)
				);
		}
	};
}//namespace NexusEngine