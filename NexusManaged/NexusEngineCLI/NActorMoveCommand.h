#pragma once
#include "NEditorCommandManager.h"
#include "NActor.h"

namespace NexusEngine
{
	ref class NActorMoveCommand
		: public NEditorCommandBase
	{
	public:
		NActorMoveCommand(NActor^ targetActor, ObjectSpace newLoc)
		{
			m_targetActor = targetActor;
			m_newLoc = newLoc;
			m_mouseMove = false;
		}
		virtual ~NActorMoveCommand(void);

		virtual bool Execute();
		virtual bool Undo();
		virtual bool Redo();
		property System::String^ Name// 返回一个简短的名称
		{
			virtual System::String^ get()	
			{
				return "Actor Move";
			}
		}
		property System::String^ Desc	// 返回一个稍长的描述
		{			
			virtual System::String^ get();			
		}

		// 是否是Mouse Move，鼠标拖动操作
		property bool MouseMove
		{
			bool get()			{	return m_mouseMove;}
			void set(bool val)	{	m_mouseMove=val;}
		}

		void Update(ObjectSpace newLoc)
		{
			m_newLoc = newLoc;
		}

		ObjectSpace GetNewLoc()
		{
			return m_newLoc;
		}

		NActor^	GetTargetActor()
		{
			return m_targetActor;
		}

	private:
		NActor^			m_targetActor;
		ObjectSpace		m_oldLoc;
		ObjectSpace		m_newLoc;
		bool			m_mouseMove;
	};

	ref class NMultiActorMoveCommand:public NEditorCommandBase
	{
	public:
		NMultiActorMoveCommand()
		{
			m_mouseMove = false;
		}
		virtual ~NMultiActorMoveCommand(void);

		virtual bool Execute();
		virtual bool Undo();
		virtual bool Redo();

		virtual void AddCommand(NActor^ targetActor, ObjectSpace newLoc)
		{
			m_Action.Add(gcnew NActorMoveCommand(targetActor,newLoc));
		}

		virtual void AddCommand(NActorMoveCommand^ cmd)
		{
			m_Action.Add(cmd);
		}

		property System::String^ Name// 返回一个简短的名称
		{
			virtual System::String^ get()	
			{
				return "Actors Move";
			}
		}

		property System::String^ Desc	// 返回一个稍长的描述
		{			
			virtual System::String^ get()
			{
				return "Actors Move";
			}
		}

		// 是否是Mouse Move，鼠标拖动操作
		property bool MouseMove
		{
			bool get()			{	return m_mouseMove;}
			void set(bool val)	{	m_mouseMove=val;}
		}

	private:
		System::Collections::Generic::List<NActorMoveCommand^>	m_Action;
		bool			m_mouseMove;
	};

}//namespace NexusEngine