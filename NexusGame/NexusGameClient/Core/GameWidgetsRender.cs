namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    using NexusEngine;

    public class GameWidgetsRender : NRenderElement
    {
        #region Fields

        private List<GameViewportControl> renderControls = new List<GameViewportControl>();

        #endregion Fields

        #region Constructors

        public GameWidgetsRender()
        {
        }

        #endregion Constructors

        #region Methods

        public void AddRenderControl( GameViewportControl control )
        {
            renderControls.Add(control);
        }

        public void Clear()
        {
            renderControls.Clear();
        }

        public override void Draw(NRenderPrimitiveDrawInterface PDI)
        {
            foreach ( GameViewportControl control in renderControls )
            {
                if (!control.Visible)
                    continue;

                NUICanvas guiCanvas = GameEngine.EngineInstance.GuiCanvas;
                if (guiCanvas != null)
                {
                    guiCanvas.Begin();
                    GameHud.Instance.DrawHud(guiCanvas);
                    control.DrawWidgets(guiCanvas);
                    guiCanvas.End();
                }
                GameHud.Instance.DrawHud(PDI);
                control.DrawWidgets(PDI);
            }
        }

        public void RemoveRenderControl(GameViewportControl control)
        {
            renderControls.Remove(control);
        }

        #endregion Methods
    }
}