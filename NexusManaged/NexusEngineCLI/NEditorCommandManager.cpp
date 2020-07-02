#include "StdAfx.h"
#include "NEditorCommandManager.h"

namespace NexusEngine
{
	//-- class NNativeEditorCommand --------------------------------------------------------------------
	NNativeEditorCommand::NNativeEditorCommand(neditor_cmd::ptr nativeCmd)
	{
		m_nativeCmd = new neditor_cmd::ptr( nativeCmd );
	}

	NNativeEditorCommand::~NNativeEditorCommand()
	{
		delete m_nativeCmd;
	}

	bool NNativeEditorCommand::Execute()
	{
		if(m_nativeCmd)
			return (*m_nativeCmd)->execute();
		return false;
	}

	bool NNativeEditorCommand::Undo()
	{
		if(m_nativeCmd)
			return (*m_nativeCmd)->undo();
		return false;
	}

	bool NNativeEditorCommand::Redo()
	{
		if(m_nativeCmd)
			return (*m_nativeCmd)->redo();
		return false;
	}

	//-- class NEditorCommandManager --------------------------------------------------------------------
	NEditorCommandManager::NEditorCommandManager(void)
	{
		m_stackSize = 64;
		m_undoStack = gcnew System::Collections::ArrayList;
		m_redoStack = gcnew System::Collections::ArrayList;
	}

	NEditorCommandManager::~NEditorCommandManager(void)
	{
	}
}//namespace NexusEngine