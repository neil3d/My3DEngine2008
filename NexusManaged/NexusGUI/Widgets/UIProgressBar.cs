using System;
using System.ComponentModel;
using NexusEngine;
using Nexus.GUI.Components;
using System.Runtime.Serialization;
using System.Security.Permissions;
using NexusEngineExtension;


namespace Nexus.GUI.Widgets
{
    [XmlClassSerializable("UIProgressBar", true)]
    public class UIProgressBar : Nexus.GUI.Window
    {
        /// <summary>
        ///		Triggered when the progress changes.
        /// </summary>
        public event EventHandler<WindowEventArgs> ProgressChanged;

        /// <summary>
        ///		Triggered when the progress reaches 100%.
        /// </summary>
        public event EventHandler<WindowEventArgs> ProgressDone;

        /// <summary>
        ///		Current progress (from 0 to 1)
        /// </summary>
        [XmlFieldSerializable("progress")]
        protected float progress;
        /// <summary>
        ///		Amount to 'step' progress by on a call to <see cref="Step"/>.
        /// </summary>
        [XmlFieldSerializable("step")]
        protected float step;

        /// <summary>
        /// display text
        /// </summary>
        [XmlFieldSerializable("textComponent")]
        protected UIComp_DrawText textComponent;
        /// <summary>
        /// foreground image
        /// </summary>
        [XmlFieldSerializable("foreground")]
        protected UIComp_DrawTextureAtlas foreground;
        /// <summary>
        /// background image display
        /// </summary>
        [XmlFieldSerializable("background")]
        protected UIComp_DrawTextureAtlas background;
        /// <summary>
        /// 进度条呈现方式
        /// </summary>
        public enum ProgressBarPresent
        {
            /// <summary>
            /// 水平方式
            /// </summary>
            Horizontal,
            // 垂直方式
            Vertical,
        };
        [XmlFieldSerializable("style")]
        protected ProgressBarPresent style;

        #region Properties
        [Category("Present")]
        public ProgressBarPresent Style
        {
            get { return style; }
            set
            {
                style = value;
                RefreshForeground();
            }
        }
        /// <summary>
        /// Display text component
        /// </summary>
        [Category("UI Component")]
        public UIComp_DrawText Text
        {
            get { return textComponent; }
        }

        /// <summary>
        /// Display foreground image component
        /// </summary>
        [Category("UI Component")]
        public UIComp_DrawTextureAtlas Foreground
        {
            get { return foreground; }
        }

        /// <summary>
        /// Display background image component
        /// </summary>
        [Category("UI Component")]
        public UIComp_DrawTextureAtlas Background
        {
            get { return background; }
        }

        /// <summary>
        ///		Get/Set the current level of progress.
        /// </summary>
        /// <remarks>
        ///		If this value is > 1.0f (100%) progress will be limited to 1.0f.
        /// </remarks>
        /// <value>Value between 0.0f and 1.0f indicating current progress.</value>
        [Category("Present")]
        public float Progress
        {
            get
            {
                return progress;
            }
            set
            {
                // legal progress range is : 0.0f <= progress <= 1.0f
                float newProgress = (value < 0.0f) ? 0.0f : (value > 1.0f) ? 1.0f : value;

                if (newProgress != progress)
                {
                    // update progress and fire off event.
                    progress = newProgress;

                    RefreshForeground();


                    OnProgressChanged(new WindowEventArgs(this));

                    // if new progress is 100%, fire off the 'done' event as well.
                    if (progress == 1.0f)
                    {
                        OnProgressDone(new WindowEventArgs(this));
                    }
                }
            }
        }


        /// <summary>
        ///		Get/Set the size of the 'step' (in percentage points).
        /// </summary>
        /// <value>Default is 0.01f or 1%.</value>
        [Category("Present")]
        public float StepSize
        {
            get
            {
                return step;
            }
            set
            {
                // legal step range is : 0.0f <= step <= 1.0f
                step = (value < 0.0f) ? 0.0f : (value > 1.0f) ? 1.0f : value;
            }
        }

        #endregion Properties

        public UIProgressBar() { } //default ctor not valid - we want to enforce initializing our data

        public UIProgressBar(string name)
            : base(name)
        {
            step = 0.01f;

            textComponent = new UIComp_DrawText(this);
            textComponent.TextFormat |= NexusEngine.DrawTextFormat.NoClip;
            background = new UIComp_DrawTextureAtlas(this);
            foreground = new UIComp_DrawTextureAtlas(this);

            background.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            background.AtlasItemName = @"progress_back";

            foreground.ResourceAtlasLocation = new NexusEngine.NResourceLoc(@"engine_data:/ui/Atlas/engine_data1.txa");
            foreground.AtlasItemName = @"progress";
        }

        protected override void DrawSelf()
        {
            background.Draw();
            foreground.Draw();
            
            textComponent.Draw();
        }

        #region Methods

        /// <summary>
        ///		Modify the progress level by a specified delta.
        /// </summary>
        /// <param name="delta">
        ///		Amount to adjust the progress by.  Whatever this value is, the progress of the bar will be kept
        ///		within the range: [0.0f, 1.0f]
        /// </param>
        public void AdjustProgress(float delta)
        {
            Progress = progress + delta;
        }

        /// <summary>
        ///		Cause the progress to step.
        /// </summary>
        /// <remarks>
        ///		The amount the progress bar will step can be changed via the <see cref="StepSize"/> property.  The
        ///		default step size is 0.01f which is equal to 1%.
        /// </remarks>
        public void Step()
        {
            Progress = progress + step;
        }

        private void RefreshForeground()
        {
            // 计算前景色区域
            if (foreground != null)
            {
                if (Style == ProgressBarPresent.Horizontal)
                {
                    foreground.Width = new UIDim(0, this.Rect.Width * progress);
                    foreground.Height = new UIDim(1.0f, 0.0f);
                }
                else
                {
                    foreground.Height = new UIDim(0, this.Rect.Height * progress);
                    foreground.Width = new UIDim(1.0f, 0.0f);
                }
            }
        }

        #endregion Methods

        /// <summary>
        ///		Event triggered when progress changes.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected internal virtual void OnProgressChanged(WindowEventArgs e)
        {
            RequestRedraw();
            if (ProgressChanged != null)
            {
                ProgressChanged(this, e);
            }
        }

        /// <summary>
        ///		Event triggered when progress is complete.
        /// </summary>
        /// <param name="e">Event arguments.</param>
        protected internal virtual void OnProgressDone(WindowEventArgs e)
        {
            if (ProgressDone != null)
            {
                ProgressDone(this, e);
            }
        }

        #region Serialization

        protected override void PostCustomSerial(Window _parent)
        {
            base.PostCustomSerial(_parent);

            this.textComponent.PostCustomSerial(this);
            this.background.PostCustomSerial(this);
            this.foreground.PostCustomSerial(this);
        }
        protected override void OnStringDictionaryChanged()
        {
            this.textComponent.OnStringDictionaryChanged();
        }
        #endregion
    }
}
