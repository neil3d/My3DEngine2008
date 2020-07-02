#include "StdAfx.h"
#include "NActorMoveCommand.h"
#include "NEditorEngine.h"

namespace NexusEngine
{
	NActorMoveCommand::~NActorMoveCommand(void)
	{
	}

	bool NActorMoveCommand::Execute()
	{
		if(m_targetActor == nullptr
			|| m_targetActor->Destroyed)
			return false;

		m_oldLoc = m_targetActor->SpaceData;
		m_targetActor->SpaceData = m_newLoc;		

		return true;
	}

	bool NActorMoveCommand::Undo()
	{
		if(m_targetActor == nullptr
			|| m_targetActor->Destroyed)
			return false;

		m_targetActor->SpaceData = m_oldLoc;
		NEditorEngine::Instance()->ActorEd->ActorMoved();
		return true;
	}

	bool NActorMoveCommand::Redo()
	{
		if(m_targetActor == nullptr
			|| m_targetActor->Destroyed)
			return false;

		m_targetActor->SpaceData = m_newLoc;
		NEditorEngine::Instance()->ActorEd->ActorMoved();

		return true;
	}

	System::String^ NActorMoveCommand::Desc::get()
	{
		System::String^ actorName = "None";
		if(m_targetActor!=nullptr)
			actorName = m_targetActor->Name;
		return System::String::Format("Actor Move - {0}", actorName);
	}

	//-----------------------------------------------------------------------------------

	NMultiActorMoveCommand::~NMultiActorMoveCommand( void )
	{

	}

	bool NMultiActorMoveCommand::Execute()
	{
		bool s=false;
		for(int i=0;i<m_Action.Count;++i)
		{
			if(m_Action[i]->Execute())
			{
				s=true;
			}
		}
		return s;
	}

	bool NMultiActorMoveCommand::Undo()
	{
		bool s=false;
		for(int i=0;i<m_Action.Count;++i)
		{
			if(m_Action[i]->Undo())
			{
				s=true;
			}
		}
		return s;
	}

	bool NMultiActorMoveCommand::Redo()
	{
		bool s=false;
		for(int i=0;i<m_Action.Count;++i)
		{
			if(m_Action[i]->Redo())
			{
				s=true;
			}
		}
		return s;
	}
}//namespace NexusEngine