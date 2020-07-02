using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Collections;
using System.IO;

using NexusEngine;
using Nexus.GUI;
using NexusEngineExtension;
using Nexus.GameFramework;
using NexusEditor.EditorMain;

namespace NexusEditor.UIEditor
{
    public partial class UIEditor : Form
    {
        #region Fields

        private NGUIRenderControl guiRenderControl;
        private Timer updateTimer = new Timer();
        private bool m_finalClose;
        private bool m_stringDictLoad;
        private ResXEditor guiStringDictEditor;
        /// <summary>
        /// 文件存储格式
        /// </summary>
        internal static readonly string DefaultExt = "nui";
        private string lastFileName;

        /// <summary>
        /// 当前选择窗口
        /// </summary>
        private WindowCollection curSelectWnds = new WindowCollection();

        public enum UIEditorMode
        {
            Preview,
            Editor,
        }

        private UIEditorMode currentMode;

        public NResourceLoc CopyWindowResourceName { get; set; }

        public UndoRedo UndoRedoCommands { get; private set; }

        #endregion

        #region Properties

        public WindowCollection CurrentSelectWindows
        {
            get { return curSelectWnds; }
        }

        public UIEditorMode CurrentMode
        {
            get { return currentMode; }
            set
            {
                currentMode = value;

                this.toolStripPreview.CheckState = CheckState.Unchecked;
                this.toolStripEditor.CheckState = CheckState.Unchecked;
                if (currentMode == UIEditorMode.Preview)
                {
                    this.toolStripPreview.CheckState = CheckState.Checked;
                }
                else if (currentMode == UIEditorMode.Editor)
                {
                    this.toolStripEditor.CheckState = CheckState.Checked;
                }
            }
        }

        #endregion

        public UIEditor()
        {
            InitializeComponent();

            m_finalClose = false;
            m_stringDictLoad = false;

            guiRenderControl = new NGUIRenderControl(this);
            guiRenderControl.Dock = DockStyle.Fill;
            guiRenderControl.CreateViewport();
            guiRenderControl.WindowCreated += new UIECreateWindowEventHander(guiRenderControl_WindowCreated);

            this.Closing += new CancelEventHandler(UIEditor_Closing);
            this.GUIPreview.Controls.Add(guiRenderControl);

            CopyWindowResourceName = new NResourceLoc("content", "ui/_temp/_copy.nui");
            CurrentMode = UIEditorMode.Editor;

            UndoRedoCommands = new UndoRedo();
        }


        public new void Close()
        {
            guiRenderControl.Dispose();

            m_finalClose = true;
            base.Close();
        }

        private void SelfDispose()
        {
            if (updateTimer != null)
            {
                updateTimer.Stop();
                updateTimer.Dispose();
                updateTimer = null;
            }
        }

        /// <summary>
        /// 添加所有已经注册的窗口类
        /// </summary>
        private void RefreshRegeditedGUIControls()
        {
            // 先清空列表框
            listWidgets.Items.Clear();

            // 添加所有已经注册的窗口类
            foreach (string wndType in WindowManager.Instance.AllWindowTypes.Keys)
            {
                ListViewItem lvi = new ListViewItem(wndType);
                listWidgets.Items.Add(lvi);
            }
        }

        /// <summary>
        /// 刷新已创建控件树结构
        /// </summary>
        private void RefreshContolsTree()
        {
            treeViewControls.BeginUpdate();
            treeViewControls.Nodes.Clear();

            Window root = GUISystem.Instance.RootWindow;
            TreeNode rootNode = new TreeNode(root.Name);
            rootNode.Tag = root;
            treeViewControls.Nodes.Add(rootNode);
            AddWindow2Tree(root, rootNode);

            treeViewControls.ExpandAll();
            treeViewControls.EndUpdate();

            // 更新组合框
            childrenControls.BeginUpdate();
            childrenControls.Items.Clear();
            foreach (Window child in root.Children)
            {
                ListViewItem item = new ListViewItem(child.Name + " " + child.GetType().ToString());
                item.Tag = child;
                childrenControls.Items.Add(item);

                foreach (Window selChild in child.Children)
                {
                    ListViewItem ltItem = new ListViewItem(selChild.Name + " " + selChild.GetType().ToString());
                    ltItem.Tag = selChild;
                    childrenControls.Items.Add(ltItem);
                }
            }
            childrenControls.EndUpdate();
        }

        private void AddWindow2Tree(Window wnd, TreeNode tn)
        {
            if (wnd == null)
                return;

            tn.Nodes.Clear();

            foreach (Window child in wnd.Children)
            {
                TreeNode childNode = new TreeNode(child.Name + " [" + child.GetType() + "]");
                childNode.Tag = child;
                tn.Nodes.Add(childNode);

                AddWindow2Tree(child, childNode);
            }
        }

        public void AddCurrentSelectWindow(Window wnd)
        {
            if (wnd == null)
                return;

            bool remove = false;
            foreach (var item in CurrentSelectWindows)
            {
                if (item.Name.Equals(wnd.Name))
                {
                    CurrentSelectWindows.Remove(item);
                    remove = true;
                    break;
                }
            }
            if (!remove)
            {
                CurrentSelectWindows.Add(wnd);
            }
            ChangeCurrentSelectWindow();
        }

        public void ClearCurrentSelectWindow()
        {
            CurrentSelectWindows.Clear();

            ChangeCurrentSelectWindow();
        }

        public void RemoveCurrentSelectWindow(Window wnd)
        {
            if (CurrentSelectWindows.Contains(wnd))
            {
                CurrentSelectWindows.Remove(wnd);

                ChangeCurrentSelectWindow();
            }
        }

        /// <summary>
        /// 当前选择窗口发生了改变
        /// </summary>
        /// <param name="wnd"></param>
        private void ChangeCurrentSelectWindow()
        {
            // 更新属性框选择对象
            propertiesWidget.SelectedObjects = curSelectWnds.ToArray();
        }

        #region  Event Handler
        private void UIEditor_Load(object sender, EventArgs e)
        {
            updateTimer.Interval = 10;
            updateTimer.Tick += new EventHandler(updateTimer_Tick);
            updateTimer.Start();


            // 显示所有已经注册的控件
            RefreshRegeditedGUIControls();
            // 刷新当前正在编辑UI控件的树状结构
            RefreshContolsTree();
            // 更改当前选择的窗口
            AddCurrentSelectWindow(GUISystem.Instance.RootWindow);

            RelocationStytlePath(new NResourceLoc("content", "/ui/style"));
        }

        private void UIEditor_Closing(object sender, CancelEventArgs e)
        {
            // 刷新当前正在编辑UI控件的树状结构
            RefreshContolsTree();
        }

        /// <summary>
        /// When the timer fires, refresh control
        /// </summary>
        private void updateTimer_Tick(object sender, EventArgs e)
        {
            guiRenderControl.Invalidate();
        }


        private void guiRenderControl_WindowCreated(object sender, UIECreateWindowEventArg e)
        {
            // 刷新当前正在编辑UI控件的树状结构
            RefreshContolsTree();
            // 更改当前选择的窗口
            ClearCurrentSelectWindow();
            AddCurrentSelectWindow(e.NewCreatedWindow);
        }

        #endregion Event Handler


        private void refreshControlTypesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 显示所有已经注册的控件
            RefreshRegeditedGUIControls();
        }

        private void refreshGUITreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 刷新当前正在编辑UI控件的树状结构
            RefreshContolsTree();
        }

        /// <summary>
        /// 处理拖放控件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void listWidgets_ItemDrag(object sender, ItemDragEventArgs e)
        {
            if (e.Button != GUISystem.ClickButton)
                return;

            int nTotalSel = listWidgets.SelectedItems.Count;
            if (nTotalSel <= 0)
                return;

            IEnumerator SelCol = listWidgets.SelectedItems.GetEnumerator();
            SelCol.MoveNext();

            ListViewItem lvi = (ListViewItem)SelCol.Current;
            listWidgets.DoDragDrop(lvi.SubItems[0].Text, DragDropEffects.Copy | DragDropEffects.Move);
        }

        private void treeViewControls_AfterSelect(object sender, TreeViewEventArgs e)
        {
            Window wnd = (e.Node.Tag) as Window;
            // 更改当前选择的窗口
            ClearCurrentSelectWindow();
            AddCurrentSelectWindow(wnd);
        }

        private void treeViewControls_DragDrop(object sender, DragEventArgs e)
        {
            try
            {
                TreeNode SrcNode;
                if (e.Data.GetDataPresent(typeof(TreeNode)))
                {
                    SrcNode = e.Data.GetData("System.Windows.Forms.TreeNode") as TreeNode;
                    Window SrcWnd = SrcNode.Tag as Window;
                    // 根窗口不允许移动
                    if (SrcWnd.Parent == null)
                        return;

                    Point pt = ((TreeView)sender).PointToClient(new Point(e.X, e.Y));

                    TreeNode DestNode = ((TreeView)sender).GetNodeAt(pt);
                    Window DestWnd = DestNode.Tag as Window;

                    if (SrcWnd == DestWnd)
                        return;

                    // 修改窗口的父窗口
                    SrcWnd.Parent.RemoveChild(SrcWnd);
                    DestWnd.AddChild(SrcWnd);

                    DestNode.Nodes.Add((TreeNode)SrcNode.Clone());

                    DestNode.Expand();

                    //删除已经移动的节点
                    SrcNode.Remove();
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Controls tree drag drop faild !");
            }
        }

        /// <summary>
        /// 拖放进入改变光标
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeViewControls_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode"))
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        /// <summary>
        /// 控件树的拖放操作
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void treeViewControls_ItemDrag(object sender, ItemDragEventArgs e)
        {
            TreeNode tn = e.Item as TreeNode;
            // 根节点不允许拖放
            if (e.Button == GUISystem.ClickButton
                && tn != null
                && tn.Parent != null)
            {
                this.treeViewControls.DoDragDrop(tn, DragDropEffects.Move | DragDropEffects.Link | DragDropEffects.Copy);
            }
        }

        /// <summary>
        /// 通过Combox选择控件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void childrenControls_SelectedIndexChanged(object sender, EventArgs e)
        {
            int index = this.childrenControls.SelectedIndex;
            ListViewItem item = this.childrenControls.Items[index] as ListViewItem;
            if (item != null)
            {
                Window wnd = item.Tag as Window;
                if (wnd != null)
                {
                    ClearCurrentSelectWindow();
                    AddCurrentSelectWindow(item.Tag as Window);
                }
            }
        }

        private void UIEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!m_finalClose)
            {
                e.Cancel = true;
                this.Hide();
            }
        }

        /// <summary>
        /// 打开文件
        /// </summary>
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewUIProject();
            OpenFromFile();
        }

        /// <summary>
        /// 是否创建新的项目
        /// </summary>
        /// <param name="newProject"></param>
        private void OpenFromFile()
        {
            try
            {
                string defalultFileName = Path.GetFileNameWithoutExtension(lastFileName);
                using (VirtualFileDialog dlg = new VirtualFileDialog(true, defalultFileName, DefaultExt))
                {
                    dlg.Text = "Open GUI file...";
                    if (dlg.ShowDialog(this) == DialogResult.OK)
                    {
                        NResourceLoc loc = dlg.GetResourceLocation();

                        ReloadResource(GUISystem.Instance.RootWindow, loc.PackageName, loc.FileName, false);
                    }
                }
            }
            catch (System.Exception e)
            {
                NexusEditor.Program.ShowException(e, "Open GUI file faild!");
            }
        }

        /// <summary>
        /// 重命名控件，在名称前加前缀
        /// </summary>
        private void RenameWindow(Window window, Window parent, bool renameChildren)
        {
            window.Name = WindowManager.Instance.FindUniqueName(window.GetType().Name, parent);
            if (renameChildren)
            {
                foreach (var item in window.Children)
                {
                    RenameWindow(item, window, renameChildren);
                }
            }
        }

        private WindowCollection ReloadResource(Window parent, string pkgName, string fileName, bool renameWindow)
        {
            if (parent == null)
                return null;

            WindowCollection ret = new WindowCollection();
            Window window = WindowManager.Instance.Deserialize(pkgName, fileName);
            if (window != null)
            {
                if (window.Name.Equals("root", StringComparison.OrdinalIgnoreCase))
                {
                    // 如果序列化的窗口时根节点，那么自处理它的子节点，约定根节点的名字为root
                    foreach (var item in window.Children)
                    {
                        item.PostSerial(parent);

                        if (renameWindow)
                        {
                            RenameWindow(item, window, renameWindow);
                        }

                        ret = window.Children;
                    }
                }
                else
                {
                    window.PostSerial(parent);

                    if (renameWindow)
                    {
                        RenameWindow(window, parent, renameWindow);
                    }

                    ret.Add(window);
                }
            }

            // 刷新界面
            RefreshContolsTree();

            return ret;
        }

        private void SaveToFile(Window wndToFile)
        {
            try
            {
                string defalultFileName = Path.GetFileNameWithoutExtension(lastFileName);
                using (VirtualFileDialog dlg = new VirtualFileDialog(false, defalultFileName, DefaultExt))
                {
                    dlg.Text = "Save GUI file...";
                    if (dlg.ShowDialog(this) == DialogResult.OK)
                    {
                        NResourceLoc loc = dlg.GetResourceLocation();

                        // 序列化到文件
                        WindowManager.Instance.Serialize(loc.PackageName, loc.FileName, wndToFile);

                        this.Text = "GUI Editor - " + loc.PackageName + ":" + loc.FileName;
                    }
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Generate ui project faild!");
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveToFile(GUISystem.Instance.RootWindow);
            ClearUndoRedoStack();
        }

        /// <summary>
        ///  新建一个新的UI 工程
        /// </summary>
        private void NewUIProject()
        {
            bool showCheck = GUISystem.Instance.RootWindow.ChildCount > 0;
            if (showCheck &&
                MessageBox.Show(NexusEditor.Properties.Resources.NewUIFile, NexusEditor.Properties.Resources.NewUIFileTips, MessageBoxButtons.YesNo) == DialogResult.No)
            {
                return;
            }

            // 清空当前选择窗口
            ClearCurrentSelectWindow();

            // 清空所有的窗口
            GUISystem.Instance.RootWindow.RemoveChildren();

            // 新建的工程没有存放路径
            this.Text = "GUI Editor - ";
            this.lastFileName = string.Empty;

            RefreshContolsTree();

            // 清空redo undo队列
            ClearUndoRedoStack();
        }

        private void ClearUndoRedoStack()
        {
            // 清空redo undo队列
            UndoRedoCommands.UndoCommands.Clear();
            UndoRedoCommands.RedoCommands.Clear();
            RefreshUndoRedoStat();
        }
        private void RefreshUndoRedoStat()
        {
            bool redo = UndoRedoCommands.IsRedoPossible();
            bool undo = UndoRedoCommands.IsUndoPossible();
            this.redoToolStripMenuItem.Enabled = redo;
            this.toolStripRedo.Enabled = redo;
            this.undoToolStripMenuItem.Enabled = undo;
            this.toolStripUndo.Enabled = undo;
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            NewUIProject();
        }
        private void toolStripNew_Click(object sender, EventArgs e)
        {
            NewUIProject();
        }

        /// <summary>
        /// 删除选中控件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripDelete_Click(object sender, EventArgs e)
        {
            DeleteSelectWindows();
        }

        private void deleteToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            DeleteSelectWindows();
        }

        private void DeleteSelectWindows()
        {
            foreach (Window wnd in CurrentSelectWindows)
            {
                if (wnd.Parent != null)
                {
                    CommandDeleteWindow command = new CommandDeleteWindow(wnd);
                    command.Execute();
                    UndoRedoCommands.UndoCommands.Push(command);
                    //wnd.Parent.RemoveChild(wnd);
                }
            }
            ClearCurrentSelectWindow();

            RefreshUndoRedoStat();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void toolStripOpen_Click(object sender, EventArgs e)
        {
            NewUIProject();
            OpenFromFile();
        }

        private void toolStripSave_Click(object sender, EventArgs e)
        {
            SaveToFile(GUISystem.Instance.RootWindow);
            ClearUndoRedoStack();
        }


        internal void LoadUI(string pkgName, string fileName)
        {
            NewUIProject();
            ReloadResource(GUISystem.Instance.RootWindow, pkgName, fileName, false);
            this.Text = "GUI Editor - " + pkgName + ":" + fileName;
            this.lastFileName = fileName;
        }

        private void toolStripCopy_Click(object sender, EventArgs e)
        {
            // 将控件copy到文件
            CopyCurrentSelectWindow();
        }

        private bool CopyCurrentSelectWindow()
        {
            if (CurrentSelectWindows.Count == 0)
            {
                return false;
            }
            if (CurrentSelectWindows.Count > 1)
            {
                MessageBox.Show("只能拷贝一个窗体！");
                return false;
            }

            try
            {
                Window copyWnd = CurrentSelectWindows[0];
                WindowManager.Instance.Serialize(CopyWindowResourceName.PackageName, CopyWindowResourceName.FileName, copyWnd);
                return true;
            }
            catch (System.Exception ex)
            {
                Program.ShowException(ex, "拷贝控件失败！");
            }
            return false;
        }
        private void toolStripPaste_Click(object sender, EventArgs e)
        {
            Window prentWindow = CurrentSelectWindows.Count == 0 ? GUISystem.Instance.RootWindow : CurrentSelectWindows[CurrentSelectWindows.Count - 1];
            PasteWindows(prentWindow);
        }

        private WindowCollection PasteWindows(Window prentWindow)
        {
            if (!CopyWindowResourceName.IsValid())
            {
                return null;
            }

            try
            {
                // 默认复制到最后一个选择的窗口
                return ReloadResource(prentWindow, CopyWindowResourceName.PackageName, CopyWindowResourceName.FileName, true);
            }
            catch (System.Exception ex)
            {
                Program.ShowException(ex, "粘贴控件失败！");
            }
            return null;
        }

        private void toolStripCut_Click(object sender, EventArgs e)
        {
            CutWindows();
        }

        private void CutWindows()
        {
            if (CopyCurrentSelectWindow())
            {

                Window copyWnd = CurrentSelectWindows[0];
                if (copyWnd.Parent != null)
                {
                    copyWnd.Parent.RemoveChild(copyWnd);
                }
                ChangeCurrentSelectWindow();
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 将控件copy到文件
            CopyCurrentSelectWindow();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CutWindows();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Window prentWindow = CurrentSelectWindows.Count == 0 ? GUISystem.Instance.RootWindow : CurrentSelectWindows[CurrentSelectWindows.Count - 1];
            PasteWindows(prentWindow);
        }

        void PointCheckHit(Window TestWnd, Vector2 pt, ref WindowCollection ret)
        {
            if (TestWnd == null) return;

            if (TestWnd.Rect.IsPointInRect(pt))
            {
                ret.Add(TestWnd);

                foreach (var item in TestWnd.Children)
                {
                    PointCheckHit(item, pt, ref ret);
                }
            }
        }

        private void selectWindowsToolStripMenuItem_DropDownOpening(object sender, EventArgs e)
        {
            Vector2 CursorPos = this.guiRenderControl.LastMouseDownPosition;
            this.selectWindowsToolStripMenuItem.DropDownItems.Clear();
            WindowCollection testRets = new WindowCollection();
            PointCheckHit(GUISystem.Instance.RootWindow, CursorPos, ref testRets);

            foreach (var item in testRets)
            {
                ToolStripMenuItem m = new ToolStripMenuItem("Select " + item.Name, null, new EventHandler(selectWindowsToolStripMenuDropDownItem_Click));
                m.Tag = item;
                this.selectWindowsToolStripMenuItem.DropDownItems.Add(m);
            }
        }

        private void selectWindowsToolStripMenuDropDownItem_Click(object sender, EventArgs e)
        {
            ToolStripMenuItem item = sender as ToolStripMenuItem;
            Window wnd = item.Tag as Window;

            if (wnd != null)
            {
                this.CurrentSelectWindows.Clear();
                this.AddCurrentSelectWindow(wnd);
            }
        }

        private void leftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 左对齐所选择的控件
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimX command = new CommandSetWindowDimX(item, new UIDim(0, 0));
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void rightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 右对齐所选择的控件
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimX command = new CommandSetWindowDimX(item, new UIDim(1, -item.AbsoluteSize.x));
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void centerToolStripMenuItemHC_Click(object sender, EventArgs e)
        {
            // 水平居中对齐所选择的控件
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimCenterX command = new CommandSetWindowDimCenterX(item);
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void horizontalFullToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 水平填充
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimFullX command = new CommandSetWindowDimFullX(item);
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void verticalFullToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 垂直填充
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimFullY command = new CommandSetWindowDimFullY(item);
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void topToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 居上对齐所选择的控件
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimY command = new CommandSetWindowDimY(item, new UIDim(0, 0));
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void bottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            // 居下对齐所选择的控件
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimY command = new CommandSetWindowDimY(item, new UIDim(1, -item.AbsoluteSize.y));
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void centerToolStripMenuItemVC_Click(object sender, EventArgs e)
        {
            // 垂直居中对齐所选择的控件
            foreach (var item in CurrentSelectWindows)
            {
                CommandSetWindowDimCenterY command = new CommandSetWindowDimCenterY(item);
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void setPositionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CurrentSelectWindows.Count == 0) return;

            using (UISetWindowPosition dlg = new UISetWindowPosition())
            {
                Vector2 wndOldPos = GUISystem.Instance.ScreenToClient(CurrentSelectWindows[0].AbsolutePosition);
                dlg.LeftPosition = wndOldPos.x;
                dlg.TopPosition = wndOldPos.y;
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    Vector2 wndNewPos = GUISystem.Instance.ClientToScreen(new Vector2(dlg.LeftPosition, dlg.TopPosition));
                    foreach (var item in CurrentSelectWindows)
                    {
                        CommandSetWindowPosition command = new CommandSetWindowPosition(item, wndNewPos);
                        command.Execute();
                        UndoRedoCommands.UndoCommands.Push(command);
                    }
                    RefreshUndoRedoStat();
                }
            }
        }

        private void setSizeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (CurrentSelectWindows.Count == 0) return;

            using (UISetWindowSize dlg = new UISetWindowSize())
            {
                Vector2 wndOldSize = CurrentSelectWindows[0].AbsoluteSize;
                dlg.WindowWidth = wndOldSize.x;
                dlg.WindowHeight = wndOldSize.y;
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    Vector2 wndNewSize = new Vector2(dlg.WindowWidth, dlg.WindowHeight);
                    foreach (var item in CurrentSelectWindows)
                    {
                        CommandSetWindowSize command = new CommandSetWindowSize(item, wndNewSize);
                        command.Execute();
                        UndoRedoCommands.UndoCommands.Push(command);
                    }
                    RefreshUndoRedoStat();
                }
            }
        }

        private void copyToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            CopyCurrentSelectWindow();
        }

        private void cutToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            CutWindows();
        }

        private void pasteToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            WindowCollection wnds = PasteWindows(GUISystem.Instance.GetTargetWindow(this.guiRenderControl.LastMouseDownPosition));
            if (wnds == null)
            {
                return;
            }

            foreach (var item in wnds)
            {
                CommandSetWindowPosition command = new CommandSetWindowPosition(item, this.guiRenderControl.LastMouseDownPosition);
                command.Execute();
                UndoRedoCommands.UndoCommands.Push(command);
            }
            RefreshUndoRedoStat();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void toolStripEditor_Click(object sender, EventArgs e)
        {
            CurrentMode = UIEditorMode.Editor;
        }

        private void toolStripPreview_Click(object sender, EventArgs e)
        {
            CurrentMode = UIEditorMode.Preview;
        }

        private void editorToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CurrentMode = UIEditorMode.Editor;
        }

        private void previewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            CurrentMode = UIEditorMode.Preview;
        }

        #region InjectInput
        internal void InjectEditorMouseDown(MouseEventArgs e)
        {

        }

        internal void InjectEditorMouseUp(MouseEventArgs e)
        {
            if (CurrentMode == UIEditorMode.Preview)
                return;
        }

        internal void InjectEditorMouseMove(MouseEventArgs e)
        {
            if (CurrentMode == UIEditorMode.Preview)
                return;

            Vector2 deltaPos = this.guiRenderControl.DeltaMouseMove;

            if (e.Button == MouseButtons.Left)
            {
                foreach (var item in CurrentSelectWindows)
                {
                    if (GUISystem.Instance.Modifiers == Keys.Alt)
                    {
                        CommandSetWindowSize command = new CommandSetWindowSize(item, item.AbsoluteSize + deltaPos);
                        command.Execute();
                        UndoRedoCommands.UndoCommands.Push(command);
                    }
                    else if (GUISystem.Instance.Modifiers == Keys.Shift)
                    {
                        CommandSetWindowPosition command = new CommandSetWindowPosition(item, item.AbsolutePosition + deltaPos);
                        command.Execute();
                        UndoRedoCommands.UndoCommands.Push(command);
                    }
                }
                RefreshUndoRedoStat();
            }
        }

        internal void InjectEditorMouseWheel(MouseEventArgs e)
        {
        }

        internal void InjectEditorMouseClick(MouseEventArgs e)
        {
            if (CurrentMode == UIEditorMode.Editor)
            {
                if (GUISystem.Instance.Modifiers != Keys.Control)
                {
                    CurrentSelectWindows.Clear();
                }
                AddCurrentSelectWindow(GUISystem.Instance.GetTargetWindow(this.guiRenderControl.LastMouseClickPosition));
            }
        }

        internal void InjectEditorMouseDoubleClick(MouseEventArgs e)
        {
        }

        internal void InjectKeyPress(KeyPressEventArgs e)
        {
        }

        internal void InjectKeyUp(KeyEventArgs e)
        {
        }

        internal void InjectKeyDown(KeyEventArgs e)
        {
            Vector2 deltaMove = Vector2.Zero;
            if (e.KeyCode == Keys.Left)
            {
                deltaMove.x = -1;
            }
            if (e.KeyCode == Keys.Right)
            {
                deltaMove.x = 1;
            }
            if (e.KeyCode == Keys.Up)
            {
                deltaMove.y = -1;
            }
            if (e.KeyCode == Keys.Down)
            {
                deltaMove.y = 1;
            }

            if (deltaMove != Vector2.Zero)
            {
                foreach (var item in CurrentSelectWindows)
                {
                    if (GUISystem.Instance.Modifiers == Keys.Alt)
                    {
                        CommandSetWindowSize command = new CommandSetWindowSize(item, item.AbsoluteSize + deltaMove);
                        command.Execute();
                        UndoRedoCommands.UndoCommands.Push(command);
                    }
                    else
                    {
                        CommandSetWindowPosition command = new CommandSetWindowPosition(item, item.AbsolutePosition + deltaMove);
                        command.Execute();
                        UndoRedoCommands.UndoCommands.Push(command);
                    }
                }
                RefreshUndoRedoStat();
            }
        }
        #endregion

        #region UndoRedo
        private void toolStripRedo_Click(object sender, EventArgs e)
        {
            UndoRedoCommands.Redo(1);

            RefreshUndoRedoStat();
        }

        private void toolStripUndo_Click(object sender, EventArgs e)
        {
            UndoRedoCommands.Undo(1);
            RefreshUndoRedoStat();
        }

        private void undoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            UndoRedoCommands.Undo(1);
            RefreshUndoRedoStat();
        }

        private void redoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            UndoRedoCommands.Redo(1);
            RefreshUndoRedoStat();
        }
        #endregion

        private void listViewStyle_ItemDrag(object sender, ItemDragEventArgs e)
        {
            int nTotalSel = listViewStyle.SelectedItems.Count;
            if (nTotalSel <= 0)
                return;

            IEnumerator SelCol = listViewStyle.SelectedItems.GetEnumerator();
            SelCol.MoveNext();

            ListViewItem lvi = SelCol.Current as ListViewItem;
            listViewStyle.DoDragDrop(lvi.Tag, DragDropEffects.Copy | DragDropEffects.Move);
        }

        private void buttonStyle_Click(object sender, EventArgs e)
        {
            using (LevelLocDlg dlg = new LevelLocDlg())
            {
                dlg.Text = "Load UI Style ...";
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    RelocationStytlePath(dlg.ResourceLocation);
                }
            }
        }

        private void RelocationStytlePath(NResourceLoc loc)
        {
            using (NexusEngineExtension.NWaitCursor wc = new NexusEngineExtension.NWaitCursor(this))
            {
                // 便利该目录中的所有Style *.nui文件
                using (UIFileQurery qurery = new UIFileQurery(this.listViewStyle, loc.PackageName, loc.FileName))
                {
                    this.textBoxStyle.Text = loc.ToString();
                }
            }
        }

        internal void CreateStyleWindow(NFileEntity entity, Vector2 screenPosition)
        {
            ReloadResource(GUISystem.Instance.GetTargetWindow(screenPosition),
                entity.PackageName, entity.FilePath, true);
        }

        private void refreshUIStyleListToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RelocationStytlePath(new NResourceLoc(this.textBoxStyle.Text.Trim()));
        }

        #region StringDictEditor
        private void gUIStringDictToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenStringDictEditor();
        }

        private void OpenStringDictEditor()
        {
            if (m_stringDictLoad)
                return;

            guiStringDictEditor = new ResXEditor(NFileSystem.DefaultFileSystemRoot + GUISystem.Instance.DefauleStringDictFilePath);
            guiStringDictEditor.Show();
            m_stringDictLoad = true;

            guiStringDictEditor.FormClosed += new FormClosedEventHandler(guiStringDictEditor_FormClosed);
            guiStringDictEditor.ResXFileSaved += new EventHandler<EventArgs>(guiStringDictEditor_ResXFileSaved);
        }

        void guiStringDictEditor_ResXFileSaved(object sender, EventArgs e)
        {
            GUISystem.Instance.ReloadStringDictionary();
        }

        void guiStringDictEditor_FormClosed(object sender, FormClosedEventArgs e)
        {
            guiStringDictEditor = null;
            m_stringDictLoad = false;
        }
        private void toolStripButtonStringDict_Click(object sender, EventArgs e)
        {
            OpenStringDictEditor();
        }
        #endregion

    }
}
