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
    /// 为地形对象生成随机高度
    /// </summary>
    public partial class TerrainNoiseDlg : Form
    {
        private NETerrainNoise m_noiseGen;

        public TerrainNoiseDlg()
        {
            InitializeComponent();
        }

        private void TerrainNoiseDlg_Load(object sender, EventArgs e)
        {
            m_noiseGen = new NETerrainNoise();
            this.propertyGridNoise.SelectedObject = m_noiseGen;
        }


        /// <summary>
        /// Noise Generate按钮事件响应
        /// </summary>        
        /// <remarks>
        /// 根据所选择的Target-Target Index调用引擎接口，来产生随机Noise
        /// </remarks>
        private void buttonGenNoise_Click(object sender, EventArgs e)
        {
            NTerrainEditor trnEd = NLevelEditorEngine.Instance.TerrainEd;
            if (trnEd.Empty())
            {
                NexusEditor.Program.ShowError("Terrain Actor NOT Bind.");
                return;
            }

            switch (m_noiseGen.Target)
            {
                case ETrnBurshTarget.Heightmap:
                    trnEd.GenerateNoise(m_noiseGen.AreaRect, m_noiseGen.NumOctaves,
                        m_noiseGen.Amplitude, m_noiseGen.Frequency);
                    break;
                case ETrnBurshTarget.Alphamap:
                    trnEd.SplatLayerNoise(m_noiseGen.TargetIndex, m_noiseGen.AreaRect, m_noiseGen.NumOctaves,
                        m_noiseGen.Amplitude, m_noiseGen.Frequency);
                    break;
                case ETrnBurshTarget.Decomap:
                    break;
            }

        }
    }
}
