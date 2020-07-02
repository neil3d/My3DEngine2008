using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;
using System.ComponentModel;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIListViewTest", true)]
    public class UIListViewTest : UIListView
    {
        private UIButton addRowBtn;
        private UIButton addColumnBtn;
        private UIButton addCellBtn;
        private UIButton removeRowBtn;
        private UIButton removeCellBtn;
        private UIButton removeColumnBtn;
        private UIButton resetBtn;

        public UIListViewTest() { } //default ctor not valid - we want to enforce initializing our data
        public UIListViewTest(string name)
            :base(name)
        {
            // 创建测试用按钮
            int offest = 0;
            addRowBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton",this) as UIButton;
            addRowBtn.X = new UIDim(0, offest);
            addRowBtn.Y = new UIDim(1, 56);
            addRowBtn.Width = new UIDim(0, 56);
            addRowBtn.Height = new UIDim(0, 28);
            addRowBtn.NormalText.Text = "addRowBtn";
            addRowBtn.HoveringText.Text = "addRowBtn";
            addRowBtn.PushedText.Text = "addRowBtn";
            offest += 56 + 8;

            addColumnBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            addColumnBtn.X = new UIDim(0, offest);
            addColumnBtn.Y = new UIDim(1, 56);
            addColumnBtn.Width = new UIDim(0, 56);
            addColumnBtn.Height = new UIDim(0, 28);
            addColumnBtn.NormalText.Text = "addColumn";
            addColumnBtn.HoveringText.Text = "addColumn";
            addColumnBtn.PushedText.Text = "addColumn";
            offest += 56 + 8;

            addCellBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            addCellBtn.X = new UIDim(0, offest);
            addCellBtn.Y = new UIDim(1, 56);
            addCellBtn.Width = new UIDim(0, 56);
            addCellBtn.Height = new UIDim(0, 28);
            addCellBtn.NormalText.Text = "addCell";
            addCellBtn.HoveringText.Text = "addCell";
            addCellBtn.PushedText.Text = "addCell";
            offest += 56 + 8;

            removeRowBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            removeRowBtn.X = new UIDim(0, offest);
            removeRowBtn.Y = new UIDim(1, 56);
            removeRowBtn.Width = new UIDim(0, 56);
            removeRowBtn.Height = new UIDim(0, 28);
            removeRowBtn.NormalText.Text = "removeRow";
            removeRowBtn.HoveringText.Text = "removeRow";
            removeRowBtn.PushedText.Text = "removeRow";
            offest += 56 + 8;

            removeCellBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            removeCellBtn.X = new UIDim(0, offest);
            removeCellBtn.Y = new UIDim(1, 56);
            removeCellBtn.Width = new UIDim(0, 56);
            removeCellBtn.Height = new UIDim(0, 28);
            removeCellBtn.NormalText.Text = "removeCell";
            removeCellBtn.HoveringText.Text = "removeCell";
            removeCellBtn.PushedText.Text = "removeCell";
            offest += 56 + 8;

            removeColumnBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            removeColumnBtn.X = new UIDim(0, offest);
            removeColumnBtn.Y = new UIDim(1, 56);
            removeColumnBtn.Width = new UIDim(0, 56);
            removeColumnBtn.Height = new UIDim(0, 28);
            removeColumnBtn.NormalText.Text = "removeCol";
            removeColumnBtn.HoveringText.Text = "removeCol";
            removeColumnBtn.PushedText.Text = "removeCol";
            offest += 56 + 8;

            resetBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            resetBtn.X = new UIDim(0, offest);
            resetBtn.Y = new UIDim(1, 56);
            resetBtn.Width = new UIDim(0, 56);
            resetBtn.Height = new UIDim(0, 28);
            resetBtn.NormalText.Text = "reset";
            resetBtn.HoveringText.Text = "reset";
            resetBtn.PushedText.Text = "reset";
            offest += 56 + 8;

            InitEventHandler();
        }

        private void InitEventHandler()
        {
            addRowBtn.Clicked += new EventHandler<GUIEventArgs>(addRowBtn_Clicked);
            addColumnBtn.Clicked += new EventHandler<GUIEventArgs>(addColumnBtn_Clicked);
            addCellBtn.Clicked += new EventHandler<GUIEventArgs>(addCellBtn_Clicked);
            removeRowBtn.Clicked += new EventHandler<GUIEventArgs>(removeRowBtn_Clicked);
            removeColumnBtn.Clicked += new EventHandler<GUIEventArgs>(removeColumnBtn_Clicked);
            removeCellBtn.Clicked += new EventHandler<GUIEventArgs>(removeCellBtn_Clicked);
            resetBtn.Clicked += new EventHandler<GUIEventArgs>(resetBtn_Clicked);
        }

        private void addRowBtn_Clicked(object sender,GUIEventArgs e)
        {
            int index = this.AppendRow();
            GridRow row = this.GridData[index];
            for (int col = 0; col < ColumnCount; ++col)
            {
                UIComp_DrawListboxItem comp = row[col];
                comp.Text = "C " + index.ToString(System.Globalization.CultureInfo.CurrentCulture) + "-" + col.ToString(System.Globalization.CultureInfo.CurrentCulture);
            }
        }
        private void addColumnBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.AddColumn("Column", 30);
        }
        private void addCellBtn_Clicked(object sender, GUIEventArgs args)
        {
            try
            {
                this.SetGridItem(0, 0, "addCellBtn_Clicked");
            }
            catch (Nexus.GUI.InvalidRequestException e)
            {
                NLogger.Instance.WriteString(LogType.Info, e.Message);
                throw e;
            }
            catch
            {
                NLogger.Instance.WriteString(LogType.Info, "Can not set grid item content.");
            }
        }
        private void removeRowBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.RemoveRow(0);
        }
        private void removeCellBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.SetGridItem(0, 0, "");
        }
        private void removeColumnBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.RemoveColumn(0);
        }
        private void resetBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.ResetList();
        }

        #region Serialization
        
        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            InitEventHandler();
        }

        [Browsable(false)]
        [XmlFieldSerializable("addRowBtnHelper")]
        public string  addRowBtnHelper
        {
            get { return addRowBtn.Name; }
            set { addRowBtn = children[value] as UIButton; }
        }

        [Browsable(false)]
        [XmlFieldSerializable("addColumnBtnHelper")]
        public string addColumnBtnHelper
        {
            get { return addColumnBtn.Name; }
            set { addColumnBtn = children[value] as UIButton; }
        }

        [Browsable(false)]
        [XmlFieldSerializable("addCellBtnHelper")]
        public string addCellBtnHelper
        {
            get
            {
                return addCellBtn.Name;
            }
            set
            {
                addCellBtn = children[value] as UIButton;
            }
        }

        [Browsable(false)]
        [XmlFieldSerializable("removeRowBtnHelper")]
        public string removeRowBtnHelper
        {
            get
            {
                return removeRowBtn.Name;
            }
            set
            {
                removeRowBtn = children[value] as UIButton;
            }
        }

        [Browsable(false)]
        [XmlFieldSerializable("removeCellBtnHelper")]
        public string removeCellBtnHelper
        {
            get
            {
                return removeCellBtn.Name;
            }
            set
            {
                removeCellBtn = children[value] as UIButton;
            }
        }

              
        [Browsable(false)]
        [XmlFieldSerializable("removeColumnBtnHelper")]
        public string removeColumnBtnHelper
        {
            get
            {
                return removeColumnBtn.Name;
            }
            set
            {
                removeColumnBtn = children[value] as UIButton;
            }
        }       
        
        [Browsable(false)]
        [XmlFieldSerializable("resetBtnHelper")]
        public string resetBtnHelper
        {
            get
            {
                return resetBtn.Name;
            }
            set
            {
                resetBtn = children[value] as UIButton;
            }
        }
        #endregion



    }
}
