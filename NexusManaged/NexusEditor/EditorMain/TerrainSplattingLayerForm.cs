using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 为地形对象的分层材质编辑某个图层
    /// </summary>
    public partial class TerrainSplattingLayerForm : Form
    {
        private int m_layerIndex;
        private NETerrainTextureLayerProp m_layer;
        public event EventHandler LayerApplied;

        public TerrainSplattingLayerForm()
        {
            InitializeComponent();            
        }

        public NETerrainTextureLayerProp Data
        {
            get { return m_layer; }
        }

        public int LayerIndex
        {
            get { return m_layerIndex; }
        }

        private void buttonApplyLayerProperty_Click(object sender, EventArgs e)
        {
            if (LayerApplied != null)
                LayerApplied(this, new EventArgs());
            this.Close();
        }

        public void BindLayer(int layer)
        { 
            m_layerIndex = layer;

            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("当前地形编辑器没有绑定对象.");
                this.Close();
                return;
            }

            NTerrainMtlSetup mtl = trnEd.GetMaterial();

            m_layer.data = mtl.GetLayerData(layer);

            this.propertyGridLayer.SelectedObject = m_layer;
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void TerrainSplattingLayerForm_Load(object sender, EventArgs e)
        {
            m_layer = new NETerrainTextureLayerProp();
            m_layerIndex = 0;

            this.propertyGridLayer.SelectedObject = m_layer;
        }
    }
}
