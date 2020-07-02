
import wx
from wx import xrc
import ResourceMain
from NexusEngine import *
from ResourcePreviewWnd import *
from ResLocationDlg import *
from WxExtend import * 
from MaterialEditor import * 

class WxMaterialProperty(wxpg.PyLongStringProperty):
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=None):
        self.m_mtl = value
        wxpg.PyLongStringProperty.__init__(self, label, name)
        self.SetValue(self.m_mtl)
        
    def GetValueAsString(self, flags):
        if self.m_mtl==None:
            return 'None'
        else:
            return self.m_mtl.get_name()
        
    def OnButtonClick(self, pg, value):
        dlg = MaterialEditorDlg(ResourceMain.instance.m_main_frame.m_frame,
                                ResourceMain.instance.m_xrc_res)
        ret = dlg.show_modal()
        if ret==wx.ID_OK:
            mtl = dlg.get_result()
            self.m_mtl = mtl
            self.SetValue(self.m_mtl)
            pg = self.GetGrid()
            pg.ChangePropertyValue(self.GetId(), mtl)
        dlg.destroy()            
        return True


class StaticMeshPreviewWnd(ResourcePreviewWnd):    
    def create_preview(self, mesh_res):
        self.m_level.clear_actor()
        new_actor = self.m_level.create_actor('static_mesh_preview','nactor')
        comp = new_actor.create_component('my_comp', 'nstatic_mesh_component')
        comp.reset_resource( mesh_res )
        
        self.m_render_mode = ERenderMode.ERM_Unlit
        self.update()


class StaticMeshEditorFrm:
    def __init__(self, parent_wnd, xrc_res):
        self.m_mesh_res = None
        self.m_dlg = xrc_res.LoadFrame(parent_wnd,'NERes_StaticMeshEditorFrm')
        self.m_preview = StaticMeshPreviewWnd(xrc.XRCCTRL(self.m_dlg, 'NERes_StaticMeshPreview'), 
                                              'StaticEditorPreview')
        
        # create property grid
        prop_panel = xrc.XRCCTRL(self.m_dlg, 'NERes_StaticMeshPropPanel')
        self.m_prop_grid = create_property_grid(prop_panel)
        self.build_property_grid()
        
        self.m_dlg.Bind(wx.EVT_CLOSE, self._on_close)
        
        # menu event bind
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_import, id=xrc.XRCID('NEResMenu_ImportStaticMesh'))
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_save, id=xrc.XRCID('NEResMenu_SaveStaticMesh'))
        self.m_dlg.Bind(wx.EVT_MENU, self._on_menu_load, id=xrc.XRCID('NEResMenu_LoadStaticMesh'))
        
        self.m_prop_grid.Bind( wxpg.EVT_PG_CHANGED, self._on_prop_changed )
        
        self.m_dlg.Show(False)
        
    def _on_close(self, evt):
        self.m_dlg.Show(False)
        
    def _on_menu_import(self, evt):
        num_importer = nresource_static_mesh.get_num_importer()
        file_filter = ''
        for i in range(0, num_importer):
            if i > 0:
                file_filter += '|'
            file_filter += '%s(*%s)|*%s'%(nresource_static_mesh.get_importer_desc(i),
                                       nresource_static_mesh.get_importer_type(i),
                                       nresource_static_mesh.get_importer_type(i)
                                       )
        dlg = wx.FileDialog(self.m_dlg,  wildcard=file_filter, style=wx.FD_OPEN)
        result = dlg.ShowModal()
        if result==wx.ID_OK:
            file_path = dlg.GetPath()
            new_mesh = nresource_manager.instance().new_static_mesh(file_path)
            new_mesh.import_lod(-1, file_path)
            self.m_preview.create_preview(new_mesh)
            self.m_mesh_res = new_mesh
            self.build_property_grid()            
            
    def _on_menu_save(self, evt):
        if self.m_mesh_res :
            dlg = ResLocationDlg('Save static mesh ...', self.m_dlg, ResourceMain.instance.m_xrc_res)
            if dlg.do_modal() == wx.ID_OK:
                loc = dlg.get_result()
                self.m_mesh_res.save_to_file(loc)
                
    def _on_menu_load(self, evt):
        dlg = ResLocationDlg('Load static mesh ...', self.m_dlg, ResourceMain.instance.m_xrc_res)
        if dlg.do_modal() == wx.ID_OK:
            loc = dlg.get_result()
            new_mesh = nresource_manager.instance().load_static_mesh(loc,
                                                                     EResourceIOMode.EIO_Block,
                                                                     EResourceIOPriority.EIOP_Normal)
            self.m_preview.create_preview(new_mesh)
            self.m_mesh_res = new_mesh
            self.build_property_grid()
    
    def build_property_grid(self):
        pg = self.m_prop_grid
        pg.Clear()
        
        if self.m_mesh_res:
            mesh = self.m_mesh_res
            pg.Append( wxpg.PropertyCategory("Resource") )
            pg.Append( wxpg.StringProperty("Name",value=mesh.get_name_str()) )
            pg.Append( WxResLocProperty("Loc", value=mesh.get_location()) )
            for lod in range(0, mesh.get_num_lod()):
                pg.Append( wxpg.PropertyCategory("Lod[%d]"%lod) )
                for sec in range(0, mesh.get_num_section(lod)):
                    mtl = mesh.get_material(lod, sec)
                    if mtl:
                        id = pg.Append( WxMaterialProperty("Material[%d]"%sec, value=mtl) )
                    else:
                        id = pg.Append( WxMaterialProperty("Material[%d]"%sec) )
                    pg.SetPropertyClientData(id, (lod,sec))                
        else:
            pg.Append( wxpg.PropertyCategory("Resource") )
            pg.Append( wxpg.StringProperty("Name",value="None") )            
            pg.Append( wxpg.PropertyCategory("LOD[0]") )
            pg.Append( wxpg.StringProperty("None",value="None") )
        pg.Refresh(True)
        
    def _on_prop_changed(self, evt):
        p = evt.GetProperty()
        name = p.GetName()
        # material changed
        if self.m_mesh_res and name.startswith('Material'):
            mtl_pos = p.GetClientData()
            mtl = p.GetValue()            
            self.m_mesh_res.import_set_material(mtl_pos[0], mtl_pos[1], mtl)
            
            
