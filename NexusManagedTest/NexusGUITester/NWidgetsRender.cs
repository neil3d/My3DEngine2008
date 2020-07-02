using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using NexusEngine;

namespace Nexus.GUI.Tester
{
    /// <summary>
    /// 2D 界面&debug info 渲染 
    /// </summary>
    public class NWidgetsRender :  NRenderElement
    {
        private List<NRenderControl> m_renderControls = new List<NRenderControl>();
        private NUICanvas m_uiCanvas;

        public NUICanvas UICanvas
        {
            get { return m_uiCanvas; }
        }

        public NWidgetsRender()
        {
            NRenderResourceManager renderResMgr = NEngine.Instance().RenderResManager;
            m_uiCanvas = renderResMgr.NewUICanvas();
        }

        public void AddRenderControl( NRenderControl control )
        {
            m_renderControls.Add(control);
        }

        public void RemoveRenderControl(NRenderControl control)
        {
            m_renderControls.Remove(control);
        }

        public void Clear()
        {
            m_renderControls.Clear();
        }

        public override void Draw(NRenderPrimitiveDrawInterface PDI)
        {
            foreach ( NRenderControl control in m_renderControls )
            {
                if( control.Visible )
                {
                    m_uiCanvas.Begin();
                    control.DrawWidgets(m_uiCanvas);
                    m_uiCanvas.End();
                    control.DrawWidgets(PDI);
                }
            }
        }
    }
}
