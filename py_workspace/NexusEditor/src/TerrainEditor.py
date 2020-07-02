import wx
from wx import xrc

import NEG
from NexusEngine import *

class NETerrainEditorDlg:
    def __init__(self, parent, xrc_res):        
        self.m_dlg = xrc_res.LoadDialog(parent, 'NETerrainEditorDlg')
        
        # get control
        #--- brush page
        self.m_op_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_OP')
        self.m_shape_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_Shape')
        self.m_lerp_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_Lerp')
        self.m_inner_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_InnerR')
        self.m_outer_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_OuterR')
        self.m_strength_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_Strength')
        self.m_brush_freq_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_Freq')
        self.m_target_hmap = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_THmap')
        self.m_target_alpha = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_TAlpha')
        self.m_target_deco = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_TDeco')
        self.m_target_index = xrc.XRCCTRL(self.m_dlg, 'NETrnBrush_TIndex')
        
        tmp_list=['layer0', 'layer1', 'layer2', 'layer3', 'layer4']
        self.m_target_index.AppendItems(tmp_list)
        self.m_target_index.Select(0)
        
        #--- noise page
        self.m_noise_left_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoiseLeft')
        self.m_noise_top_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoiseTop')
        self.m_noise_right_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoiseRight')
        self.m_noise_bottom_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoiseBottom')
        self.m_noise_full_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoiseFull')
        self.m_noise_oct_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NumOoctaves')
        self.m_noise_amp_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_Amplitude')
        self.m_noise_freq_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_Frequency')
        self.m_noise_target_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoiseTarget')        
        
        #--- material page
        self.m_basic_mtl_tex_loc = xrc.XRCCTRL(self.m_dlg, 'NETrn_BasicMtlTexLoc')
        
        self.m_splat_alpha_w = xrc.XRCCTRL(self.m_dlg, 'NETrn_SplatAlphaW')
        self.m_splat_alpha_h = xrc.XRCCTRL(self.m_dlg, 'NETrn_SplatAlphaH')
        self.m_mtl_layer_index = xrc.XRCCTRL(self.m_dlg, 'NETrn_MtlLayerIndex')
        self.m_mtl_layer_tex_loc = xrc.XRCCTRL(self.m_dlg, 'NETrn_MtlLayerTexLoc')
        
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_set_splat_layer_tex, id=xrc.XRCID('NETrn_MtlSetLayerTex'))
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_create_splat_mtl, id=xrc.XRCID('NETrn_CreateSplatMtl')) 
        
        #--- tools page
        self.m_hmap_file_ctrl = xrc.XRCCTRL(self.m_dlg, 'NETrn_HMapFile')
        
        
        # map events
        # wx.choice event
        self.m_dlg.Bind(wx.EVT_CHOICE, self._on_brush_settings_changed)
        # wx.slider event
        self.m_dlg.Bind(wx.EVT_SCROLL_CHANGED, self._on_brush_settings_changed)
        self.m_inner_ctrl.Bind(wx.EVT_SCROLL, self._on_inner_chang)
        self.m_outer_ctrl.Bind(wx.EVT_SCROLL, self._on_outer_chang)
        
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_gen_noise, id=xrc.XRCID('NETrn_GenNoise'))
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_create_basic_mtl, id=xrc.XRCID('NETrn_CreateBasicMtl'))
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_import_hmap, id=xrc.XRCID('NETrn_ImportHMap'))
        
        # brush target chage
        self.m_dlg.Bind(wx.EVT_RADIOBUTTON, self._on_brush_settings_changed, id=xrc.XRCID('NETrnBrush_THmap'))
        self.m_dlg.Bind(wx.EVT_RADIOBUTTON, self._on_brush_settings_changed, id=xrc.XRCID('NETrnBrush_TAlpha'))
        self.m_dlg.Bind(wx.EVT_RADIOBUTTON, self._on_brush_settings_changed, id=xrc.XRCID('NETrnBrush_TDeco'))
        self.m_dlg.Bind(wx.EVT_LISTBOX, self._on_brush_settings_changed, id=xrc.XRCID('NETrnBrush_TIndex'))
        
        note_book = xrc.XRCCTRL(self.m_dlg, 'NETrn_NoteBook')
        note_book.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self._on_page_changed)
        
    
    def show_dlg(self):
        self.m_dlg.Show()
        
    def _on_inner_chang(self, evt):
        inner_radius = self.m_inner_ctrl.GetValue()
        outer_radius = self.m_outer_ctrl.GetValue()
        if outer_radius < inner_radius:
            self.m_outer_ctrl.SetValue(inner_radius)
        
    def _on_outer_chang(self, evt):
        inner_radius = self.m_inner_ctrl.GetValue()
        outer_radius = self.m_outer_ctrl.GetValue()
        if inner_radius > outer_radius:
            self.m_inner_ctrl.SetValue(outer_radius)
            
    def _on_brush_settings_changed(self, evt):
        new_brush = nterrain_brush()
        self._fetch_brush(new_brush)        
        NEG.level_ed.ui_update_trn_brush(new_brush)
        
    def _on_gen_noise(self, evt):
        rc = nrect()
        full = self.m_noise_full_ctrl.IsChecked()
        if full:
            rc.left = 0
            rc.top = 0
            rc.right = -1
            rc.bottom = -1
        else:
            rc.left = int(self.m_noise_left_ctrl.GetValue())
            rc.top = int(self.m_noise_top_ctrl.GetValue())
            rc.right = int(self.m_noise_right_ctrl.GetValue())
            rc.bottom = int(self.m_noise_bottom_ctrl.GetValue())
        
        num_oct = int(self.m_noise_oct_ctrl.GetValue())
        amp = int(self.m_noise_amp_ctrl.GetValue())
        freq = int(self.m_noise_freq_ctrl.GetValue())
        
        new_brush = nterrain_brush()
        self._fetch_brush(new_brush)
        if new_brush.target == ETrnBurshTarget.EBT_Heightmap:
            NEG.level_ed.m_trn_ed.generate_noise(rc, num_oct, amp, freq)
        elif new_brush.target == ETrnBurshTarget.EBT_Alphamap:
            NEG.level_ed.m_trn_ed.splat_layer_noise(new_brush.target_index, rc, num_oct, amp, freq)
        
    def _on_create_basic_mtl(self, evt):
        tex_loc = self.m_basic_mtl_tex_loc.GetValue()
        if len(tex_loc) <= 0:
            wx.MessageBox('Please input texture location', 'Input error')
            return
        NEG.level_ed.m_trn_ed.create_material_basic(resource_location(tex_loc))        
    
    def _on_import_hmap(self, evt):
        hmap_path = self.m_hmap_file_ctrl.GetPath()
        if len(hmap_path) <= 0:
            wx.MessageBox('Please select a image file to import.', 'Input error')
            return
        
        NEG.level_ed.m_trn_ed.import_heightmap(hmap_path)
        
    def _fetch_brush(self, brush):
        brush.op = ETrnBrushOP(self.m_op_ctrl.GetCurrentSelection())
        brush.shape = ETrnBrushShape(self.m_shape_ctrl.GetCurrentSelection())
        brush.lerp_mode = ETrnLerpMode(self.m_lerp_ctrl.GetCurrentSelection())
        brush.inner_radius = self.m_inner_ctrl.GetValue()
        brush.outer_radius = self.m_outer_ctrl.GetValue()
        brush.strength = self.m_strength_ctrl.GetValue()
        brush.noise_freq = int(self.m_brush_freq_ctrl.GetValue())
        
        if self.m_target_hmap.GetValue():
            brush.target = ETrnBurshTarget.EBT_Heightmap
        elif self.m_target_alpha.GetValue():
            brush.target = ETrnBurshTarget.EBT_Alphamap
        elif self.m_target_deco.GetValue():
            brush.target = ETrnBurshTarget.EBT_Decomap
            
        brush.target_index = self.m_target_index.GetSelection()
        
    def _on_set_splat_layer_tex(self, evt):
        layer_index = self.m_mtl_layer_index.GetValue()
        
        layer_tex_loc = self.m_mtl_layer_tex_loc.GetValue()
        if len(layer_tex_loc) <= 0:
            wx.MessageBox('Please input splatting layer texture location', 'Input error')
            return
        
        NEG.level_ed.m_trn_ed.splat_set_texture(layer_index, resource_location(layer_tex_loc))
        
    def _on_create_splat_mtl(self, evt):
        alpha_w = int(self.m_splat_alpha_w.GetValue())
        alpha_h = int(self.m_splat_alpha_h.GetValue())
        
        NEG.level_ed.m_trn_ed.create_texture_splatting(alpha_w, alpha_h)
        
    def _on_page_changed(self, evt):
        new_pg = evt.GetSelection()
        if new_pg == 1:
            new_brush = nterrain_brush()
            self._fetch_brush(new_brush)
            text = "generate for %s[%d]"%(str(new_brush.target),new_brush.target_index)
            self.m_noise_target_ctrl.SetLabel(text)
        evt.Skip()
            