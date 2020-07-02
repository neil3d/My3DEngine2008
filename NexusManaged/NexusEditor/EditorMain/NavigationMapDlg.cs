using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;
using System.Drawing.Imaging;
using System.Diagnostics;

namespace NexusEditor
{

    public partial class NavigationMapDlg : Form
    {
        private NavigationMapDescription levelDesc;

        #region Nest class NavigationMapDescription

        private class NavigationMapDescription
        {
            public Vector3 Location { get; set; }
            public Vector3 Scale { get; set; }

            public UInt32 Width { get; set; }
            public UInt32 Height { get; set; }

            public float CullDistance { get; set; }

            //[Description(NexusEditor.Properties.Resources.HeightMapColor)]
            public System.Drawing.Color HeightMapColor { get; set; }

            //[Description(NexusEditor.Properties.Resources.WalkableColor)]
            public System.Drawing.Color WalkableColor { get; set; }

            /// <summary>
            /// 生成可行走bitmap用的最大高度范围
            /// </summary>
            public float MaxWalkableHeight { get; set; }
            /// <summary>
            /// 生成可行走bitmap用的步阶
            /// </summary>
            public float MaxWalkableStep { get; set; }

            private NavigationMapDescription()
            {
            }

            public NavigationMapDescription(NLevel hostLevel)
            {
                Location = hostLevel.NavigationMapLocation;
                Scale = hostLevel.NavigationMapScale;
                Width = hostLevel.NavigationMapWidth;
                Height = hostLevel.NavigationMapHeight;

                HeightMapColor = NLevelEditorEngine.Instance.NavigateMapEd.NavigationHeightMapColor.ToColor();

                CullDistance = NLevelEditorEngine.Instance.NavigateMapEd.CullDistance;

                MaxWalkableHeight = float.MaxValue;
                MaxWalkableStep = float.MaxValue;
            }
        }
        #endregion


        public NavigationMapDlg()
        {
            InitializeComponent();

            levelDesc = new NavigationMapDescription(NLevelEditorEngine.Instance.MainLevel);

            this.propertyGridNavMapDesc.SelectedObject = levelDesc;
        }

        private void AdapterTerrain_Click(object sender, EventArgs e)
        {
            NLevel host = NLevelEditorEngine.Instance.MainLevel;
            if (host != null)
            {
                // 找到地形的大小和位置
                using (NexusEngineExtension.NWaitCursor waitCursor = new NexusEngineExtension.NWaitCursor(this))
                {
                    NActor actor = host.FirstActor();
                    while (actor != null)
                    {
                        NTerrainActor terrain = actor as NTerrainActor;
                        if (terrain != null)
                        {
                            levelDesc.Location = terrain.Location;
                            levelDesc.Scale = terrain.Scale;
                            levelDesc.Width = Math.Max(levelDesc.Width, terrain.HeightMapWidth);
                            levelDesc.Height = Math.Max(levelDesc.Height, terrain.HeightMapHeight);

                            this.propertyGridNavMapDesc.SelectedObject = levelDesc;
                            break;
                        }

                        actor = host.NextActor();
                    }
                }
            }
        }

        private void ResetNavMap_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(NexusEditor.Properties.Resources.ResetNavMap,
                        NexusEditor.Properties.Resources.Ok,
                        MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                NLevelEditorEngine.Instance.NavigateMapEd.MoveNavigationMap(levelDesc.Location, levelDesc.Scale);
                NLevelEditorEngine.Instance.NavigateMapEd.ResetNavigationMap(levelDesc.Width, levelDesc.Height);
            }
        }

        private void ApplyDesc_Click(object sender, EventArgs e)
        {
            NLevelEditorEngine.Instance.NavigateMapEd.MoveNavigationMap(levelDesc.Location, levelDesc.Scale);
            NLevelEditorEngine.Instance.NavigateMapEd.ResizeNavigationMap(levelDesc.Width, levelDesc.Height);

            NLevelEditorEngine.Instance.NavigateMapEd.NavigationHeightMapColor = new Color4f(levelDesc.HeightMapColor);
            NLevelEditorEngine.Instance.NavigateMapEd.NavigationWalkableColor = new Color4f(levelDesc.WalkableColor);

            NLevelEditorEngine.Instance.NavigateMapEd.CullDistance = levelDesc.CullDistance;

            this.Hide();
        }

        private void Rebuild_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(NexusEditor.Properties.Resources.RebuildNavMapByLineCheck,
                        NexusEditor.Properties.Resources.Ok,
                        MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                using (NexusEngineExtension.NWaitCursor waitCursor = new NexusEngineExtension.NWaitCursor(this))
                {
                    NLevelEditorEngine.Instance.NavigateMapEd.RebuildNavigationMap(levelDesc.MaxWalkableHeight, levelDesc.MaxWalkableStep, true);
                }
            }
        }
        private void rebuildByActors_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show(NexusEditor.Properties.Resources.RebuildNavMap,
                    NexusEditor.Properties.Resources.Ok,
                    MessageBoxButtons.OKCancel) == DialogResult.OK)
            {
                using (NexusEngineExtension.NWaitCursor waitCursor = new NexusEngineExtension.NWaitCursor(this))
                {
                    NLevelEditorEngine.Instance.NavigateMapEd.RebuildNavigationMap(levelDesc.MaxWalkableHeight, levelDesc.MaxWalkableStep, false);
                }
            }
        }

        /// <summary>
        /// 保存位图到文件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void SaveBitmap_Click(object sender, EventArgs e)
        {
            NLevel level = NLevelEditorEngine.Instance.MainLevel;
            if (level == null
                || level.NavigationMapWidth <= 0
                || level.NavigationMapHeight <= 0
                )
            {
                Program.ShowError("Invalid level param!");
                return;
            }

            int map_w = (int)level.NavigationMapWidth;
            int map_h = (int)level.NavigationMapHeight;

            using (SaveFileDialog dlg = new SaveFileDialog())
            {
                dlg.Title = "Bitmap Image: Image Name, File Format, and Destination";
                dlg.FileName = "bitmap";
                dlg.DefaultExt = "jpg";
                dlg.AddExtension = true;
                dlg.Filter = @"Jpeg Image(JPG)|*.jpg|PNG Image|*.png|GIF Image (GIF)|*.gif|Bitmap (BMP)|*.bmp";
                dlg.FilterIndex = 1;
                if (dlg.ShowDialog(this) == DialogResult.OK)
                {
                    ImageFormat fmtStyle;
                    switch (dlg.FilterIndex)
                    {
                        case 2: fmtStyle = ImageFormat.Png; break;
                        case 3: fmtStyle = ImageFormat.Gif; break;
                        case 4: fmtStyle = ImageFormat.Bmp; break;
                        default: fmtStyle = ImageFormat.Jpeg; break;
                    }

                    try
                    {
                        Bitmap bitmap = new Bitmap(map_w, map_h);

                        // Loop through the images pixels to reset color.
                        for (int x = 0; x < map_w; x++)
                        {
                            for (int y = 0; y < map_h; y++)
                            {
                                if (level.GetTileWalkable(x,y))
                                {
                                    // 可行走的区域使用高度图颜色
                                    bitmap.SetPixel(x, y, levelDesc.HeightMapColor);
                                }
                                else
                                {
                                    // 不可行走区域
                                    bitmap.SetPixel(x, y, levelDesc.WalkableColor);
                                }
                            }
                        }

                        bitmap.Save(dlg.FileName, fmtStyle);

                        Process.Start(dlg.FileName);
                    }
                    catch (Exception)
                    {
                        try
                        { // try IE
                            Process.Start("iexplore.exe", dlg.FileName);
                        }
                        catch (Exception) { }
                    }
                }
            }
        }

    }
}
