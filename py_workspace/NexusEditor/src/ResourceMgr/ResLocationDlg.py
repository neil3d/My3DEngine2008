
import wx
from wx import xrc
from NexusEngine import resource_location

class ResLocationDlg:
    def __init__(self, title, parent, xrc_res):
        self.m_dlg = xrc_res.LoadDialog(parent, 'NEResourceLocationDlg');
        self.m_dlg.SetTitle(title)
        self.m_loc = resource_location()
        
        # get controls
        self.m_package_ctrl = xrc.XRCCTRL(self.m_dlg, 'NEResLoc_Package')
        self.m_path_ctrl = xrc.XRCCTRL(self.m_dlg, 'NEResLoc_Path')
        
        # event bind
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_cancel, id=xrc.XRCID('NEResLoc_Cancel'))
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_ok, id=xrc.XRCID('NEResLoc_OK'))
    
    def _on_btn_cancel(self, evt):
        self.m_dlg.EndModal(wx.ID_CANCEL)
    
    def _on_btn_ok(self, evt):
        self._fetch_result()        
        self.m_dlg.EndModal(wx.ID_OK)
        
    def _fetch_result(self):
        self.m_loc.pkg_name = self.m_package_ctrl.GetValue()
        self.m_loc.file_name = self.m_path_ctrl.GetValue()
    
    def do_modal(self):
        return self.m_dlg.ShowModal() 
    
    def get_result(self):
        return self.m_loc