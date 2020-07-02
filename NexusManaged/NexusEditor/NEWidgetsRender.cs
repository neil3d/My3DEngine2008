using System;
using System.Collections.Generic;

using NexusEngine;

namespace NexusEditor
{
    /// <summary>
    /// 绘制基本的editor界面元素
    /// </summary>
    public class NEWidgetsRender : NRenderElement
    {
        private List<NEViewport> m_viewports;

        public NEWidgetsRender()
        {
            m_viewports = new List<NEViewport>();
        }

        public void AddViewport(NEViewport vp)
        {
            m_viewports.Add(vp);
        }

        public void RemoveViewport(NEViewport vp)
        {
            m_viewports.Remove(vp);
        }

        public override void Draw(NRenderPrimitiveDrawInterface PDI)
        {
            foreach (NEViewport vp in m_viewports)
            {
                if (vp.Visible)
                    vp.DrawWidgets(PDI);
            }
        }
    }

    class NELevelEditorWidgetsRender : NEWidgetsRender
    {
        public override void Draw(NRenderPrimitiveDrawInterface PDI)
        {
            base.Draw(PDI);
            NLevelEditorEngine.Instance.DrawWidgets(PDI);
            NLevelEditorEngine.Instance.ActorEd.DrawActorHandlesHitProxy(PDI, false);
        }

        public override void UpdateHitProxy(NRenderPrimitiveDrawInterface PDI)
        {
            NLevelEditorEngine.Instance.ActorEd.DrawActorHandlesHitProxy(PDI, true);
        }
    }
}
