#include "StdAfx.h"
#include "WxEngineConfigDlg.h"

WxEngineConfigDlg::WxEngineConfigDlg(wxWindow *parent):wxDialog(parent,
																wxID_ANY, 
																wxString(_T("nexus engine config")),
																wxDefaultPosition,
																wxSize(298, 260)
																)
{
	create_controls();
	CenterOnScreen(wxBOTH);
}

WxEngineConfigDlg::~WxEngineConfigDlg(void)
{
}

static wxSize _ctrlSize(120, 20);
static wxPoint _getCtrlPos(int row, int col)
{
	wxPoint start(4,4);
	wxSize space(4,8);
	return wxPoint( (_ctrlSize.x+space.x)*col+start.x,
		(_ctrlSize.y+space.y)*row+start.y );
}

void WxEngineConfigDlg::create_controls()
{
	long static_style = wxALIGN_CENTER|wxBORDER_SUNKEN;
	wxStaticText* stc = NULL;

	wxPoint box_offset(18,20);
	wxPoint box_pt(4,4);
	wxSize	box_size(280, 80);
	wxStaticBox* box_sys = new wxStaticBox(this, -1, _T("system plugin"), box_pt, box_size);
	//-- renderer class
	stc = new wxStaticText(this, -1, _T("renderer class:"), 
		_getCtrlPos(0,0)+box_offset, _ctrlSize, static_style);
	m_cmb_renderer = new wxComboBox(this, -1, _T(""), 
		_getCtrlPos(0,1)+box_offset, _ctrlSize );

	init_class_combo_list(m_cmb_renderer, 
		nexus::nrenderer_base::reflection_get_static_class() );	

	//-- file system class
	stc = new wxStaticText(this, -1, _T("file sys class:"), 
		_getCtrlPos(1,0)+box_offset, _ctrlSize, static_style);
	m_cmb_file_sys = new wxComboBox(this, -1, _T(""), 
		_getCtrlPos(1,1)+box_offset, _ctrlSize );

	init_class_combo_list(m_cmb_file_sys, 
		nexus::nfile_system::reflection_get_static_class());

	//-- resolution
	box_pt = wxPoint(4, box_size.y+4);
	box_offset = box_pt+wxPoint(18,20);
	wxStaticBox* box_render = new wxStaticBox(this, -1, _T("renderer"), box_pt, box_size);

	stc = new wxStaticText(this, -1, _T("resolution"), 
		_getCtrlPos(0,0)+box_offset, _ctrlSize, static_style);

	wxArrayString res_array;
	res_array.Add(_T("640x480"));
	res_array.Add(_T("800x600"));
	res_array.Add(_T("1024x768"));
	res_array.Add(_T("1440x900"));
	m_cmb_res = new wxComboBox(this, -1, _T("640x480"), 
		_getCtrlPos(0,1)+box_offset, _ctrlSize, res_array );

	//-- windowed
	m_chk_windowed = new wxCheckBox(this, -1, _T("windowed"),
		_getCtrlPos(1,0)+box_offset, _ctrlSize);
	m_chk_windowed->SetValue(true);

	//-- HDR rendering
	m_chk_hdr = new wxCheckBox(this, -1, _T("HDR rendering"),
		_getCtrlPos(1,1)+box_offset, _ctrlSize);
	// m_chk_hdr->SetValue(true);

	//-- buttons
	int btn_y = 16+_getCtrlPos(2,0).y+box_offset.y;
	wxSize btn_size(68, 24);
	wxSize btn_space(20, 0);
	wxRect wnd_rect = GetRect();
	int btn_x = (wnd_rect.width-btn_size.x*2)/2;

	wxButton* btn_ok = new wxButton(this, wxID_OK, _T("OK"),
		wxPoint(btn_x, btn_y),
		btn_size);
	btn_ok->SetDefault();

	wxButton* btn_cancel = new wxButton(this, wxID_CANCEL, _T("Cancel"),
		wxPoint(btn_x+btn_size.x+btn_space.x, btn_y), 
		btn_size);
}

void WxEngineConfigDlg::init_class_combo_list(wxComboBox* cmb, const nexus::nclass* super_class)
{
	for(size_t i=0; i<super_class->get_num_children(); i++)
	{
		const nexus::nclass* sub_class = super_class->get_child(i);
		cmb->AppendString(sub_class->get_name());
		if( i==0 )
			cmb->SetValue(sub_class->get_name());
	}
}

void WxEngineConfigDlg::get_config(nexus::engine_config& out_cfg)
{
	wxString render_sel = m_cmb_renderer->GetValue();
	out_cfg.renderer_class = render_sel.c_str();	

	wxString file_sel = m_cmb_file_sys->GetValue();
	out_cfg.file_system_class = file_sel.c_str();

	out_cfg.engine_data_pkg = _T("../engine_data");

	out_cfg.bWindowed	= m_chk_windowed->GetValue();
	out_cfg.bEnableHDR	= m_chk_hdr->GetValue();
	out_cfg.color_bits	= 32;

	//-- ·Ö±æÂÊ
	wxString res_sel = m_cmb_res->GetValue();
	
	if( res_sel == _T("640x480"))
	{
		out_cfg.width  = 640;
		out_cfg.height= 480;
	}
	else if( res_sel == _T("1024x768"))
	{
		out_cfg.width  = 1024;
		out_cfg.height= 768;
	}
	else if( res_sel == _T("1440x900"))
	{
		out_cfg.width  = 1440;
		out_cfg.height = 900;
	}
	else
	{
		out_cfg.width = 800;
		out_cfg.height = 600;
	}
}