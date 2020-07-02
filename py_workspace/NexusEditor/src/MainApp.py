import wx
import sys
from wx import xrc

import NEG
from EditorEngine import *
from MainFrame import *
from ResourceMgr import *
from LevelEditor import *

class NexusEditorApp(wx.App):
        
    def OnInit(self):    
        # create main frame window            
        self.m_main_frm = NEMainFrame()
        self.SetTopWindow(self.m_main_frm.get_frame_wnd())
        self._init_engine()
        
        # create resource manager window
        self.m_res_mgr = NEResourceMgr(NEG.engine)
        self.m_res_mgr.create_frame_wnd(self.m_main_frm.get_frame_wnd())
        self.m_res_mgr.init_file_sys(NEG.engine.get_file_sys())
        self.m_main_frm.set_res_mgr(self.m_res_mgr)        
        return True
    
    def _init_engine(self):
        # initialize engine                
        NEG.engine.init_core()
        
        cfg = engine_config() 
        cfg.renderer_class      = "nrenderer_d3d9"        
        cfg.file_system_class   = "nstd_file_system"
        cfg.engine_data_pkg     = "engine_data"
        cfg.bWindowed   = True
        cfg.bEnableHDR  = False
        cfg.color_bits  = 32
        cfg.width       = 1024
        cfg.height      = 768
        py_set_window_handle( cfg, self.m_main_frm.get_view_wnd().get_hwnd() )                
        NEG.engine.init_modules(cfg)
        
        # my init
        NEG.engine.init_editor()
        NEG.level_ed.init()          
        
    # override wx.App
    def OnExit(self): 
        self.m_res_mgr.close()
        self.m_res_mgr = None      
        
        NEG.level_ed.close()
        NEG.level_ed = None
        
        NEG.engine.close()
        NEG.engine = None        
    
if __name__ == '__main__':        
    app = NexusEditorApp(False)
    app.MainLoop()