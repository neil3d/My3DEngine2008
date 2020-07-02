import wx
from wx import xrc

import NEG
from EditorEngine import *

class NEViewport:
    def __init__(self, wnd, xrc_res):
        self.m_render_wnd = wnd   
        self.m_render_mode = ERenderMode.ERM_Wireframe
        
        # add tool bar
        self.m_tool_bar = xrc_res.LoadToolBar(self.m_render_wnd, 'NEViewToolBar')
        self.m_tool_bar.Bind(wx.EVT_TOOL, self._on_render_wireframe, id=xrc.XRCID('NEView_RenderWireframe'))
        self.m_tool_bar.Bind(wx.EVT_TOOL, self._on_render_unlit, id=xrc.XRCID('NEView_RenderUnlit'))
        
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
        
        
        # initial camera        
        wnd_size = self.m_render_wnd.GetClientSize()
        self.m_camera = ncamera()        
        self.m_camera.set_lookat(vector3(0,0,-500), vector3(0, 0, 0), vector3(0,1,0))
        self.m_camera.set_perspective(3.14159/4, wnd_size.x, wnd_size.y, 5, 50000)    
        
        self.m_camera_ctrl = editor_camera_controller()
        
        # reg
        NEG.engine.register_view(self)
        
    def update(self):
        self.m_render_wnd.Refresh(False)
    def get_hwnd(self):
        return self.m_render_wnd.GetHandle()                
        
    def _on_render_wireframe(self, evt):
        self.m_render_mode = ERenderMode.ERM_Wireframe
        self.update()
        
    def _on_render_unlit(self, evt):
        self.m_render_mode = ERenderMode.ERM_Unlit
        self.update()
        
    def _on_left_down(self, evt):
        pos = npoint(evt.GetX(), evt.GetY())
        NEG.level_ed.mouse_left_down(pos, evt.ControlDown(), self)
        self.m_render_wnd.SetFocus()
        #self.CaptureMouse()        
        
    def _on_left_up(self, evt):
        NEG.level_ed.mouse_left_up(evt.ControlDown(), self)
        #self.ReleaseMouse()        
        
    def _on_right_down(self, evt):
        pos = npoint(evt.GetX(), evt.GetY())
        NEG.level_ed.mouse_right_down(pos, evt.ControlDown(), self)        
        
    def _on_right_up(self, evt):
        NEG.level_ed.mouse_right_up(evt.ControlDown(), self)        
        
    def _on_mouse_move(self, evt):
        pos = evt.GetPosition()      
        NEG.level_ed.mouse_move(npoint(pos.x, pos.y), evt.ControlDown(), self)
            
    def _on_mouse_wheel(self, evt):        
        delta = evt.GetWheelRotation()        
        NEG.level_ed.mouse_wheel(delta, evt.ControlDown(), self)
        
        
    def _on_paint(self, evt):
        dc = wx.PaintDC(self.m_render_wnd)        
        self.m_camera_ctrl.update_camera(self.m_camera)
        NEG.engine.update_all_view()
    
    def _on_resize(self, evt):
        wnd_size = evt.GetSize()
        self.m_camera.set_perspective(3.14159/4, wnd_size.x, wnd_size.y, 5, 50000)        
        