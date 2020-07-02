import wx
from wx import xrc

from ResourcePreviewWnd import *
from ResLocationDlg import *
from WxExtend import *
import NEG

class MaterialPreviewWnd(ResourcePreviewWnd):    
    def create_perview(self, mtl):
        self.m_level.clear_actor()
        
        #-- create a shape actor for preview
        new_actor = self.m_level.create_actor('material_perview','nactor')
        comp = new_actor.create_component('my_comp', 'nshape_component')
        comp.create_box(100, 100, 100)
        comp.set_material(mtl)
        
        #-- create a point light
        lgt_actor = self.m_level.create_actor('material_perview_point_lgt','nactor')
        lgt_comp = lgt_actor.create_component('lgt_comp', 'nlight_component')
        lgt_comp.create_point_light()
        lgt = lgt_comp.get_point_light()
        lgt.m_ambient_color = color4f(0.3, 0.3, 0.3, 1)
        lgt.m_diffuse_color = color4f(0.7, 0.7, 0.7, 1)
        lgt.m_position = vector3(0, 150, 0)
        lgt.m_attenuation = vector3(0.5, 0.0001, 0.9);
        
        #-- refresh
        self.m_render_mode = ERenderMode.ERM_Lit
        self.update()        

       
class MaterialEditorDlg:
    def __init__(self, parent_wnd, xrc_res):
        self.m_xrc_res = xrc_res
        self.m_dlg = xrc_res.LoadDialog(parent_wnd,'NERes_MaterialEditorDlg')
                
        # create preview
        self.m_preview = MaterialPreviewWnd(xrc.XRCCTRL(self.m_dlg, 'NERes_MaterialPreview'),
                                            'MaterialEditorPreview')
        self.m_btn_create = xrc.XRCCTRL(self.m_dlg, 'NERes_MtlCreate')
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_create, id=xrc.XRCID('NERes_MtlCreate'))
        
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_cancel, id=xrc.XRCID('NEDlg_Cancel'))
        self.m_dlg.Bind(wx.EVT_BUTTON, self._on_btn_ok, id=xrc.XRCID('NEDlg_OK'))
        
        # create property grid
        prop_panel = xrc.XRCCTRL(self.m_dlg, 'NERes_MtlPropPanel')
        self.m_prop_grid = create_property_grid(prop_panel)
        self.m_prop_grid.Bind( wxpg.EVT_PG_CHANGED, self._on_prop_changed )
        
        self.m_mtl = None
        
        # init controls
        self.build_property_grid()
        
        self.m_dlg.Fit()
        self.m_dlg.Layout()
    
    def get_result(self):
        return self.m_mtl
     
    def _on_btn_cancel(self, evt):
        self.m_dlg.EndModal(wx.ID_CANCEL)
    
    def _on_btn_ok(self, evt):                
        self.m_dlg.EndModal(wx.ID_OK)
        
    def _on_btn_create(self, evt):
        pg = self.m_prop_grid
        p_name = pg.GetProperty('Name').GetValue()
        p_loc = pg.GetProperty('Template Loc').GetValue()
        if len(p_name) <= 0:
            wx.MessageBox('Please input material name', 'Input error')
            return
        
        if not p_loc.is_valid():
            wx.MessageBox('Please input material template location', 'Input error')
            return
        self.new_material(p_name, p_loc)        
            
    def _on_prop_changed(self, evt):
        p = evt.GetProperty()
        param = p.GetClientData()
        if param and isinstance(param, material_parameter) and self.m_mtl:
            mtl = self.m_mtl
            t = param.type
            if t == EMaterialParameter.EMPT_Float:
                v = p.GetValue()
                val = vector4(v, 0, 0, 0)
                mtl.set_vector_parameter(param.name, val)
            elif t == EMaterialParameter.EMPT_Float2:
                v = p.GetValue()
                val = vector4(v.x, v.y, 0, 0)
                mtl.set_vector_parameter(param.name, val)                
            elif t == EMaterialParameter.EMPT_Float3:
                v = p.GetValue()
                val = vector4(v.x, v.y, v.z, 0)
                mtl.set_vector_parameter(param.name, val)
            elif t == EMaterialParameter.EMPT_Float4:
                v = p.GetValue()                
                mtl.set_vector_parameter(param.name, v)
            elif t == EMaterialParameter.EMPT_Texture:
                loc = p.GetValue()
                if loc.is_valid():
                    mtl.set_texture_parameter(param.name, loc)
            self.m_preview.update()            
            
    def new_material(self, mtl_name, temp_loc):
        # create mtl
        mtl_temp = nresource_manager.instance().load_material_template_script(temp_loc,
                                                                              EResourceIOMode.EIO_Block,
                                                                              EResourceIOPriority.EIOP_Normal)
        mtl = nmaterial(mtl_name)
        mtl.create(mtl_temp)
        self.m_mtl = mtl
        self.build_property_grid()
        self.m_preview.create_perview(mtl)        
    
    def build_property_grid(self):
        pg = self.m_prop_grid
        pg.Clear()
        if self.m_mtl:
            mtl = self.m_mtl
            pg.Append( wxpg.PropertyCategory("Creation") )
            pg.Append( wxpg.StringProperty("Name", value=mtl.get_name()) )
            pg.Append( WxResLocProperty("Template Loc",value=mtl.get_template_loc()) )            
            pg.Append( wxpg.PropertyCategory("Parameters") )
            for i in range(0, mtl.get_num_parameter()):
                param = mtl.get_parameter(i)
                t = param.type
                if t == EMaterialParameter.EMPT_Float:
                    pg.Append( wxpg.FloatProperty(param.name, value=0) )
                elif t == EMaterialParameter.EMPT_Float2:
                    pg.Append( WxVector2Property(param.name) )
                elif t == EMaterialParameter.EMPT_Float3:
                    pg.Append( WxVector3Property(param.name) )
                elif t == EMaterialParameter.EMPT_Float4:
                    pg.Append( WxVector4Property(param.name) )
                elif t == EMaterialParameter.EMPT_Texture:
                    pg.Append( WxResLocProperty(param.name, value=resource_location()) )
                pg.SetPropertyClientData(param.name, param)
        else:
            pg.Append( wxpg.PropertyCategory("Creation") )
            pg.Append( wxpg.StringProperty("Name") )
            pg.Append( WxResLocProperty("Template Loc") )                        
        
    def show_modal(self):
        return self.m_dlg.ShowModal()
    def destroy(self):
        self.m_dlg.Destroy()
        self.m_preview.on_close()
    
    