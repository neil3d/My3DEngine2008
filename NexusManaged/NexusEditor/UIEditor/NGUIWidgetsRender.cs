using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NexusEngine;

namespace NexusEditor.UIEditor
{
    /// <summary>
    /// 2D 界面&debug info 渲染 
    /// </summary>
    public class NGUIWidgetsRender :  NRenderElement
    {
        private List<NGUIRenderControl> m_renderControls = new List<NGUIRenderControl>();
        private NUICanvas m_uiCanvas;

        public NUICanvas UICanvas
        {
            get { return m_uiCanvas; }
        }

        public NGUIWidgetsRender()
        {
            NRenderResourceManager renderResMgr = NEngine.Instance().RenderResManager;
            m_uiCanvas = renderResMgr.NewUICanvas();
        }

        public void AddRenderControl( NGUIRenderControl control )
        {
            m_renderControls.Add(control);
        }

        public void RemoveRenderControl(NGUIRenderControl control)
        {
            m_renderControls.Remove(control);
        }

        public void Clear()
        {
            m_renderControls.Clear();
        }

        public override void Draw(NRenderPrimitiveDrawInterface PDI)
        {
            foreach ( NGUIRenderControl control in m_renderControls )
            {
                if( control.Visible )
                {
                    control.DrawWidgets(PDI);
                    m_uiCanvas.Begin();
                    control.DrawWidgets(m_uiCanvas);
                    m_uiCanvas.End();
                }
            }
        }
    }
}
