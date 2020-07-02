
from ResourceFrame import *

instance = None
PREVIEW_LEVEL_NAME = 'resource_preview_ed_lv'

class NEResourceMgr:
    def __init__(self, engine):
        global instance
        instance = self
        
    def create_frame_wnd(self, parent):
        self.m_xrc_res = xrc.XmlResource('../engine_data/editor_ui/res_mgr_frm.xrc')
        self.m_main_frame = NEResourceFrame(parent, self.m_xrc_res)        
    
    def init_file_sys(self, file_sys):
        self.m_file_sys = file_sys        
        self.m_main_frame.build_package_tree(file_sys)
    
    def show_wnd(self):
        self.m_main_frame.show()
    
    def close(self):
        self.m_level = None
        self.m_engine = None
        del self.m_main_frame