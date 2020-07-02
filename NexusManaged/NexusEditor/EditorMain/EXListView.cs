using System;
using System.Windows.Forms;
using System.Drawing;
using System.Collections;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace EXControls 
{
    public delegate void ExListViewItemChangedEvent(EXListViewItem item);
    public delegate void ExListViewSubItemChangedEvent(EXListViewSubItemBase item,string oldText,string newText);

    public class EXListView : ListView 
    {
        private List<EXListViewItem> m_ItemList;
        public List<EXListViewItem> ItemList
        {
            get { return m_ItemList; }
        }
        private ListViewItem.ListViewSubItem _clickedsubitem; //clicked ListViewSubItem
        private ListViewItem _clickeditem; //clicked ListViewItem
        private int _col; //index of doubleclicked ListViewSubItem
        private TextBox txtbx; //the default edit control
        private int _sortcol; //index of clicked ColumnHeader
        private Brush _sortcolbrush; //color of items in sorted column
        private Brush _highlightbrush; //color of highlighted items
        private int _cpadding; //padding of the embedded controls
            
        private const UInt32 LVM_FIRST = 0x1000;
        private const UInt32 LVM_SCROLL = (LVM_FIRST + 20);
        private const int WM_HSCROLL = 0x114;
        private const int WM_VSCROLL = 0x115;
        private const int WM_MOUSEWHEEL = 0x020A;
        private const int WM_PAINT = 0x000F;
        private ImageList m_ItemHeight;
            
        [DllImport("user32.dll")]
        private static extern bool SendMessage(IntPtr hWnd, UInt32 m, int wParam, int lParam);
        
        protected override void WndProc(ref Message m) 
        {
            switch (m.Msg)
            {
                case WM_HSCROLL:
                case WM_VSCROLL:
                case WM_MOUSEWHEEL:
                    this.Focus();
                    break;
            }
            base.WndProc(ref m);
        }
        
        private void ScrollMe(int x, int y) 
        {
            SendMessage((IntPtr) this.Handle, LVM_SCROLL, x, y);
        }
        
        public EXListView()
        {
            _cpadding = 4;
            _sortcol = -1;
            _sortcolbrush = SystemBrushes.ControlLight;
            _highlightbrush = SystemBrushes.Highlight;
            this.OwnerDraw = true;
            this.FullRowSelect = true;
            this.View = View.Details;
            this.MouseDown += new MouseEventHandler(this_MouseDown);
            this.MouseDoubleClick += new MouseEventHandler(this_MouseDoubleClick);
            this.DrawColumnHeader += new DrawListViewColumnHeaderEventHandler(this_DrawColumnHeader);
            this.DrawSubItem += new DrawListViewSubItemEventHandler(this_DrawSubItem);
            this.MouseMove += new MouseEventHandler(this_MouseMove);
            this.ColumnClick += new ColumnClickEventHandler(this_ColumnClick);
            txtbx = new TextBox();
            txtbx.Visible = false;
            this.Controls.Add(txtbx);
            txtbx.Leave += new EventHandler(c_Leave);
            txtbx.KeyPress += new KeyPressEventHandler(txtbx_KeyPress);
            m_ItemHeight = new ImageList();
            m_ItemHeight.ImageSize = new Size(20, 20);
            this.SmallImageList = m_ItemHeight;
            m_ItemList = new List<EXListViewItem>();
        }
        
        public void AddItem(EXListViewItem item)
        {
            m_ItemList.Add(item);
            base.Items.Add(item);
        }

        public void RemoveItem(int index)
        {
            if (index < 0 || index >= m_ItemList.Count) return;
            base.Items.RemoveAt(index);
            m_ItemList.RemoveAt(index);
        }

        public new void Clear()
        {
            base.Items.Clear();
            m_ItemList.Clear();
        }

        public void SetItemHeight(int height)
        {
            m_ItemHeight.ImageSize = new Size(height, height); // 控制行高的丑陋方法,但是的确工作的很不错……
        }

        public int ControlPadding 
        {
            get {return _cpadding;}
            set {_cpadding = value;}
        }
        
        public Brush MySortBrush 
        {
            get {return _sortcolbrush;}
            set {_sortcolbrush = value;}
        }
        
        public Brush MyHighlightBrush 
        {
            get {return _highlightbrush;}
            set {_highlightbrush = value;}
        }
        
        private void txtbx_KeyPress(object sender, KeyPressEventArgs e) 
        {
            if (e.KeyChar == (char) Keys.Return) 
            {
                txtbx.Visible = false;
                _clickeditem.Tag = null;
            }
        }
        
        private void c_Leave(object sender, EventArgs e) 
        {
            Control c = (Control) sender;
            EXListViewSubItemBase item = (EXListViewSubItemBase)(_clickedsubitem);
            if (item != null)
            {
                item.Text = c.Text;
            }
            else
            {
                _clickedsubitem.Text = c.Text;
            }
            c.Visible = false;
            _clickeditem.Tag = null;
        }
        
        private void this_MouseDown(object sender, MouseEventArgs e) 
        {
            ListViewHitTestInfo lstvinfo = this.HitTest(e.X, e.Y);
            ListViewItem.ListViewSubItem subitem = lstvinfo.SubItem;
            if (subitem == null) return;
            int subx = subitem.Bounds.Left;
            if (subx < 0)
            {
                this.ScrollMe(subx, 0);
            }
        }
        
        private void this_MouseDoubleClick(object sender, MouseEventArgs e) 
        {
            EXListViewItem lstvItem = this.GetItemAt(e.X, e.Y) as EXListViewItem;
            if (lstvItem == null) return;
            _clickeditem = lstvItem;
            int x = lstvItem.Bounds.Left;
            int i;
            for (i = 0; i < this.Columns.Count; i++)
            {
                x = x + this.Columns[i].Width;
                if (x > e.X) 
                {
                    x = x - this.Columns[i].Width;
                    _clickedsubitem = lstvItem.SubItems[i];
                    _col = i;
                    break;
                }
            }
            if (!(this.Columns[i] is EXColumnHeader)) return;
            EXColumnHeader col = (EXColumnHeader) this.Columns[i];
            if (col.GetType() == typeof(EXEditableColumnHeader)) 
            {
                EXEditableColumnHeader editcol = (EXEditableColumnHeader) col;
                txtbx.Location = new Point(x, this.GetItemRect(this.Items.IndexOf(lstvItem)).Y);
                txtbx.Width = this.Columns[i].Width;
                if (txtbx.Width > this.Width) txtbx.Width = this.ClientRectangle.Width;
                txtbx.Text = _clickedsubitem.Text;
                txtbx.Visible = true;
                txtbx.BringToFront();
                txtbx.Focus();
            } 
            else if (col.GetType() == typeof(EXBoolColumnHeader)) 
            {
                EXBoolColumnHeader boolcol = (EXBoolColumnHeader) col;
                if (boolcol.Editable) 
                {
                    EXBoolListViewSubItem boolsubitem = (EXBoolListViewSubItem) _clickedsubitem;
                    if (boolsubitem.BoolValue == true) 
                    {
                        boolsubitem.BoolValue = false;
                    } 
                    else
                    {
                        boolsubitem.BoolValue = true;
                    }
                    this.Invalidate(boolsubitem.Bounds);
                }
            }
        }
        
        private void cmbx_SelectedValueChanged(object sender, EventArgs e) 
        {
            if (((Control) sender).Visible == false || _clickedsubitem == null) return;
        }
        
        private void this_MouseMove(object sender, MouseEventArgs e) 
        {
            ListViewItem item = this.GetItemAt(e.X, e.Y);
            if (item != null && item.Tag == null) {
                this.Invalidate(item.Bounds);
                item.Tag = "t";
            }
        }
        
        private void this_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e) 
        {
            e.DrawDefault = true;
        }
        
        private void this_DrawSubItem(object sender, DrawListViewSubItemEventArgs e) 
        {
            e.DrawBackground();
            if (e.ColumnIndex == _sortcol)
            {
                e.Graphics.FillRectangle(_sortcolbrush, e.Bounds);
            }
            if ((e.ItemState & ListViewItemStates.Selected) != 0) {
                e.Graphics.FillRectangle(_highlightbrush, e.Bounds);
            }
            int fonty = e.Bounds.Y + ((int)(e.Bounds.Height / 2)) - ((int)(e.SubItem.Font.Height / 2));
            int x = e.Bounds.X + 2;
            if (e.ColumnIndex == 0) 
            {
                EXListViewItem item = (EXListViewItem) e.Item;
                if (item.GetType() == typeof(EXImageListViewItem)) {
                    EXImageListViewItem imageitem = (EXImageListViewItem) item;
                    if (imageitem.ItemImage != null) {
                        Image img = imageitem.ItemImage;
                        int imgy = e.Bounds.Y + ((int) (e.Bounds.Height / 2)) - ((int) (img.Height / 2));
                        e.Graphics.DrawImage(img, x, imgy, img.Width, img.Height);
                        x += img.Width + 2;
                    }
                }
                e.Graphics.DrawString(e.SubItem.Text, e.SubItem.Font, new SolidBrush(e.SubItem.ForeColor), x, fonty);
                return;
            }
            EXListViewSubItemBase subitem = e.SubItem as EXListViewSubItemBase;
            if (subitem == null) 
            {
                e.DrawDefault = true;
            }
            else 
            {
                x = subitem.DoDraw(e, x, this.Columns[e.ColumnIndex] as EXColumnHeader);                
                e.Graphics.DrawString(e.SubItem.Text, e.SubItem.Font, new SolidBrush(e.SubItem.ForeColor), x, fonty);
            }
        }
        
        private void this_ColumnClick(object sender, ColumnClickEventArgs e) 
        {
            
        }        
    }
    
    public class EXColumnHeader : ColumnHeader 
    {
        
        public EXColumnHeader() 
        {
            
        }
        
        public EXColumnHeader(string text)
        {
            this.Text = text;
        }
        
        public EXColumnHeader(string text, int width)
        {
            this.Text = text;
            this.Width = width;
        }
        
    }
    
    public class EXEditableColumnHeader : EXColumnHeader 
    {
        
        private Control _control;
        
        public EXEditableColumnHeader() 
        {
            
        }
        
        public EXEditableColumnHeader(string text) 
        {
            this.Text = text;
        }
        
        public EXEditableColumnHeader(string text, int width) 
        {
            this.Text = text;
            this.Width = width;
        }
        
        public EXEditableColumnHeader(string text, Control control) 
        {
            this.Text = text;
            this.MyControl = control;
        }
        
        public EXEditableColumnHeader(string text, Control control, int width) 
        {
            this.Text = text;
            this.MyControl = control;
            this.Width = width;
        }
        
        public Control MyControl 
        {
            get {return _control;}
            set 
            {
                _control = value;
                _control.Visible = false;
                _control.Tag = "not_init";
            }
        }
        
    }
    
    public class EXBoolColumnHeader : EXColumnHeader 
    {
        
        private Image _trueimage;
        private Image _falseimage;
        private bool _editable;
            
        public EXBoolColumnHeader()
        {
            init();
        }
        
        public EXBoolColumnHeader(string text)
        {
            init();
            this.Text = text;
        }
        
        public EXBoolColumnHeader(string text, int width) 
        {
            init();
            this.Text = text;
            this.Width = width;
        }
        
        public EXBoolColumnHeader(string text, Image trueimage, Image falseimage)
        {
            init();
            this.Text = text;
            _trueimage = trueimage;
            _falseimage = falseimage;
        }
        
        public EXBoolColumnHeader(string text, Image trueimage, Image falseimage, int width)
        {
            init();
            this.Text = text;
            _trueimage = trueimage;
            _falseimage = falseimage;
            this.Width = width;
        }
        
        private void init()
        {
            _editable = false;
        }
        
        public Image TrueImage 
        {
            get {return _trueimage;}
            set {_trueimage = value;}
        }
        
        public Image FalseImage 
        {
            get {return _falseimage;}
            set {_falseimage = value;}
        }
        
        public bool Editable 
        {
            get {return _editable;}
            set {_editable = value;}
        }
        
    }
    
    public abstract class EXListViewSubItemBase : ListViewItem.ListViewSubItem
    {
        private Object m_UserData;
        public Object UserData
        {
            get
            {
                return m_UserData;
            }
            set
            {
                m_UserData = value;
            }
        }
        private bool m_Responsible; //是否响应用户的双击操作

        public bool Responsible
        {
            get
            {
                return m_Responsible;
            }
            set
            {
                m_Responsible = value;
            }
        }
        private string _value = "";
        public ExListViewSubItemChangedEvent  OnItemValueChanged;

        public EXListViewSubItemBase() 
        {
            m_UserData = null; m_Responsible = true;
        }
        
        public EXListViewSubItemBase(string text) 
        {
            this.Text = text;
            m_UserData = null;
            m_Responsible = true;
        }
        
        public string ItemValue 
        {
            get {return _value;}
            set 
            {
                _value = value;
                base.Text = value;
            }
        }

        public new string Text
        {
            get
            {
                return base.Text;
            }
            set
            {
                string old = _value;
                base.Text = value;
                _value = value;
                if (OnItemValueChanged != null)
                {
                    OnItemValueChanged(this,old,_value);
                }
            }
        }

        //return the new x coordinate
        public abstract int DoDraw(DrawListViewSubItemEventArgs e, int x, EXControls.EXColumnHeader ch);
    }
    
    public class EXListViewSubItem : EXListViewSubItemBase
    {
        public EXListViewSubItem() 
        {
            
        }
        
        public EXListViewSubItem(string text)
        {
            this.Text = text;
        }
        
        public override int DoDraw(DrawListViewSubItemEventArgs e, int x, EXControls.EXColumnHeader ch) 
        {
            return x;
        }

    }
    
    public class EXBoolListViewSubItem : EXListViewSubItemBase 
    {
        
        private bool _value;
       
        
        public EXBoolListViewSubItem() 
        {
            
        }
        
        public EXBoolListViewSubItem(bool val) 
        {
            _value = val;
        }

        public bool BoolValue
        {
            get { return _value; }
            set
            {
                bool old = _value;
                _value = value;
                if (OnItemValueChanged != null && old!=_value)
                {
                    OnItemValueChanged(this, "", "");
                }
            }
        }

        public void SetValue(bool value)
        {
            _value = value;
        }
        
        public override int DoDraw(DrawListViewSubItemEventArgs e, int x, EXColumnHeader ch) 
        {    
            EXBoolColumnHeader boolcol = (EXBoolColumnHeader) ch;
            Image boolimg;
            if (this.BoolValue == true) 
            {
                boolimg = boolcol.TrueImage;
            } 
            else
            {
                boolimg = boolcol.FalseImage;
            }
            int imgy = e.Bounds.Y + ((int) (e.Bounds.Height / 2)) - ((int) (boolimg.Height / 2));
            e.Graphics.DrawImage(boolimg, x, imgy, boolimg.Width, boolimg.Height);
            x += boolimg.Width + 2;
            return x;
        }
        
    }
    
    public class EXListViewItem : ListViewItem 
    {
        protected List<EXListViewSubItemBase> m_SubItemList;
        public List<EXListViewSubItemBase> SubItemList
        {
            get { return m_SubItemList; }
        }
        private Object m_UserData;
        public Object UserData
        {
            get
            {
                return m_UserData;
            }
            set
            {
                m_UserData = value;
            }
        }
	    private string _value;
        public ExListViewItemChangedEvent  OnItemValueChanged;

        public EXListViewItem()
        {
            m_UserData = null;
            m_SubItemList = new List<EXListViewSubItemBase>();
        }
        
        public EXListViewItem(string text)
        {
            this.Text = text;
            m_UserData = null;
            m_SubItemList = new List<EXListViewSubItemBase>();
        }

        public void AddSubItem(EXListViewSubItemBase item)
        {
            m_SubItemList.Add(item);
            base.SubItems.Add(item);
        }
	
        public string ItemValue 
        {
            get {return _value;}
            set 
            { 
                _value = value; 
                if (OnItemValueChanged != null)
                {
                    OnItemValueChanged(this);
                }
            }
        }
        
    }
    
    public class EXImageListViewItem : EXListViewItem
    {
        
        private Image _image;
            
        public EXImageListViewItem()
        {
            
        }
        
        public EXImageListViewItem(string text) 
        {
            this.Text = text;
        }
        
        public EXImageListViewItem(Image image) 
        {
            _image = image;
        }
	
        public EXImageListViewItem(string text, Image image) 
        {
            _image = image;
            this.Text = text;
        }
	
	    public EXImageListViewItem(string text, Image image, string value) 
        {
            this.Text = text;
            _image = image;
	        this.ItemValue = value;
        }
        
        public Image ItemImage 
        {
            get {return _image;}
            set 
            {
                _image = value; 
                OnItemValueChanged(this);
                if (OnItemValueChanged != null)
                {
                    OnItemValueChanged(this);
                }
            }
        }
    }    

}