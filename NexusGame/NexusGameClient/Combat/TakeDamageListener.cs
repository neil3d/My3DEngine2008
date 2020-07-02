namespace NexusGameClient
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;
    using NexusEngine;

    #region ScreenDamageInfo
    public class ScreenDamageInfo
    {
        /// <summary>
        /// 谁的伤害信息
        /// </summary>
        public GameCharacterController TargetCC { get; private set; }

        /// <summary>
        /// 需要绘制的字符串是什么
        /// </summary>
        public string DamageMessage { get; private set; }

        /// <summary>
        /// 产生伤害时的时间戳
        /// </summary>
        public float StartTimeStamp { get; private set; }

        /// <summary>
        /// 总计显示了多长时间
        /// </summary>
        public float CountTime { get; set; }

        /// <summary>
        /// 初始化大小
        /// </summary>
        public Vector2 InitSize { get; set; }
        /// <summary>
        /// 缩放参数
        /// </summary>
        public float Scale { get; set; }

        /// <summary>
        /// 屏幕位置
        /// </summary>
        public Vector4 ScreenPosition { get; set; }

        private ScreenDamageInfo()
        {
        }

        public ScreenDamageInfo(GameCharacterController cc, string message)
        {
            TargetCC = cc;
            DamageMessage = message;
            StartTimeStamp = NativeEngineHelper.GetTimeSeconds();
            CountTime = 0;
            Scale = 1.0f;
        }
    }
    #endregion

    #region ScreenDamageListener
    /// <summary>
    /// 屏幕伤害数字显示
    /// </summary>
    public sealed class ScreenDamageListener
    {
        /// <summary>
        /// 控制移动的参数
        /// </summary>
        public readonly Vector2 DefaultMovement = new Vector2(0, -100);
        /// <summary>
        /// 控制缩放的参数
        /// </summary>
        public readonly Vector2 DefaultZoom = new Vector2(1, 1);

        public readonly float DefaultShowInfoTime = 2.2f;

        public readonly Color4f DefaultTextColor = new Color4f(0.68f, 0.09f, 0.01f);
        public readonly Color4f DefaultShadowColor = new Color4f(0.33f, 0.06f, 0.07f);

        private List<ScreenDamageInfo> damageInfos = new List<ScreenDamageInfo>();

        private NRenderFont font;

        #region Constructor
        public ScreenDamageListener()
        {
            FontDescription desc = FontDescription.DefaultFontDesc;
            desc.FaceName = "隶书";
            desc.Height = 24;
            font = NFontManager.Instance.ConstructFont(desc);
        }
        #endregion

        public void PushHitElement(GameCharacterController cc, S2C_SpellHit hit)
        {
            string message = FormatSpellHitMessage(hit);
            if (!string.IsNullOrEmpty(message))
            {
                ScreenDamageInfo info = new ScreenDamageInfo(cc, message);
                Rect fontRC = font.MeasureString(info.DamageMessage, DrawTextFormat.Center);
                info.InitSize = new Vector2(fontRC.Width, fontRC.Height);
                damageInfos.Add(info);
            }
        }

        private string FormatSpellHitMessage(S2C_SpellHit hit)
        {
            string ret = string.Empty;

            //if ((hit.Flag & (Byte)SpellHitResult.Hit) != 0)
            {
                ret = hit.Value.ToString();
            }

            return ret;
        }


        public void DrawElement(NUICanvas canvas)
        {
            if (font == null)
                return;

            foreach (var item in damageInfos)
            {
                Rect targetRC = new Rect(
                    item.ScreenPosition.x - item.InitSize.x * 0.5f,
                    item.ScreenPosition.y - item.InitSize.y * 0.5f,
                    item.ScreenPosition.x + item.InitSize.x * 0.5f,
                    item.ScreenPosition.y + item.InitSize.y * 0.5f);
                font.DrawString(item.DamageMessage, targetRC, item.ScreenPosition.z, item.Scale * (1.0f - item.ScreenPosition.w) * (1.0f - item.ScreenPosition.w), DrawTextFormat.Center, DefaultTextColor, DefaultShadowColor);
            }
        }

        public void UpdateElement(float deltaTime, NViewport vp)
        {
            int index = damageInfos.FindLastIndex(x => (x.CountTime > DefaultShowInfoTime));
            if (index >= 0)
                damageInfos.RemoveRange(0, index+1);

            foreach (var item in damageInfos)
            {
                item.CountTime += deltaTime;

                // 更新屏幕位置
                Vector4 screenLocation = vp.Camera.WorldToScreenWithDepth(item.TargetCC.Location);

                screenLocation.x += item.CountTime * item.CountTime * DefaultMovement.x;
                screenLocation.y += item.CountTime * item.CountTime * DefaultMovement.y;

                item.ScreenPosition = screenLocation;
            }
        }
    }
    #endregion
}
