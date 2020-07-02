using System;
using System.Collections.Generic;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor
{
    /// <summary>
    /// 处理主窗口上ToolBar中多个Command按钮的互斥选中状态
    /// </summary>
    partial class EditorMainForm
    {
        private List<ToolStripButton> m_btnList;

        /// <summary>
        /// 初始化, 创建command button列表, 绑定事件
        /// </summary>
        private void CommandToolStripLoad()
        { 
            m_btnList = new List<ToolStripButton>();
            m_btnList.Add(this.toolBtnPick);
            m_btnList.Add(this.toolBtnCamera);
            m_btnList.Add(this.toolBtnTerrainEd);
            m_btnList.Add(this.toolBtnNavEditor);
            m_btnList.Add(this.toolBtnTranslate);
            m_btnList.Add(this.toolBtnRotate);
            m_btnList.Add(this.toolBtnScale);
            m_btnList.Add(this.toolBtnPanView);
            m_btnList.Add(this.toolBtnRotateView);

            CheckCommandButton(this.toolBtnPick);

            int i = 0;
            foreach (ToolStripButton btn in m_btnList)
            {
                btn.Tag = (EditorCommand)i;
                i++;
                btn.Click += new EventHandler(btnCommand_Click);
                btn.CheckedChanged += new EventHandler(btnCommand_Checked);
            }
        }

        /// <summary>
        /// 单击选中某个Command Button, 并取消其他Button的选中状态;
        /// 更新编辑器当前的Command
        /// </summary>        
        void btnCommand_Click(object sender, EventArgs e)
        {
            ToolStripButton clickBtn = sender as ToolStripButton;
            CheckCommandButton(clickBtn);         
        }

        /// <summary>
        /// 设置那个Button为Checked状态
        /// </summary>        
        private void CheckCommandButton(ToolStripButton btn)
        { 
            foreach (ToolStripButton ibtn in m_btnList)
            {
                ibtn.Checked = (ibtn == btn);
            }
        }

        void btnCommand_Checked(object sender, EventArgs e)
        {
            ToolStripButton clickBtn = sender as ToolStripButton;
            if (!clickBtn.Checked)
                return;

            EditorCommand cmd = (EditorCommand)clickBtn.Tag;

            bool ok = true;
            if (cmd == EditorCommand.TerrainMode)
            {
                ok = NLevelEditorEngine.Instance.BindSelectedActorToTerrainEd();
                if (!ok)
                    NexusEditor.Program.ShowError("Please Select a Terrain First!");
            }

            // 开启或者关闭导航图编辑模式
            if( cmd == EditorCommand.NavigateMapMode)
            {
                NLevelEditorEngine.Instance.NavigateMapEd.BeginNavigateMapEditor(NLevelEditorEngine.Instance.MainLevel);
            }
            else
            {
                NLevelEditorEngine.Instance.NavigateMapEd.EndNavigateMapEditor();
            }

            if (ok)
            {                
                NLevelEditorEngine.Instance.Command = cmd;
            }           
        }
    }
}