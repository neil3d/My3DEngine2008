using System;
using System.Windows.Forms;
using System.Drawing;
using NexusEngine;
using System.Collections.Generic;
using NexusEditor.EditorMain;

namespace NexusEditor
{
    /// <summary>
    /// 主窗口中Terrain Tab Page中的相关功能实现
    /// </summary>
    partial class EditorMainForm
    {
        private NTerrainBrush m_brush;
        
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

            this.numericUpDownInner.ValueChanged += new System.EventHandler(this.BrushChanged);
            this.numericUpDownOutter.ValueChanged += new System.EventHandler(this.BrushChanged);
            this.numericUpDownStrength.ValueChanged += new System.EventHandler(this.BrushChanged);

            this.buttonTrnNoise.Click += new System.EventHandler(this.buttonTrnNoise_Click);
            this.btnTrnTools.Click += new System.EventHandler(this.btnTrnTools_Click);

            this.btnTrnAddLayer.Click += new EventHandler(btnTrnAddLayer_Click);
            this.btnTrnRemoveLayer.Click += new EventHandler(btnTrnRemoveLayer_Click);
            this.btnTrnLayerUp.Click += new EventHandler(btnTrnLayerUp_Click);
            this.btnTrnLayerDown.Click += new EventHandler(btnTrnLayerDown_Click);

            this.checkBoxShowChunk.CheckedChanged += new EventHandler(checkBoxShowChunk_CheckedChanged);
            
            this.listViewTrnTarget.SelectedIndexChanged += new System.EventHandler(this.listViewTrnTarget_SelectedIndexChanged);
            this.comboBoxTrnTarget.SelectedIndexChanged += new System.EventHandler(this.comboBoxTrnTarget_SelectedIndexChanged);

            this.listViewTrnTarget.DoubleClick += new System.EventHandler(this.listViewTrnTarget_DoubleClick);

            this.btnTrnMtl.Click += new EventHandler(btnTrnMtl_Click);

            //-- sub forms             
            NLevelEditorEngine.Instance.TerrainEd.TerranActorBinded += new EventHandler(this.TerrainBinded);
        }

        void btnTrnMtl_Click(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
                return;
            }

            EditorMain.NewTerrainMaterial dlg = new NexusEditor.EditorMain.NewTerrainMaterial(
                NLevelEditorEngine.Instance.TerrainEd.GetTerrainBinded());
            dlg.Show(this);
        }

        void checkBoxShowChunk_CheckedChanged(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.TerrainEd.ShowChunkEdge(checkBoxShowChunk.Checked);
        }

        void btnTrnLayerDown_Click(object sender, EventArgs e)
        {
            if (m_brush.target == ETrnBurshTarget.Alphamap)
            {
                NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
                if (trnEd.Empty())
                {
                    NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
                    return;
                }

                NTerrainMtlSetup mtl = trnEd.GetMaterial();
                mtl.ShiftDownLayer(m_brush.targetIndex);
                this.BuildTargetList();
            }
        }

        void btnTrnLayerUp_Click(object sender, EventArgs e)
        {
            if (m_brush.target == ETrnBurshTarget.Alphamap)
            {
                NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
                if (trnEd.Empty())
                {
                    NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
                    return;
                }

                NTerrainMtlSetup mtl = trnEd.GetMaterial();
                mtl.ShiftUpLayer(m_brush.targetIndex);
                this.BuildTargetList();
            }
        }

        void btnTrnRemoveLayer_Click(object sender, EventArgs e)
        {

            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
                return;
            }

            if (m_brush.target == ETrnBurshTarget.Alphamap)
            {
                NTerrainMtlSetup mtl = trnEd.GetMaterial();
                mtl.RemoveLayer(m_brush.targetIndex);
                this.BuildTargetList();
            }
			else if (m_brush.target == ETrnBurshTarget.Decomap)
			{
				NTerrainDecoSetup deco = trnEd.GetTerrainBinded().GetDeco();
				deco.DeleteLayer(m_brush.targetIndex);
				this.BuildTargetList();
			}
        }

        void layerDialog_Add(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");                
                return;
            }

            EditorMain.TerrainSplattingLayerForm dlg = sender as EditorMain.TerrainSplattingLayerForm;
            EditorMain.NETerrainTextureLayerProp ld = dlg.Data;

            if (!ld.TextureRes.IsValid())
            {
                NexusEditor.Program.ShowError("添加图层失败：图层贴图路径为空.");
                return;
            }

            try
            {                
                NTerrainMtlSetup mtl = trnEd.GetMaterial();

                int i = mtl.AddLayer(ld.data);
                if (i == -1)
                {
                    NexusEditor.Program.ShowError("添加图层失败：可能是未创建地形材质.");
                }
                else
                {
                    this.BuildTargetList();
                }
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "地形材质图层更新失败！");
            }         
        }

	    void layerDialog_Update(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
                return;
            }

            EditorMain.TerrainSplattingLayerForm dlg = sender as EditorMain.TerrainSplattingLayerForm;
            EditorMain.NETerrainTextureLayerProp ld = dlg.Data;

            if (!ld.TextureRes.IsValid())
            {
                NexusEditor.Program.ShowError("更新图层失败：图层贴图路径为空.");
                return;
            }

            try
            {
                NTerrainMtlSetup mtl = trnEd.GetMaterial();
                mtl.SetLayerData(dlg.LayerIndex, ld.data);
                this.BuildTargetList();
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "地形材质图层更新失败！");
            }         
        }


        void btnTrnAddLayer_Click(object sender, EventArgs e)
        {
            if (m_brush.target == ETrnBurshTarget.Alphamap)
            {
                EditorMain.TerrainSplattingLayerForm dlg = new EditorMain.TerrainSplattingLayerForm();
                dlg.LayerApplied += new EventHandler(layerDialog_Add);
                dlg.Show(this);
            }
			else if (m_brush.target == ETrnBurshTarget.Decomap)
			{
				NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
				if (trnEd.Empty())
				{
					NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
					return;
				}
				using (CreateDecoLayerDialog dlg = new CreateDecoLayerDialog())
				{
					dlg.Text = "加载材质 ...";
					if (dlg.ShowDialog(this) == DialogResult.OK)
					{
						NTerrainDecoLayer layer = trnEd.GetTerrainBinded().GetDeco().AddLayer(dlg.LayerType, dlg.GridSize);
						if (layer != null)
						{
							layer.Name = dlg.LayerName;
							this.listViewTrnTarget.Items.Add(layer.Name, "NoTexture").Tag = layer;
						}
					}
				}
				
			}
        }

        /// <summary>
        /// 响应引擎的绑定Terrain对象事件, 更新相关界面的数据
        /// </summary>        
        void TerrainBinded(object sender, EventArgs e)
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
            m_brush.innerRadius = (int)this.numericUpDownInner.Value;
            m_brush.outerRadius = (int)this.numericUpDownOutter.Value;
            m_brush.strength = (int)this.numericUpDownStrength.Value;
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
            NLevelEditorEngine.Instance.NavigateMapEd.SetBrush(m_brush);
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
        /// 取得当前地形的图层,更新列表
        /// </summary>
        private void BuildTargetList()
        {
            this.listViewTrnTarget.Items.Clear();

            //--
            this.listViewTrnTarget.SuspendLayout();
            this.listViewTrnTarget.BeginUpdate();
            
            //-- todo: clear ImageList
			NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;

            System.Drawing.Size imgSize = this.imageListTrnTarget.ImageSize;
            //-- build target list
            switch (m_brush.target)
            {
                case ETrnBurshTarget.Heightmap:
                    this.listViewTrnTarget.Items.Add("高度图", "Heightmap");
                    this.btnTrnLayerDown.Enabled = false;
                    this.btnTrnLayerUp.Enabled = false;
                    this.btnTrnAddLayer.Enabled = false;
                    this.btnTrnRemoveLayer.Enabled = false;
                    break;
                case ETrnBurshTarget.Alphamap:
                    {
                        this.btnTrnLayerDown.Enabled = true;
                        this.btnTrnLayerUp.Enabled = true;
                        this.btnTrnAddLayer.Enabled = true;
                        this.btnTrnRemoveLayer.Enabled = true;
                        
                        if (!trnEd.Empty())
                        {
                            NTerrainMtlSetup mtl = trnEd.GetMaterial();

                            for (int i = 0; i < mtl.GetNumLayers(); i++)
                            {
                                LayerData ld = mtl.GetLayerData(i);
                                string txt = string.Format("[{0}]{1}",
                                    i, ld.DiffuseTexture.FileName);

                                if (ld.DiffuseTexture.IsValid())
                                {
                                    IntPtr hbmp = NLevelEditorEngine.Instance.CreateThumbnail(
                                        ld.DiffuseTexture, imgSize.Width, imgSize.Height);
                                    String thumbName = ld.DiffuseTexture.ToString();
                                    this.imageListTrnTarget.Images.Add(thumbName, Image.FromHbitmap(hbmp));

                                    this.listViewTrnTarget.Items.Add(txt, thumbName);
                                }
                                else
                                {
                                    this.listViewTrnTarget.Items.Add(txt, "NoTexture");
                                }
                            }// end of for()
                        }// end of if()
                    }
                    break;
                case ETrnBurshTarget.Decomap:
                    this.btnTrnLayerDown.Enabled = false;
                    this.btnTrnLayerUp.Enabled = false;
                    this.btnTrnAddLayer.Enabled = true;
                    this.btnTrnRemoveLayer.Enabled = true;
					if (!trnEd.Empty())
					{
						List<NTerrainDecoLayer> layers = trnEd.GetTerrainBinded().GetDeco().DecoLayers;
						for (int i = 0; i < layers.Count; ++i)
						{
							this.listViewTrnTarget.Items.Add(layers[i].Name, "NoTexture").Tag = layers[i];
						}
					}
					
					break;

                case ETrnBurshTarget.NavigationMap:
                case ETrnBurshTarget.WalkableMap:
                    ListViewItem li = new ListViewItem(NexusEngineExtension.Utility.GetEnumItemDescription(m_brush.target), "Heightmap");
                    li.Tag = m_brush.target;
                    this.listViewTrnTarget.Items.Add(li);
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
        /// 双击List,显示Height Map, Alpha Map, Deco Layer，Navigation Map属性, 可编辑它们
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
                    {
                        EditorMain.TerrainSplattingLayerForm dlg = new EditorMain.TerrainSplattingLayerForm();                        
                        dlg.LayerApplied += new EventHandler(layerDialog_Update);                       
                        dlg.Show(this);
                        dlg.BindLayer(m_brush.targetIndex);
                    }
                    break;
                case ETrnBurshTarget.Decomap:
					NTerrainDecoLayer layer=this.listViewTrnTarget.SelectedItems[0].Tag as NTerrainDecoLayer;
					if(layer==null) break;
					TerrainDecoLayerProperty tdp=new TerrainDecoLayerProperty();
					tdp.PropertyView.SelectedObject = layer;
					tdp.Show(this);
					this.listViewTrnTarget.SelectedItems[0].Text = layer.Name;
                    break;
                case ETrnBurshTarget.NavigationMap:
                case ETrnBurshTarget.WalkableMap:
                    // 显示导航图属性对话框
                    NavigationMapDlg navDesc = new NavigationMapDlg();
                    navDesc.Show();
                    break;
            }
        }       
    }
}