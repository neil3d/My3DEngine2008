import wx
from wx import xrc

import NEG
from Viewport import *
from EditorEngine import *
from TerrainEditor import *
from LevelEditor import *

class NEMainFrame:
    def __init__(self):
        self.m_main_frm_xrc = xrc.XmlResource('../engine_data/editor_ui/main_frm.xrc')
        self.m_dlg = self.m_main_frm_xrc.LoadFrame(None,'NEMainFrame')
        self.m_view_panel = xrc.XRCCTRL(self.m_dlg, 'NEMainFrmViewPanel')
        self.m_view_frm = NEViewport(self.m_view_panel, self.m_main_frm_xrc)
        self.m_trn_dlg = NETerrainEditorDlg(self.m_dlg, self.m_main_frm_xrc)        
        
        # event bind
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_new_level, id=xrc.XRCID('NEMainFrm_NewLevel'))        
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_exit, id=xrc.XRCID('NEMainFrm_Exit'))        
        
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_show_res_mgr, id=xrc.XRCID('NEMainFrm_ShowResMgr'))
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_show_trn_ed, id=xrc.XRCID('NEMainFrm_ShowTrnEd'))
        
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_new_terrain, id=xrc.XRCID('NEMainFrm_NewTerrain'))        
                       
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_about, id=xrc.XRCID('NEMainFrm_About'))
        
        self.m_dlg.Bind(wx.EVT_RADIOBUTTON, self._on_camera_mode, id=xrc.XRCID('NETool_CameraMode'))
        self.m_dlg.Bind(wx.EVT_RADIOBUTTON, self._on_terrain_mode, id=xrc.XRCID('NETool_TerrainMode'))
        
        # show window
        self.m_dlg.Show()
        
    def set_res_mgr(self, res_mgr):
        self.m_res_mgr = res_mgr
        
    def _on_menu_show_res_mgr(self, evt):
        self.m_res_mgr.show_wnd()
    
    def _on_menu_show_trn_ed(self, evt):
        self.m_trn_dlg.show_dlg()
    
    def get_frame_wnd(self):
        return self.m_dlg
    
    def get_view_wnd(self):
        return self.m_view_frm
    
    def _on_camera_mode(self, evt):
        NEG.level_ed.m_ctrl_mode = NEEditorCtrlMode.EdCtrl_Camera
        
    def _on_terrain_mode(self, evt):
        NEG.level_ed.m_ctrl_mode = NEEditorCtrlMode.EDCtrl_TerrainEd
    
    def _on_menu_new_level(self, evt):
        NEG.level_ed.ui_new_level()
    
    def _on_menu_new_terrain(self, evt):
        dlg = NENewTerrainDlg(self.m_dlg, self.m_main_frm_xrc)
        result = dlg.show_modal()
        if result==wx.ID_OK:
            NEG.level_ed.ui_new_terrain(dlg.m_name, dlg.m_hmap_w, dlg.m_hmap_h, dlg.m_init_h, dlg.m_chunk, dlg.m_scale, dlg.m_pos)
            
    def _on_menu_exit(self, evt):                
        self.m_dlg.Close(True)
    
    def _on_menu_about(self, evt):
        about_info = wx.AboutDialogInfo()
        about_info.AddDeveloper('Neil Fang')
        about_info.SetCopyright('All rights reserved, 2008-2009 GPlusPlus Studio')
        about_info.SetDescription('Nexus Engine Game Content Creation Suite')
        about_info.SetName('Nexus Editor')
        about_info.SetWebSite('http://www.gplusplus.net')
        wx.AboutBox(about_info)
    
class NENewTerrainDlg:
    def __init__(self, parent, xrc_res):
        self.m_dlg = xrc_res.LoadDialog(parent, 'NENewTerrainDlg');
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_cancel, id=xrc.XRCID('NEDlg_Cancel'))
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_ok, id=xrc.XRCID('NEDlg_OK'))
    
    def _on_btn_cancel(self, evt):
        self.m_dlg.EndModal(wx.ID_CANCEL)
    
    def _on_btn_ok(self, evt):
        self._fetch_result()        
        self.m_dlg.EndModal(wx.ID_OK)
        
    def _fetch_result(self):
        self.m_name = xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnName').GetValue()
        self.m_hmap_w = int(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnWidth').GetValue())
        self.m_hmap_h = int(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnHeight').GetValue())
        self.m_init_h = int(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnInitH').GetValue())
        self.m_chunk = int(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnChunkSize').GetValue())
        
        self.m_scale = vector3()
        self.m_scale.x = float(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnScaleX').GetValue())
        self.m_scale.y = float(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnScaleY').GetValue())
        self.m_scale.z = float(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnScaleZ').GetValue())
        
        self.m_pos = vector3()
        self.m_pos.x = float(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnPosX').GetValue())
        self.m_pos.y = float(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnPosY').GetValue())
        self.m_pos.z = float(xrc.XRCCTRL(self.m_dlg, 'NE_NewTrnPosZ').GetValue())
        
        
    def show_modal(self):
        return self.m_dlg.ShowModal()