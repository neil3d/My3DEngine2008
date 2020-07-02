
import wx
import wx.propgrid as wxpg
from NexusEngine import vector2,vector3,vector4,resource_location

class WxVector2Property(wxpg.PyProperty):
    'Property for NexusEngine.vector2'
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=vector2()):
        wxpg.PyProperty.__init__(self, label, name)
        self.SetValue(value)
        
        self.AddChild(wxpg.PyFloatProperty('x', wxpg.LABEL_AS_NAME, value.x))
        self.AddChild(wxpg.PyFloatProperty('y', wxpg.LABEL_AS_NAME, value.y))
        
    def GetValueAsString(self, flags):
        v = self.GetValue()
        return '%.2f,%.2f'%(v.x, v.y)
    
    def ChildChanged(self, this_val, child_idx, child_value):
        #print 'test: ', this_val, child_idx, child_value
        if child_idx == 0:
            this_val.x = child_value
        elif child_idx == 1:
            this_val.y = child_value
            
    def StringToValue(self, str, flags):
        val = self.GetValue()
        child_idx = 0
        for s in str.split(','):
            if child_idx == 0:
                val.x = float(s)
            elif child_idx == 1:
                val.y = float(s)
            child_idx += 1
        
        return val
    
    def RefreshChildren(self):
        if self.GetCount()==2:
            val = self.GetValue()
            self.Item(0).SetValue(val.x)
            self.Item(1).SetValue(val.y)    
#--
class WxVector3Property(wxpg.PyProperty):
    'Property for NexusEngine.vector3'
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=vector3()):
        wxpg.PyProperty.__init__(self, label, name)
        self.SetValue(value)
        
        self.AddChild(wxpg.PyFloatProperty('x', wxpg.LABEL_AS_NAME, value.x))
        self.AddChild(wxpg.PyFloatProperty('y', wxpg.LABEL_AS_NAME, value.y))
        self.AddChild(wxpg.PyFloatProperty('z', wxpg.LABEL_AS_NAME, value.z))
        
    def GetValueAsString(self, flags):
        v = self.GetValue()
        return '%.2f,%.2f,%.2f'%(v.x, v.y, v.z)
    
    def ChildChanged(self, this_val, child_idx, child_value):
        #print 'test: ', this_val, child_idx, child_value
        if child_idx == 0:
            this_val.x = child_value
        elif child_idx == 1:
            this_val.y = child_value
        elif child_idx == 2:
            this_val.z = child_value
            
    def StringToValue(self, str, flags):
        val = self.GetValue()
        child_idx = 0
        for s in str.split(','):
            if child_idx == 0:
                val.x = float(s)
            elif child_idx == 1:
                val.y = float(s)
            elif child_idx == 2:
                val.z = float(s)
            child_idx += 1
        
        return val
    
    def RefreshChildren(self):
        if self.GetCount()==3:
            val = self.GetValue()
            self.Item(0).SetValue(val.x)
            self.Item(1).SetValue(val.y)
            self.Item(2).SetValue(val.z)    

#--
class WxVector4Property(wxpg.PyProperty):
    'Property for NexusEngine.vector4'
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=vector4()):
        wxpg.PyProperty.__init__(self, label, name)
        self.SetValue(value)
        
        self.AddChild(wxpg.PyFloatProperty('x', wxpg.LABEL_AS_NAME, value.x))
        self.AddChild(wxpg.PyFloatProperty('y', wxpg.LABEL_AS_NAME, value.y))
        self.AddChild(wxpg.PyFloatProperty('z', wxpg.LABEL_AS_NAME, value.z))
        self.AddChild(wxpg.PyFloatProperty('w', wxpg.LABEL_AS_NAME, value.w))
        
    def GetValueAsString(self, flags):
        v = self.GetValue()
        return '%.2f,%.2f,%.2f,%.2f'%(v.x, v.y, v.z, v.w)
    
    def ChildChanged(self, this_val, child_idx, child_value):
        #print 'test: ', this_val, child_idx, child_value
        if child_idx == 0:
            this_val.x = child_value
        elif child_idx == 1:
            this_val.y = child_value
        elif child_idx == 2:
            this_val.z = child_value
        elif child_idx == 3:
            this_val.w = child_value
            
    def StringToValue(self, str, flags):
        val = self.GetValue()
        child_idx = 0
        for s in str.split(','):
            if child_idx == 0:
                val.x = float(s)
            elif child_idx == 1:
                val.y = float(s)
            elif child_idx == 2:
                val.z = float(s)
            elif child_idx == 3:
                val.w = float(s)
            child_idx += 1
        
        return val
    
    def RefreshChildren(self):
        if self.GetCount()==4:
            val = self.GetValue()
            self.Item(0).SetValue(val.x)
            self.Item(1).SetValue(val.y)
            self.Item(2).SetValue(val.z)
            self.Item(3).SetValue(val.w)    
#--
class WxResLocProperty(wxpg.PyProperty):
    'Property for NexusEngine.resource_location'
    def __init__(self, label, name = wxpg.LABEL_AS_NAME, value=resource_location()):
        wxpg.PyProperty.__init__(self, label, name)
        self.SetValue(value)
        
        self.AddChild(wxpg.PyStringProperty('package', wxpg.LABEL_AS_NAME, value.pkg_name))
        self.AddChild(wxpg.PyStringProperty('file', wxpg.LABEL_AS_NAME, value.file_name))
        
    def GetValueAsString(self, flags):
        v = self.GetValue()
        if v.is_valid():
            return v.to_string()
        else:
            return 'None:None'
    
    def ChildChanged(self, this_val, child_idx, child_value):
        #print 'test: ', this_val, child_idx, child_value
        if child_idx == 0:
            this_val.pkg_name = child_value
        elif child_idx == 1:
            this_val.file_name = child_value
            
    def StringToValue(self, str, flags):
        val = resource_location(str)
        return val
    
    def RefreshChildren(self):
        if self.GetCount()==2:
            val = self.GetValue()
            self.Item(0).SetValue(val.pkg_name)
            self.Item(1).SetValue(val.file_name)    


#-- test code
if __name__ == '__main__':
    class MyFrame(wx.Frame):
        def __init__(self):
            wx.Frame.__init__(self, parent=None, id=-1, title='Property Grid Test')
            
            prop_sizer = wx.BoxSizer(wx.VERTICAL)
            pg = wxpg.PropertyGrid(self,
                                   style=wxpg.PG_SPLITTER_AUTO_CENTER)
            pg.SetExtraStyle(wxpg.PG_EX_HELP_AS_TOOLTIPS)
            prop_sizer.Add(pg,1,wx.EXPAND)
            self.SetSizer(prop_sizer)            
            self.m_prop_grid = pg            
            prop_sizer.SetSizeHints(pg)
                        
            
            #-- system property
            pg.Append( wxpg.StringProperty("String Prop",value="hello") )            
            
            #-- my property 
            pg.Append( WxVector2Property("Vector2 Prop",value=vector2(12,34)) )
            pg.Append( WxVector3Property("Vector3 Prop",value=vector3(12,34,56)) )
            pg.Append( WxVector4Property("Vector4 Prop",value=vector4(12,34,56,78)) )
            pg.Append( WxResLocProperty("ResLoc Prop",value=resource_location('engine_data', "material/test.hlsl") ) )
            pg.Append( WxResLocProperty("ResLoc None" ) )
            
            self.Refresh()

            
    app = wx.PySimpleApp()
    frame = MyFrame()
    frame.Show()
    app.MainLoop()