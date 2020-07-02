#pragma once

namespace NexusEngine
{
	/*
	 *	Editor Command Manager设计概要
	 *	.关键在于native代码和managed代码都要操作command stack，所以command stack必须有CLI层来负责管理；
	 *	.native 代码需要操作CLI层的代码，这涉及到native代码调用managed代码的过程；这通过native的editor engine的派生类实现；
	*/

	/**
	 *	因为需要处理managed和native两种command，所以我们需要一个虚基类
	*/
	public interface class NEditorCommandBase
	{
	public:
		bool Execute();
		bool Undo();
		bool Redo();
		property System::String^ Name// 返回一个简短的名称
		{
			System::String^ get();
		}
		property System::String^ Desc	// 返回一个稍长的描述
		{			
			System::String^ get();			
		}
	};


	/**
	 *	包装native的command类对象
	*/
	public ref class NNativeEditorCommand
		: public NEditorCommandBase
	{
		neditor_cmd::ptr*	m_nativeCmd;
	public:
		NNativeEditorCommand(neditor_cmd::ptr nativeCmd);
		~NNativeEditorCommand();

		virtual bool Execute();
		virtual bool Undo();
		virtual bool Redo();
		property System::String^ Name
		{
			virtual System::String^ get()
			{
				if(m_nativeCmd)
				{
					nstring nstr = (*m_nativeCmd)->get_name();
					return gcnew System::String(nstr.c_str());
				}

				return gcnew System::String("Unknown Command");
			}
		}

		property System::String^ Desc
		{
			virtual System::String^ get()
			{
				if(m_nativeCmd)
				{
					nstring nstr = (*m_nativeCmd)->get_desc();
					return gcnew System::String(nstr.c_str());
				}

				return gcnew System::String("Unknown Command");
			}
		}
	};

	/**
	 *	command管理器，管理undo，redo两个command stack
	*/
	public ref class NEditorCommandManager
	{
	public:
		System::EventHandler^ UndoStackChanged;
		System::EventHandler^ RedoStackChanged;

	public:
		NEditorCommandManager(void);
		virtual ~NEditorCommandManager(void);

		property int StackSize
		{
			int get()	{	return m_stackSize;}
			void set(int val)	{	m_stackSize = val;}
		}

		void PushCommand(NEditorCommandBase^ cmd)
		{
			AppendCommand(cmd, m_undoStack);
			if(UndoStackChanged)
				UndoStackChanged(this, nullptr);
		}

		bool Undo()
		{
			bool ret = false;
			if (m_undoStack->Count > 0)
			{
				int lastIndex = m_undoStack->Count-1;
				NEditorCommandBase^ lastCmd =  safe_cast<NEditorCommandBase^>(m_undoStack[lastIndex]);
				ret = lastCmd->Undo();

				//-- 把这个command从undo栈移动到redo栈				
				m_undoStack->RemoveAt(lastIndex);
				if(UndoStackChanged)
					UndoStackChanged(this, nullptr);
				if( ret)
				{
					AppendCommand(lastCmd, m_redoStack);
					if(RedoStackChanged)
						RedoStackChanged(this, nullptr);
				}
			}
			return ret;
		}

		bool Redo()
		{
			bool ret = false;
			if (m_redoStack->Count > 0)
			{
				int lastIndex = m_redoStack->Count-1;
				NEditorCommandBase^ lastCmd = safe_cast<NEditorCommandBase^>(m_redoStack[lastIndex]);
				ret = lastCmd->Redo();

				//-- 把这个command从undo栈移动到redo栈
				m_redoStack->RemoveAt(lastIndex);
				if(RedoStackChanged)
					RedoStackChanged(this, nullptr);
				if( ret)
				{
					AppendCommand(lastCmd, m_undoStack);
					if(UndoStackChanged)
						UndoStackChanged(this, nullptr);
				}
			}
			return ret;
		}

		property NEditorCommandBase^ UndoStackTop
		{
			NEditorCommandBase^ get()	
			{
				if(m_undoStack->Count > 0)
					return  safe_cast<NEditorCommandBase^>(m_undoStack[m_undoStack->Count-1]);
				return nullptr;
			}
		}

		property NEditorCommandBase^ RedoStackTop
		{
			NEditorCommandBase^ get()	
			{
				if(m_redoStack->Count > 0)
					return  safe_cast<NEditorCommandBase^>(m_redoStack[m_redoStack->Count-1]);
				return nullptr;
			}
		}

	protected:
		void AppendCommand(NEditorCommandBase^ cmd, System::Collections::ArrayList^ cmdStack)
		{
			// 把新的command添加到后面
			cmdStack->Add(cmd);
			if(cmdStack->Count > m_stackSize)
			{
				// 移除最前面的一个
				cmdStack->RemoveAt(0);
			}
		}

	protected:
		int	m_stackSize;	// 最多保存多少个command在单个stack中
		System::Collections::ArrayList^ 	m_undoStack;
		System::Collections::ArrayList^ 	m_redoStack;
	};
}//namespace NexusEngine