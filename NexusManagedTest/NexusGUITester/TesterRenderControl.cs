using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using Nexus.GUI;
using Nexus.GUI.Widgets;

namespace Nexus.GUI.Tester
{
    public class TesterRenderControl : NRenderControl
    {
        private FontDescription     m_deafultFontDesc;
        private NRenderFont         m_defaultFont;
        private NResourceTexture2D  m_defaultTexture;
        private string m_debugString;

        public TesterRenderControl()
        {
        }

        public override void PostInitialize()
        {
            //-- 创建缺省字体
            m_deafultFontDesc = new FontDescription();
            m_deafultFontDesc.FaceName = "宋体";
            m_deafultFontDesc.Height = 24;
            m_deafultFontDesc.Weight = FontWeight.Normal;
            m_defaultFont = NFontManager.Instance.ConstructFont(m_deafultFontDesc);

            m_defaultTexture = NResourceManager.Instance.LoadResourceTexture2D(
                new NResourceLoc("demo_data", "ui/Texture_Icon.tga"), EResourceIOMode.Auto,EResourceIOPriority.Normal);

            UILabel testLabel = WindowManager.Instance.CreateWindow("Nexus.GUI.Widgets.UILabel", "testUILabel1", GUISystem.Instance.RootWindow) as UILabel;
            testLabel.X = new UIDim(0,50);
            testLabel.Y = new UIDim(0,50);
            testLabel.Width = new UIDim(0, 300);
            testLabel.Height = new UIDim(0, 300);
            testLabel.TextRenderComp.Text = "这是一个测试Label控件";
            testLabel.TextRenderComp.TextFormat = DrawTextFormat.Center | DrawTextFormat.WordBreak;
            //testLabel.BackgroundRenderComp.Texture = m_defaultTexture;
            //testLabel.BackgroundRenderComp.TextureRect = testLabel.Rect;
            testLabel.BackgroundRenderComp.TextureAtlas = NResourceManager.Instance.LoadTextureAtlas(
                new NResourceLoc("demo_data", "ui/Atlas/demo_data1.txa"), EResourceIOMode.Auto, EResourceIOPriority.Normal );
            //testLabel.BackgroundRenderComp.TargetRect = testLabel.Rect;
            testLabel.BackgroundRenderComp.AtlasItemName = "Texture_Icon";

            testLabel.MouseButtonDown += new System.Windows.Forms.MouseEventHandler(testLabel_MouseButtonDown);
            testLabel.MouseClicked +=new System.Windows.Forms.MouseEventHandler(testLabel_MouseClicked);
            testLabel.MouseMove += new System.Windows.Forms.MouseEventHandler(testLabel_MouseMove);
        }

        protected void testLabel_MouseButtonDown(Object sender, System.Windows.Forms.MouseEventArgs e)
        {
            m_debugString = "testLabel_MouseButtonDown " + e.Button.ToString();
        }
        protected void testLabel_MouseClicked(Object sender, System.Windows.Forms.MouseEventArgs e)
        {
            m_debugString = "testLabel_MouseClicked " + e.Button.ToString() + e.Clicks;
        }
        protected void testLabel_MouseMove(Object sender, System.Windows.Forms.MouseEventArgs e)
        {
            m_debugString = "testLabel_MouseMove ";
        }
        /// <summary>
        /// Engine render element callback
        /// </summary>
        /// <param name="PDI"></param>
        override public void DrawWidgets(NRenderPrimitiveDrawInterface PDI)
        {
            PDI.DrawDebugString(2, 2, "Welcome Nexus GUI tester application", new Color4ub(255, 150, 150, 255));
            // TODO self draw
        }

        /// <summary>
        /// 使用<see cref="NUICanvas"/>渲染2D界面
        /// </summary>
        /// <param name="Canvas"></param>
        override public void DrawWidgets(NUICanvas Canvas)
        {
            base.DrawWidgets(Canvas);

            Canvas.DrawString(m_defaultFont, m_debugString, 100, 100, new Color4f(1.0f, 0.5f, 1.0f, 0.7f));

            Canvas.DrawTexture(m_defaultTexture, new Rect(0, 0, 32/1024, 32/1024), new Rect(300, 300, 400, 400), new ColorRect(-1));
        }
    }
}
