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
        private uint m_layerIndex;
        private NETerrainTextureLayerProp m_layer;
        public event EventHandler LayerApplied;

        public TerrainSplattingLayerForm()
        {
            InitializeComponent();
            m_layer = new NETerrainTextureLayerProp();
            m_layerIndex = 0;
        }

        private void buttonApplyLayerProperty_Click(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("Terrain Actor NOT Bind.");
                this.Hide();
                return;
            }

            if (m_layer.TextureRes.ToString() == "None")
            {
                NexusEditor.Program.ShowError("Please Input Texture Resource Location.");
                return;
            }

            try
            {
                trnEd.SplatSetLayer(m_layerIndex,
                    m_layer.TextureRes, m_layer.UVScale, m_layer.UVRotate);
                if (LayerApplied != null)
                    LayerApplied(this, new EventArgs());
                this.Hide();
            }
            catch (System.Exception ex)
            {
                NexusEditor.Program.ShowException(ex, "Terrain Material Layer Set FAILED!");
            }         
        }

        public void BindLayer(uint layer)
        { 
            m_layerIndex = layer;

            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            NResourceLoc loc = new NResourceLoc();
            Vector2 scale = new Vector2();
            float rotate = 0;

            trnEd.SplatGetLayer((uint)layer, ref loc, ref scale, ref rotate);

            m_layer.TextureRes = loc;
            m_layer.UVRotate = rotate;
            m_layer.UVScale = scale;

            this.propertyGridLayer.SelectedObject = m_layer;
        }

        private void buttonClose_Click(object sender, EventArgs e)
        {
            this.Hide();
        }
    }
}
