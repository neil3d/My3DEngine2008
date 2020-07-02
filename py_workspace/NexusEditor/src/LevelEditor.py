import NEG
from NexusEngine import *
from EditorEngine import *
from TerrainEditor import *


class NEEditorCtrlMode:
    EdCtrl_Camera = 0
    EDCtrl_TerrainEd = 1
    
class NELevelEditor:
    def __init__(self):
        self.m_ctrl_mode = NEEditorCtrlMode.EdCtrl_Camera
        self.m_level = None
        self.m_trn_ed = nterrain_editor()
    
    def init(self):
        # create a level by default
        self.ui_new_level()
    
    def close(self):
        self.m_trn_ed.close()
        self.m_level = None
        
    def draw_widgets(self, PDI):
        if self.m_ctrl_mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.draw_widgets(PDI)
        
    def ui_update_trn_brush(self, brush):
        self.m_trn_ed.set_brush(brush)
        
    def ui_new_level(self):
        self.m_level = NEG.engine.create_level('main_ed_lv', 'nlevel')
        self.m_level.init('nplain_scene')        
    
    def ui_new_terrain(self, trn_name, trn_w, trn_h, init_h, chunk_size, scale, pos):
        trn = self.m_level.create_actor(trn_name, 'nterrain_actor')        
        trn.create(trn_w, trn_w, init_h, chunk_size)
        trn.set_scale(scale)
        trn.set_position(pos)
        
        self.m_trn_ed.bind_terrain(trn)
        
    def mouse_left_down(self, pos, ctrl_down, view):
        mode = self.m_ctrl_mode;
        if ctrl_down:
            mode = NEEditorCtrlMode.EdCtrl_Camera
        
        if mode == NEEditorCtrlMode.EdCtrl_Camera:
            view.m_camera_ctrl.on_mouse_left_down(pos)
        elif mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.on_mouse_left_down(pos)
            
        view.update()
            
    def mouse_left_up(self, ctrl_down, view):
        mode = self.m_ctrl_mode;
        if ctrl_down:
            mode = NEEditorCtrlMode.EdCtrl_Camera
            
        if mode == NEEditorCtrlMode.EdCtrl_Camera:
            view.m_camera_ctrl.on_mouse_left_up()
        elif mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.on_mouse_left_up()
            
        view.update()
            
    def mouse_right_down(self, pos, ctrl_down, view):
        mode = self.m_ctrl_mode;
        if ctrl_down:
            mode = NEEditorCtrlMode.EdCtrl_Camera
            
        if mode == NEEditorCtrlMode.EdCtrl_Camera:
            view.m_camera_ctrl.on_mouse_right_down()
        elif mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.on_mouse_right_down()
            
        view.update()
            
    def mouse_right_up(self, ctrl_down, view):
        mode = self.m_ctrl_mode;
        if ctrl_down:
            mode = NEEditorCtrlMode.EdCtrl_Camera
            
        if mode == NEEditorCtrlMode.EdCtrl_Camera:
            view.m_camera_ctrl.on_mouse_right_up()
        elif mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.on_mouse_right_up()
            
        view.update()
            
    def mouse_wheel(self, delta, ctrl_down, view):
        mode = self.m_ctrl_mode;
        if ctrl_down:
            mode = NEEditorCtrlMode.EdCtrl_Camera
            
        if mode == NEEditorCtrlMode.EdCtrl_Camera:
            view.m_camera_ctrl.on_mouse_wheel(delta)
        elif mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.on_mouse_wheel(delta)
            
        view.update()
            
    def mouse_move(self, pos, ctrl_down, view):
        mode = self.m_ctrl_mode;
        if ctrl_down:
            mode = NEEditorCtrlMode.EdCtrl_Camera
            
        if mode == NEEditorCtrlMode.EdCtrl_Camera:
            view.m_camera_ctrl.on_mouse_move(pos, ctrl_down)
        elif mode == NEEditorCtrlMode.EDCtrl_TerrainEd:
            self.m_trn_ed.on_mouse_move(pos, ctrl_down, view.m_camera)
            
        view.update()
            
            