from NexusEngine import *
from LevelEditor import *
from EditorWidgetsRenderer import *

class EditorEngine(neditor_engine):
    def __init__(self):
        neditor_engine.__init__(self)
        self.m_view_list = []                
    
    def init_editor(self):
        self.m_widgets_render = NEWidgetsRenderer()        
        self.render_attach_element(self.m_widgets_render)
        
        # set default render mode
        self.set_render_mode(ERenderMode.ERM_Wireframe)        
        
    # override nengine.close()
    def close(self):
        self.render_detach_element(self.m_widgets_render)
        self.m_widgets_render = None      
        
        neditor_engine.close(self)
        
    def register_view(self, view_wnd):
        self.m_view_list.append(view_wnd)
        
    def update_all_view(self):
        self.update_objects(1.0/50)
        for view in self.m_view_list:
            self.set_render_mode(view.m_render_mode)
            self.render_level('main_ed_lv', view.m_camera, view.get_hwnd())
    