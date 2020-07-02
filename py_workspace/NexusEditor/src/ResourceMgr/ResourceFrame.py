# -*- coding: UTF-8 -*-
import wx
from wx import xrc

from NexusEngine import *
from ResLocationDlg import *
from StaticMeshEditorFrm import *
from MaterialEditor import *

tree_root_txt = 'File Root'  

# 递归的列出package中的所有子目录
class NEPackageWalker(nfile_query):
    def __init__(self, file_sys, tree_ctrl, parent_item, pkg_name):
        nfile_query.__init__(self)
        self.m_file_sys = file_sys
        self.m_tree_ctrl = tree_ctrl
        self.m_parent_item = parent_item
        self.m_pkg_name = pkg_name
                
        
    def on_file_entity(self, file):
        if file.type == EFileEntityType.EFile_Folder:            
            tree_item = self.m_tree_ctrl.AppendItem(self.m_parent_item, file.name)
            self.m_tree_ctrl.SetItemImage(tree_item, 1, wx.TreeItemIcon_Normal)
            self.m_tree_ctrl.SetItemImage(tree_item, 2, wx.TreeItemIcon_Expanded)
            self.m_tree_ctrl.SetItemPyData(tree_item, (self.m_pkg_name, file) )
            
            # walk through sub folders 
            pkg_walker = NEPackageWalker(self.m_file_sys, self.m_tree_ctrl, tree_item, self.m_pkg_name)
            self.m_file_sys.query_package(self.m_pkg_name, file.path, pkg_walker)
            
# 列出Package中指定目录中的文件
class NEPackageFileList(nfile_query):
    def __init__(self, list_ctrl):
        nfile_query.__init__(self)
        self.m_list_ctrl = list_ctrl
        
    def on_file_entity(self, file):
        if file.type == EFileEntityType.EFile_Regular:
            if file.name != '__nexus_pkg__':
                self.m_list_ctrl.Append([file.name])

class NEResourceFrame:
    def __init__(self, parent, xrc_res):
        self.m_frame = xrc_res.LoadFrame(parent,'NEResourceMgrFrm')
        self.m_frame.Bind(wx.EVT_CLOSE, self._on_close)
        self.m_filter = NEResourceFilter(xrc.XRCCTRL(self.m_frame, 'NERes_TypeCheckList'))
        self.m_list = NEResourceList(xrc.XRCCTRL(self.m_frame, 'NERes_ListCtrl'))
        
        # init tree ctrl
        self.m_tree_ctrl = xrc.XRCCTRL(self.m_frame, 'NERes_PkgTree')
        isz = (16,16)
        il = wx.ImageList(isz[0], isz[1])
        il.Add(wx.ArtProvider_GetBitmap(wx.ART_FLOPPY, wx.ART_OTHER, isz))
        il.Add(wx.ArtProvider_GetBitmap(wx.ART_FOLDER, wx.ART_OTHER, isz))
        il.Add(wx.ArtProvider_GetBitmap(wx.ART_FOLDER_OPEN, wx.ART_OTHER, isz))
        #self.m_tree_ctrl.SetImageList(il)
        
        self.m_tree_ctrl.Bind(wx.EVT_TREE_SEL_CHANGED, self._on_tree_sel_changed)
        
        # create sub frames
        self.m_static_mesh_frm = StaticMeshEditorFrm(self.m_frame, xrc_res)
        
        self.m_frame.Bind(wx.EVT_MENU, self._on_menu_show_static_mesh_editor, id=xrc.XRCID('NERes_ShowStaticMeshEditor'))
                        
                
        #--
        self.m_frame.SetSize(wx.Size(800,600))
        self.m_frame.Show(True)      
        
    def _on_close(self, evt):
        self.m_frame.Show(False)
    
    def _on_menu_show_static_mesh_editor(self, evt):
        self.m_static_mesh_frm.m_dlg.Show(True)
        
    def _on_tree_sel_changed(self, evt):
        item = evt.GetItem()
        item_data = self.m_tree_ctrl.GetItemPyData(item)
        if item_data:
            pkg_name = item_data[0]
            file = item_data[1]
            self.m_list.show_folder(pkg_name,file.path)
    
    def show(self):        
        self.m_frame.Show(True)
        
    def build_package_tree(self, file_sys):
        tree_ctrl = self.m_tree_ctrl
        root = tree_ctrl.AddRoot(tree_root_txt)
        
        #-- file entity for package root folder
        root_file = nfile_entity()
        root_file.type = EFileEntityType.EFile_Folder;
        root_file.path = '/'
        
        self.m_tree_ctrl.SetItemImage(root, 1, wx.TreeItemIcon_Normal)
        self.m_tree_ctrl.SetItemImage(root, 2, wx.TreeItemIcon_Expanded)
        for i in range(0, file_sys.get_num_package()):
            pkg_name = file_sys.get_package_name(i)
            tree_item = tree_ctrl.AppendItem(root, pkg_name)
            tree_ctrl.SetItemPyData(tree_item, (pkg_name, root_file) )
            tree_ctrl.SetItemImage(tree_item, 1, wx.TreeItemIcon_Normal)
            tree_ctrl.SetItemImage(tree_item, 2, wx.TreeItemIcon_Expanded)
            pkg_walker = NEPackageWalker(file_sys, tree_ctrl, tree_item, pkg_name)
            file_sys.query_package(pkg_name, '/', pkg_walker)
        self.m_tree_ctrl.Expand(root)                
        
class NEResourceFilter:
    def __init__(self, check_list_ctrl):
        self.m_check_list_ctrl = check_list_ctrl;
        type_list =['static mesh', 'texture', 'skeletal mesh', 'anim set', 'morph anim']
        self.m_check_list_ctrl.SetItems(type_list)
        
class NEResourceList:
    def __init__(self, list_ctrl):
        self.m_ctrl = list_ctrl
        
    def show_folder(self, pkg_name, path):
        ctrl = self.m_ctrl
        #-- remove old
        ctrl.DeleteAllItems()
        
        #-- build new list content
        file_sys = NEG.engine.get_file_sys()
        list_query = NEPackageFileList(self.m_ctrl)
        file_sys.query_package(pkg_name, path, list_query) 
    
    
    
    