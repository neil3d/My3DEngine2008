namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngineExtension;
    using NexusEngine;

    /// <summary>
    /// 用于逻辑层做2D绘图用
    /// </summary>
    internal sealed class GameHud : Singleton<GameHud>
    {
        public ScreenDamageListener DamageListener { get; private set; }

        private GameHud()
        {
        }

        public override bool Initialize(params object[] args)
        {
            DamageListener = new ScreenDamageListener();

            return base.Initialize(args);
        }

        /// <summary>
        /// 使用NUICanvas类绘图回调接口
        /// </summary>
        /// <param name="canvas"></param>
        public void DrawHud(NUICanvas canvas)
        {
            if (DamageListener != null)
            {
                DamageListener.DrawElement(canvas);
            }
        }

        /// <summary>
        /// 使用NRenderPrimitiveDrawInterface绘图的回调接口
        /// </summary>
        /// <param name="pdi"></param>
        public void DrawHud(NRenderPrimitiveDrawInterface pdi)
        {

        }

        /// <summary>
        /// Update
        /// </summary>
        public void Update(float deltaTime, NViewport view)
        {
            if (DamageListener != null)
            {
                DamageListener.UpdateElement(deltaTime, view);
            }
        }

    }
}
