#pragma once

class WxEngineConfigDlg
	: public wxDialog
{
public:
	WxEngineConfigDlg(wxWindow *parent);
	virtual ~WxEngineConfigDlg(void);

	void get_config(nexus::engine_config& out_cfg);
private:
	void create_controls();
	void init_class_combo_list(wxComboBox* cmb, const nexus::nclass* super_class);

	wxComboBox*	m_cmb_renderer;
	wxComboBox*	m_cmb_file_sys;
	wxComboBox*	m_cmb_res;
	wxCheckBox*	m_chk_windowed;
	wxCheckBox*	m_chk_hdr;
};
