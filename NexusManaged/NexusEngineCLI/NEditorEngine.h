/**
*	Nexus Engine
*
*	Author: Neil
*	Date:	August, 2009
*/
#pragma once
#include "NEngine.h"
#include "NViewport.h"
#include "NTerrainEditor.h"
#include "NavigateMapEditor.h"
#include "NActorEditorBase.h"
#include "NLevel.h"
#include "EditorCommand.h"
#include "NRenderElement.h"
#include "NEditorCommandManager.h"

namespace NexusEngine
{
	ref class NCamera;
	ref class ThumbnailCreatorManager;

	public value struct NEditorConfig
	{
		System::String^ actorEdClassAssembly;
		System::String^	actorEditorClass;
	};	
	
	class neditor_engine_cli;

	/**
	 *	编辑器使用的引擎基类
	*/
	public ref class NEditorEngine : public NEngine
	{
	public:
		System::EventHandler^ SelectionLockTag;

		//--
		NEditorEngine(void);

		static NEditorEngine^ Instance()
		{
			return safe_cast<NEditorEngine^>(NEngine::Instance());
		}

		//!	增加"缩略图管理"处理
		virtual void InitModules(NEngineConfig^ engineCfg, NEditorConfig editofCfg);

		//!	增加"缩略图管理"处理
		virtual void Close() override;

		//!	渲染指定的Level
		virtual void RenderLevel(System::String^ lvName, NViewport^ vp, bool present);		

		//!	渲染指定的Level,并更新HitProxyMap
		virtual void RenderMainLevel(System::String^ lvName, NViewport^ vp);
		

		//!	更新所有关卡中的对象
		virtual void UpdateLevel(System::String^ lvName, float delta_time,NViewport^ vp);

		System::IntPtr CreateThumbnail(NResourceLoc loc, int w, int h);		
		System::IntPtr LoadTextureAsBitmap(NResourceLoc loc);

		void MakeTextureAtlas(NResourceLoc inputFolder,NResourceLoc outFolder, int w, int h);
		
		//! 处理viewport的窗口消息
		//!	@return 0-没有处理这个消息，1已经处理了这个消息，2应由CameraCtrl处理这个消息
		virtual int HandleViewportMessage(System::Windows::Forms::Message% msg, NViewport^ view);		

		property EditorCommand Command
		{
			EditorCommand get()	{	return m_command; }
			void set(EditorCommand val)	{	m_command = val; }
		}

		property NTerrainEditor^ TerrainEd
		{
			NTerrainEditor^ get()	{	return m_terrainEd;}
		}

		property NavigateMapEditor^ NavigateMapEd
		{
			NavigateMapEditor^ get() { return m_navEditor; }
		}

		property NEditorCommandManager^ CommandMgr
		{
			NEditorCommandManager^ get()	{	return m_commandMgr;}
		}

		property NActorEditorBase^ ActorEd
		{
			NActorEditorBase^ get()	{	return m_actorEd;}
		}

		property NLevel^ MainLevel
		{
			NLevel^ get() { return m_mainLevel; }            
		}

		property System::String^ MainLevelName
		{
			System::String^ get() 
			{ 
				return MainLevel->Name; 
			}
		}        	

		property bool SelectionLocked
		{
			bool get()	{	return m_selectionLock;}
		}

		virtual void	CreateMainLevel(System::String^ levelName);

		NResourceLoc GetCurrentResourceLoc()
		{
			return m_curResrouce;
		}

	protected:
		void CreateSubEditors();		
		void ResetSubEditors();		

		EActorEditHandle PickActorHandle(NViewport^ vp, int mouseX, int mouseY);
		
		//!	鼠标拾取主关卡中的Actor
		NActor^ PickMainLevelActor(NViewport^ vp, int mouseX, int mouseY);

	protected:
		bool						m_selectionLock;
		EditorCommand				m_command;
		NEditorConfig				m_editofCfg;
		
		NLevel^						m_mainLevel;	// 正在编辑器的关卡
		NTerrainEditor^				m_terrainEd;
		NActorEditorBase^			m_actorEd;
		ThumbnailCreatorManager^	m_thumbCreator;
		NEditorCommandManager^		m_commandMgr;

		NResourceLoc				m_curFolder;   // 当前工作路径(文件夹)
		NResourceLoc				m_curResrouce; // 当前选择的资源(文件)

		// 导航图编辑器
		NavigateMapEditor^			m_navEditor;
		
		//-- Native Wrapper
	protected:
		property neditor_engine_cli* NativePtr
		{
			neditor_engine_cli* get();
		}
	};

	//-- native混合代码
	class neditor_engine_cli : public neditor_engine
	{
	public:
		neditor_engine_cli(void)	{}
		virtual ~neditor_engine_cli(void)	{}

		virtual void push_command(neditor_cmd::ptr new_cmd)
		{
			NNativeEditorCommand^ cmdCLI = gcnew NNativeEditorCommand(new_cmd);
			m_cmd_manager->PushCommand(cmdCLI);
		}

		void set_command_manager(NEditorCommandManager^ cmdMgr)
		{
			m_cmd_manager = cmdMgr;
		}
	protected:
		gcroot<NEditorCommandManager^>	m_cmd_manager;

	private:
		nDECLARE_CLASS(neditor_engine_cli)

	};
}//namespace NexusEngine