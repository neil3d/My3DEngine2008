#include "StdAfx.h"
#include "NTerrainEditor.h"
#include "NWrapperException.h"

namespace NexusEngine
{
	void NTerrainBrush::ToNative(nexus::nterrain_brush& brush)
	{
		brush.op			= static_cast<nexus::ETrnBrushOP>(op);
		brush.shape			= static_cast<nexus::ETrnBrushShape>(shape);
		brush.lerp_mode		= static_cast<nexus::ETrnLerpMode>(lerpMode);
		brush.inner_radius	= innerRadius;
		brush.outer_radius	= outerRadius;
		brush.strength		= strength;
		brush.noise_freq	= noiseFreq;
		brush.target		= static_cast<nexus::ETrnBurshTarget>(target);
		brush.target_index	= targetIndex;
	}

	NTerrainEditor::NTerrainEditor(void)
	{
		m_nativeEditor = new nexus::nterrain_editor;
	}

	NTerrainEditor::~NTerrainEditor(void)
	{
		delete m_nativeEditor;
		m_nativeEditor = NULL;
	}

	void NTerrainEditor::BindTerrain(NTerrainActor^ trn)
	{
		m_trn = trn;
		if (trn)
		{
			m_nativeEditor->bind_terrain(trn->GetSmartPtr());
		}
		else
		{
			m_nativeEditor->bind_terrain(nterrain_actor::ptr());
		}		

		if(TerranActorBinded)
			TerranActorBinded(this, nullptr);
	}

	void NTerrainEditor::Close()
	{
		m_nativeEditor->close();
	}

	void NTerrainEditor::GenerateNoise(System::Windows::Int32Rect rc, int numOctaves, float amplitude, float frequency)
	{
		nrect nrc;
		nrc.left = rc.X;
		nrc.right = rc.X+rc.Width;
		nrc.top = rc.Y;
		nrc.bottom = rc.Y+rc.Height;

		m_nativeEditor->generate_noise(nrc, numOctaves, amplitude, frequency);
	}

	void NTerrainEditor::ImportHeightmap(System::String^ imgFileName)
	{
		BEGIN_NATIVE_GUARD

		pin_ptr<const wchar_t> szImgFileName = PtrToStringChars(imgFileName);
		m_nativeEditor->import_heightmap(szImgFileName);

		END_NATIVE_GUARD
	}

	bool NTerrainEditor::InputMessgaeProc(System::Windows::Forms::Message% msg, NCamera^ cam)
	{
		bool ret = false;
		switch(msg.Msg)
		{
		case WM_LBUTTONDOWN:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt(LOWORD(lParam), HIWORD(lParam));
				m_nativeEditor->on_mouse_left_down(npt);
				ret = true;
			}
			break;
		case WM_LBUTTONUP:
			m_nativeEditor->on_mouse_left_up();
			ret = true;
			break;
		case WM_RBUTTONDOWN:
			m_nativeEditor->on_mouse_right_down();
			ret = true;
			break;
		case WM_RBUTTONUP:
			m_nativeEditor->on_mouse_right_up();
			ret = true;
			break;
		case WM_MOUSEMOVE:
			{
				LPARAM lParam = msg.LParam.ToInt32();
				npoint npt((short)LOWORD(lParam), (short)HIWORD(lParam));

				WPARAM wParam = msg.WParam.ToInt32();
				m_nativeEditor->on_mouse_move(npt, (wParam&MK_CONTROL)!=0, cam->NativePtr);
				ret = true;
			}
			break;
		case WM_MOUSEWHEEL:
			{
				WPARAM wParam = msg.WParam.ToInt32();
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				m_nativeEditor->on_mouse_wheel(delta);
				ret = true;
			}
			break;
		}

		return ret;
	}

	void NTerrainEditor::DrawWidgets(NRenderPrimitiveDrawInterface^ PDI)
	{
		m_nativeEditor->draw_widgets(PDI->NativePtr);
	}

	void NTerrainEditor::SetBrush(NTerrainBrush brush)
	{
		nexus::nterrain_brush nbrush;
		brush.ToNative(nbrush);
		m_nativeEditor->set_brush(nbrush);
	}

	bool NTerrainEditor::Empty()
	{
		return m_nativeEditor->empty();
	}

	NTerrainMtlSetup^ NTerrainEditor::GetMaterial()
	{
		return m_trn->GetMaterial();
	}

	void NTerrainEditor::ShowChunkEdge(bool s)
	{
		m_nativeEditor->show_chunk_edge(s);
	}
}//namespace NexusEngine