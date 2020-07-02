using System;
using System.Windows.Forms;
using System.Drawing;
using NexusEngine;

namespace NexusEditor
{
    /// <summary>
    /// 主窗口中Terrain Tab Page中的相关功能实现
    /// </summary>
    partial class EditorMainForm
    {
        private NTerrainBrush m_brush;
        private EditorMain.TerrainSplattingLayerForm m_splatDlg;

        /// <summary>
        /// 初始化, 绑定事件
        /// </summary>
        private void TerrainPageLoad()
        {
            //-- Brush panel
            foreach (ETrnBrushOP op in Enum.GetValues(typeof(ETrnBrushOP)))
                this.comboBoxTrnOp.Items.Add(op);
            this.comboBoxTrnOp.SelectedIndex = 0;

            foreach (ETrnBrushShape sp in Enum.GetValues(typeof(ETrnBrushShape)))
                this.comboBoxTrnShape.Items.Add(sp);
            this.comboBoxTrnShape.SelectedIndex = 0;

            foreach (ETrnLerpMode lp in Enum.GetValues(typeof(ETrnLerpMode)))
                this.comboBoxTrnLerp.Items.Add(lp);
            this.comboBoxTrnLerp.SelectedIndex = 1;

            //-- target panel
            foreach (ETrnBurshTarget t in Enum.GetValues(typeof(ETrnBurshTarget)))
                this.comboBoxTrnTarget.Items.Add(t);
            this.comboBoxTrnTarget.SelectedIndex = 0;

            BuildTargetList();

            //-- events
            this.comboBoxTrnOp.SelectedIndexChanged += new System.EventHandler(this.BrushChanged);
            this.comboBoxTrnLerp.SelectedIndexChanged += new System.EventHandler(this.BrushChanged);
            this.comboBoxTrnShape.SelectedIndexChanged += new System.EventHandler(this.BrushChanged);

            this.trackBarTrnInner.Scroll += new System.EventHandler(this.BrushChanged);
            this.trackBarTrnOutter.Scroll += new System.EventHandler(this.BrushChanged);
            this.trackBarTrnStrength.Scroll += new System.EventHandler(this.BrushChanged);

            this.buttonTrnNoise.Click += new System.EventHandler(this.buttonTrnNoise_Click);
            this.btnTrnTools.Click += new System.EventHandler(this.btnTrnTools_Click);
            this.btnTrnBasicMtl.Click += new System.EventHandler(this.btnTrnBasicMtl_Click);
            this.btnTrnSplatting.Click += new System.EventHandler(this.btnTrnSplatting_Click);
            this.listViewTrnTarget.SelectedIndexChanged += new System.EventHandler(this.listViewTrnTarget_SelectedIndexChanged);
            this.comboBoxTrnTarget.SelectedIndexChanged += new System.EventHandler(this.comboBoxTrnTarget_SelectedIndexChanged);

            this.listViewTrnTarget.DoubleClick += new System.EventHandler(this.listViewTrnTarget_DoubleClick);

            //-- sub forms 
            m_splatDlg = new NexusEditor.EditorMain.TerrainSplattingLayerForm();
            m_splatDlg.LayerApplied += new EventHandler(splatDlg_LayerApplied);
            NLevelEditorEngine.Instance.TerrainEd.TerranActorBinded += new EventHandler(this.TerrainBinded);
        }

        /// <summary>
        /// 响应引擎的绑定Terrain对象事件, 更新相关界面的数据
        /// </summary>        
        void TerrainBinded(object sender, EventArgs e)
        {
            BuildTargetList();
        }

        /// <summary>
        /// 响应地形图层对话框应用图层事件, 更新相关界面
        /// </summary>        
        void splatDlg_LayerApplied(object sender, EventArgs e)
        {
            BuildTargetList();
        }

        /// <summary>
        /// 根据界面控件设定更新m_brush的值
        /// </summary>
        private void UpdateBrush()
        {
            m_brush = new NTerrainBrush();
            m_brush.op = (ETrnBrushOP)this.comboBoxTrnOp.SelectedItem;
            m_brush.shape = (ETrnBrushShape)this.comboBoxTrnShape.SelectedItem;
            m_brush.lerpMode = (ETrnLerpMode)this.comboBoxTrnLerp.SelectedItem;
            m_brush.innerRadius = this.trackBarTrnInner.Value;
            m_brush.outerRadius = this.trackBarTrnOutter.Value;
            m_brush.strength = this.trackBarTrnStrength.Value;
            m_brush.noiseFreq = int.Parse(this.textBoxTrnFreq.Text);
            m_brush.target = (ETrnBurshTarget)this.comboBoxTrnTarget.SelectedItem;

            if (this.listViewTrnTarget.SelectedIndices.Count <= 0)
            {
                m_brush.targetIndex = 0;
            }
            else
            {
                m_brush.targetIndex = this.listViewTrnTarget.SelectedIndices[0];
            }

            NLevelEditorEngine.Instance.TerrainEd.SetBrush(m_brush);
        }

        /// <summary>
        /// [Brush]更新Brush值的事件，绑定到多个控件的操作响应接口
        /// </summary>        
        private void BrushChanged(object sender, EventArgs e)
        {
            UpdateBrush();
        }        

        /// <summary>
        /// 显示噪音生成对话框
        /// </summary>        
        private void buttonTrnNoise_Click(object sender, EventArgs e)
        {
            using (EditorMain.TerrainNoiseDlg dlg = new EditorMain.TerrainNoiseDlg())
            {
                dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// [Brush]操作目标的Target List View事件响应
        /// </summary>        
        private void listViewTrnTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateBrush();
        }


        /// <summary>
        /// 显示Terrain工具对话框
        /// </summary>        
        private void btnTrnTools_Click(object sender, EventArgs e)
        {
            using (EditorMain.TerrainToolsDlg dlg = new EditorMain.TerrainToolsDlg())
            {
                dlg.ShowDialog(this);
            }
        }        

        /// <summary>
        /// 显示基本材质创建对话框
        /// </summary>        
        private void btnTrnBasicMtl_Click(object sender, EventArgs e)
        {
            using (EditorMain.TerrainBasicMtlDlg dlg = new EditorMain.TerrainBasicMtlDlg())
            {
                dlg.ShowDialog(this);
            }
        }

        /// <summary>
        /// 显示多层材质创建对话框
        /// </summary>        
        private void btnTrnSplatting_Click(object sender, EventArgs e)
        {
            using (EditorMain.TerrainSplattingMtlDlg dlg = new EditorMain.TerrainSplattingMtlDlg())
            {
                dlg.ShowDialog(this);
                BuildTargetList();
            }
        }

        /// <summary>
        /// 取得当前地形的图层,更新列表
        /// </summary>
        private void BuildTargetList()
        {
            this.listViewTrnTarget.SuspendLayout();
            this.listViewTrnTarget.BeginUpdate();
            this.listViewTrnTarget.Items.Clear();

            //-- todo: clear ImageList

            System.Drawing.Size imgSize = this.imageListTrnTarget.ImageSize;
            //-- build target list
            switch (m_brush.target)
            {
                case ETrnBurshTarget.Heightmap:
                    this.listViewTrnTarget.Items.Add("Terrain Heightmap", "Heightmap");
                    break;
                case ETrnBurshTarget.Alphamap:
                    {
                        NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
                        NResourceLoc loc = new NResourceLoc();
                        Vector2 scale = new Vector2();
                        float rotate = 0;

                        for (int i = 0; i < NTerrainEditor.MaxLayers; i++)
                        {
                            trnEd.SplatGetLayer((uint)i, ref loc, ref scale, ref rotate);
                            string txt = string.Format("[{0}]{1}",
                                i, loc.FileName);                            

                            if (loc.IsValid())
                            {
                                IntPtr hbmp = NLevelEditorEngine.Instance.CreateThumbnail(
                                    loc, imgSize.Width, imgSize.Height);
                                String thumbName = loc.ToString();
                                this.imageListTrnTarget.Images.Add(thumbName, Image.FromHbitmap(hbmp));

                                this.listViewTrnTarget.Items.Add(txt, thumbName);
                            }
                            else 
                            {
                                this.listViewTrnTarget.Items.Add(txt, "NoTexture");
                            }
                        }
                    }
                    break;
                case ETrnBurshTarget.Decomap:
                    this.listViewTrnTarget.Items.Clear();
                    break;
            }

            this.listViewTrnTarget.EndUpdate();
            this.listViewTrnTarget.ResumeLayout();
        }

        /// <summary>
        /// Brush操作对象ComboBox选择更改
        /// </summary>        
        private void comboBoxTrnTarget_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateBrush();
            BuildTargetList();
        }

        /// <summary>
        /// 双击List,显示Height Map, Alpha Map, Deco Layer属性, 可编辑它们
        /// </summary>        
        private void listViewTrnTarget_DoubleClick(object sender, EventArgs e)
        {
            UpdateBrush();

            switch (m_brush.target)
            { 
                case ETrnBurshTarget.Heightmap:
                    MessageBox.Show("TODO : Show Heightmap");
                    break;
                case ETrnBurshTarget.Alphamap:
                    m_splatDlg.BindLayer((uint)m_brush.targetIndex);
                    m_splatDlg.Show(this);                        
                    break;
                case ETrnBurshTarget.Decomap:
                    MessageBox.Show("TODO : Show Deco Layer");
                    break;
            }
        }       
    }
}