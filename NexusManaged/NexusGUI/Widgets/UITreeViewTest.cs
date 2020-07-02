using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Nexus.GUI.Components;
using NexusEngine;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;
using System.ComponentModel;

namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UITreeViewTest", true)]
    public class UITreeViewTest : UITreeView
    {
        protected UIButton addItemBtn;
        protected UIButton removeItemBtn;
        protected UIButton resetBtn;

        public UITreeViewTest() { } //default ctor not valid - we want to enforce initializing our data
        public UITreeViewTest(string name)
            :base(name)
        {
            // 创建测试用按钮
            int offest = 0;
            addItemBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton",this) as UIButton;
            addItemBtn.X = new UIDim(0, offest);
            addItemBtn.Y = new UIDim(1, 56);
            addItemBtn.Width = new UIDim(0, 56);
            addItemBtn.Height = new UIDim(0, 28);
            addItemBtn.NormalText.Text = "addItem";
            addItemBtn.HoveringText.Text = "addItem";
            addItemBtn.PushedText.Text = "addItem";
            offest += 56 + 8;

            removeItemBtn = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UIButton", this) as UIButton;
            removeItemBtn.X = new UIDim(0, offest);
            removeItemBtn.Y = new UIDim(1, 56);
            removeItemBtn.Width = new UIDim(0, 56);
            removeItemBtn.Height = new UIDim(0, 28);
            removeItemBtn.NormalText.Text = "removeItem";
            removeItemBtn.HoveringText.Text = "removeItem";
            removeItemBtn.PushedText.Text = "removeItem";
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
            this.ClearTreeNodes();
        }

        private void InitEventHandler()
        {
            addItemBtn.Clicked += new EventHandler<GUIEventArgs>(addItemBtn_Clicked);
            removeItemBtn.Clicked += new EventHandler<GUIEventArgs>(removeItemBtn_Clicked);
            resetBtn.Clicked += new EventHandler<GUIEventArgs>(resetBtn_Clicked);
        }

        private void addItemBtn_Clicked(object sender,GUIEventArgs e)
        {
            UIComp_TreeNodeBase newNode = this.AppendTreeNode(this.RootNode);
            newNode.Text = "My custom node";
            int count = 10;
            while (--count > 0)
            {
                newNode = newNode.InsertTreeNode(0);
            }
        }
 
        private void removeItemBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.RemoveTreeNode(0, rootNode);
        }

        private void resetBtn_Clicked(object sender, GUIEventArgs e)
        {
            this.ClearTreeNodes();
        }

        #region Serialization

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            InitEventHandler();
        }

        [Browsable(false)]
        [XmlFieldSerializable("addItemBtnHelper")]
        public string addItemBtnHelper
        {
            get { return addItemBtn.Name; }
            set { addItemBtn = children[value] as UIButton; }
        }

        [Browsable(false)]
        [XmlFieldSerializable("removeItemBtnHelper")]
        public string removeItemBtnHelper
        {
            get { return removeItemBtn.Name; }
            set { removeItemBtn = children[value] as UIButton; }
        }

        [Browsable(false)]
        [XmlFieldSerializable("resetBtnHelper")]
        public string resetBtnHelper
        {
            get { return resetBtn.Name; }
            set { resetBtn = children[value] as UIButton; }
        }

        #endregion
    }
}
