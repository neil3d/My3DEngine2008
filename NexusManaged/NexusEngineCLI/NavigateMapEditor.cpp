#include "StdAfx.h"
#include "NavigateMapEditor.h"
#include "NWrapperException.h"
#include "NLevel.h"


namespace NexusEngine
{
	
	NavigateMapEditor::NavigateMapEditor(void)
	{
		m_nav_map_editor = new nexus::nav_map_editor;
	}
	
	NavigateMapEditor::~NavigateMapEditor(void)
	{
		delete m_nav_map_editor;
		m_nav_map_editor = NULL;
	}

	bool NavigateMapEditor::Empty()
	{
		return m_nav_map_editor->empty();
	}

	void NavigateMapEditor::Close()
	{
		m_nav_map_editor->close();
	}

	void NavigateMapEditor::SetBrush( NTerrainBrush brush )
	{
		nexus::nterrain_brush nbrush;
		brush.ToNative(nbrush);
		m_nav_map_editor->set_brush(nbrush);
	}

	void NavigateMapEditor::ImportHeightmap( System::String^ imgFileName )
	{
		BEGIN_NATIVE_GUARD

			pin_ptr<const wchar_t> szImgFileName = PtrToStringChars(imgFileName);
		m_nav_map_editor->import_heightmap(szImgFileName);

		END_NATIVE_GUARD
	}

	bool NavigateMapEditor::InputMessgaeProc( System::Windows::Forms::Message% msg, NCamera^ cam )
	{
		bool ret = false;
		switch(msg.Msg)
		{
		case WM_LBUTTONDOWN:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt(LOWORD(lParam), HIWORD(lParam));
				m_nav_map_editor->on_mouse_left_down(npt);
				ret = true;
			}
			break;
		case WM_LBUTTONUP:
			m_nav_map_editor->on_mouse_left_up();
			ret = true;
			break;
		case WM_RBUTTONDOWN:
			m_nav_map_editor->on_mouse_right_down();
			ret = true;
			break;
		case WM_RBUTTONUP:
			m_nav_map_editor->on_mouse_right_up();
			ret = true;
			break;
		case WM_MOUSEMOVE:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt((short)LOWORD(lParam), (short)HIWORD(lParam));

				WPARAM wParam = msg.WParam.ToInt32();
				m_nav_map_editor->on_mouse_move(npt, (wParam&MK_CONTROL)!=0, cam->NativePtr);
				ret = true;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				WPARAM wParam = msg.WParam.ToInt32();
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				m_nav_map_editor->on_mouse_wheel(delta);
				ret = true;
			}
			break;
		}

		return ret;
	}

	void NavigateMapEditor::DrawWidgets( NRenderPrimitiveDrawInterface^ PDI )
	{
		m_nav_map_editor->draw_widgets(PDI->NativePtr);

	}

	void NavigateMapEditor::BeginNavigateMapEditor( NLevel^ level )
	{
		m_nav_map_editor->begin_navigate_map_editor( level->GetNativeSmartPtr() );
	}
	void NavigateMapEditor::EndNavigateMapEditor()
	{
		m_nav_map_editor->end_navigate_map_editor();
	}

	void NavigateMapEditor::MoveNavigationMap( Vector3 loc, Vector3 scale )
	{
		nexus::vector3* nativeLoc = reinterpret_cast<nexus::vector3*>(&loc);
		nexus::vector3* nativeScale = reinterpret_cast<nexus::vector3*>(&scale);
		m_nav_map_editor->move_navigation_map(*nativeLoc,*nativeScale);
	}

	void NavigateMapEditor::ResizeNavigationMap( UInt32 width, UInt32 heigth )
	{
		m_nav_map_editor->resize_navigation_map(width,heigth);
	}

	void NavigateMapEditor::ResetNavigationMap( UInt32 width, UInt32 heigth )
	{
		m_nav_map_editor->reset_navigation_map(width,heigth);
	}

	void NavigateMapEditor::RebuildNavigationMap(float maxHeight, float stepHegiht, bool use_line_check)
	{
		m_nav_map_editor->rebuild_navigation_map(maxHeight, stepHegiht, use_line_check);
	}
}
