import wx

from EditorEngine import *
import NEG

import wx.propgrid as wxpg

def create_property_grid(parent):
    prop_sizer = wx.BoxSizer(wx.VERTICAL)
    pg = wxpg.PropertyGrid(parent,
                           style=wxpg.PG_SPLITTER_AUTO_CENTER)
    # Show help as tooltips
    pg.SetExtraStyle(wxpg.PG_EX_HELP_AS_TOOLTIPS)        
    prop_sizer.Add(pg,1,wx.EXPAND)
    parent.SetSizer(prop_sizer)
    prop_sizer.SetSizeHints(parent)
    prop_sizer.Layout()
    return pg


class ResourcePreviewWnd:
    def __init__(self, wnd, name):
        self.m_render_wnd = wnd
        self.m_render_mode = ERenderMode.ERM_Wireframe
        
        # basic rendering
        self.m_render_wnd.Bind(wx.EVT_PAINT, self._on_paint)
        self.m_render_wnd.Bind(wx.EVT_SIZE, self._on_resize)
        
        # mouse-keyboard event
        self.m_render_wnd.Bind(wx.EVT_LEFT_DOWN, self._on_left_down)
        self.m_render_wnd.Bind(wx.EVT_LEFT_UP, self._on_left_up)
        self.m_render_wnd.Bind(wx.EVT_RIGHT_DOWN, self._on_right_down)
        self.m_render_wnd.Bind(wx.EVT_RIGHT_UP, self._on_right_up)
        self.m_render_wnd.Bind(wx.EVT_MOTION, self._on_mouse_move)
        self.m_render_wnd.Bind(wx.EVT_MOUSEWHEEL, self._on_mouse_wheel)        
        
        # init camera
        wnd_size = self.m_render_wnd.GetClientSize()
        self.m_camera = ncamera()        
        self.m_camera.set_lookat(vector3(0,0,-500), vector3(0, 0, 0), vector3(0,1,0))
        self.m_camera.set_perspective(3.14159/4, wnd_size.x, wnd_size.y, 5, 50000)    
        
        self.m_camera_ctrl = editor_camera_controller()
        
        # level
        self.m_level_name = name
        engine = NEG.engine
        self.m_level = engine.create_level(name, 'nlevel')
        self.m_level.init('nplain_scene')
    
    def update(self):
        self.m_render_wnd.Refresh(False)
        
    def on_close(self):
        NEG.engine.free_level(self.m_level)
        self.m_level = None
        
    def _on_paint(self, evt):
        dc = wx.PaintDC(self.m_render_wnd)
        self.m_camera_ctrl.update_camera(self.m_camera)
        NEG.engine.set_render_mode(self.m_render_mode)
        NEG.engine.render_level(self.m_level_name, self.m_camera, self.m_render_wnd.GetHandle())        
        
    def _on_resize(self, evt):
        wnd_size = evt.GetSize()
        self.m_camera.set_perspective(3.14159/4, wnd_size.x, wnd_size.y, 5, 50000)
        
    def _on_left_down(self, evt):
        pos = npoint(evt.GetX(), evt.GetY())
        self.m_camera_ctrl.on_mouse_left_down(npoint(pos.x, pos.y))
        self.m_render_wnd.SetFocus()
        self.update()        
        
    def _on_left_up(self, evt):
        self.m_camera_ctrl.on_mouse_left_up()
        self.update()
        
    def _on_right_down(self, evt):
        self.m_camera_ctrl.on_mouse_right_down()
        self.update()        
        
    def _on_right_up(self, evt):
        self.m_camera_ctrl.on_mouse_right_up()
        self.update()        
        
    def _on_mouse_move(self, evt):
        pos = evt.GetPosition()   
        self.m_camera_ctrl.on_mouse_move(npoint(pos.x, pos.y), evt.ControlDown())
        self.update()
            
    def _on_mouse_wheel(self, evt):        
        delta = evt.GetWheelRotation()        
        self.m_camera_ctrl.on_mouse_wheel(delta)
        self.update()
        
        