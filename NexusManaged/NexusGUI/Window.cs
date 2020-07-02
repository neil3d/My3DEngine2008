using System;
using NexusEngine;
using System.ComponentModel;
using System.Runtime.Serialization;
using System.Security.Permissions;

using NexusEngineExtension;

namespace Nexus.GUI
{
    /// <summary>
    /// Nexus.GUI 窗口基类
    /// </summary>
    [XmlClassSerializable("Window")]
    public abstract class Window : IDisposable
    {
        #region Events
        [Category("Data")]
        public event EventHandler<TimePulseEventArgs> TimePulse;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> Sized;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> Moved;
        [Category("Data")]
        public event EventHandler<WindowEventArgs> TextChange;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> FontChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> AlphaChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> Shown;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> Hidden;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> Enabled;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> Disabled;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> ClippingChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> ParentDestroyChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> InheritsAlphaChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> RectChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> AlwaysOnTopChanged;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> CaptureGained;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> CaptureLost;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> RenderingStarted;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> RenderingEnded;
        [Category("Data")]
        public event EventHandler<GUIEventArgs> DestructionStarted;

        #region Window events
        [Category("Action")]
        public event EventHandler<WindowEventArgs> Activated;
        [Category("Action")]
        public event EventHandler<WindowEventArgs> Deactivated;
        [Category("Action")]
        public event EventHandler<WindowEventArgs> ParentSized;
        [Category("Action")]
        public event EventHandler<WindowEventArgs> ParentMoved;
        [Category("Action")]
        public event EventHandler<WindowEventArgs> ChildAdded;
        [Category("Action")]
        public event EventHandler<WindowEventArgs> ChildRemoved;
        #endregion

        #region  Mouse events
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseEnter;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseLeave;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseMove;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseWheel;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseButtonDown;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseButtonUp;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseClicked;
        [Category("Input")]
        public event EventHandler<GUIMouseEventArgs> MouseDoubleClicked;
        #endregion

        #region Key events
        [Category("Input")]
        public event System.Windows.Forms.KeyEventHandler KeyDown;
        [Category("Input")]
        public event System.Windows.Forms.KeyEventHandler KeyUp;
        [Category("Input")]
        public event System.Windows.Forms.KeyPressEventHandler KeyPress;
        #endregion

        #endregion

        #region Field

        /// <summary>
        /// GUI System unique name of this windows
        /// </summary>
        [XmlFieldSerializable("name")]
        protected string name;

        /// <summary>
        ///		Alpha transparency setting for the Window.
        /// </summary>
        [XmlFieldSerializable("alpha")]
        protected float alpha;


        /// <summary>
        /// 窗口对象区域绝对坐标
        /// </summary>
        [NonSerialized]
        protected Rect area;

        /// <summary>
        /// 窗口的位置
        /// </summary>
        [XmlFieldSerializable("x")]
        protected UIDim x;
        [XmlFieldSerializable("y")]
        protected UIDim y;

        /// <summary>
        /// 窗口像素大小
        /// </summary>
        [XmlFieldSerializable("width")]
        protected UIDim width;
        [XmlFieldSerializable("height")]
        protected UIDim height;

        /// <summary>
        /// 当前窗口的最大和最小大小，窗口的大小局限在[minSize,maxSize]之间
        /// </summary>
        [XmlFieldSerializable("minSize")]
        protected Vector2 minSize;
        [XmlFieldSerializable("maxSize")]
        protected Vector2 maxSize;

        /// <summary>
        ///	List of child windows attached to this window.
        /// </summary>
        [TypeConverter(typeof(WindowCollectionConverter))]
        [XmlFieldSerializable("children")]
        protected WindowCollection children = new WindowCollection();

        /// <summary>
        /// 记录父窗口引用
        /// </summary>
        [NonSerialized]
        protected Window parent;

        /// <summary>
        ///     Window that has captured inputs.
        /// </summary>
        [NonSerialized]
        protected static Window captureWindow;
        /// <summary>
        ///     Previous window to have mouse capture.
        /// </summary>
        [NonSerialized]
        protected Window oldCapture;

        [XmlFieldSerializable("isEnabled")]
        protected bool isEnabled;
        [XmlFieldSerializable("isVisible")]
        protected bool isVisible;
        [NonSerialized]
        protected bool isActive;
        [XmlFieldSerializable("isClippedByParent")]
        protected bool isClippedByParent;
        /// <summary>
        /// top-most window
        /// </summary>
        [XmlFieldSerializable("isAlwaysOnTop")]
        protected bool isAlwaysOnTop;
        [XmlFieldSerializable("isHeritsAlpha")]
        protected bool isHeritsAlpha;

        /// <summary>
        ///     True if window should restore any 'old' capture when it releases capture.
        /// </summary>
        [NonSerialized]
        protected bool restoreOldCapture;

        #endregion

        #region Constructor
        protected Window() { } //default ctor not valid - we want to enforce initializing our data

        protected Window(string name) 
        {
            this.name = name;

            alpha = 1.0f;
            isEnabled = true;
            isVisible = true;
            isHeritsAlpha = true;

            area = new Rect(0, 0, 0, 0);
            x = new UIDim(0, 0);
            y = new UIDim(0, 0);
            width = new UIDim(0, 0);
            height = new UIDim(0, 0);

            minSize = new Vector2(0, 0);
			maxSize = new Vector2(float.MaxValue, float.MaxValue);
        }

        ~Window()
        {
            Dispose(false);
        }
        #endregion

        #region Porperties

        /// <summary>
        ///     Returns a references to the window that currently has input capture, or null if none.
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never),Browsable(false)]
        public static Window CaptureWindow
        {
            get
            {
                return captureWindow;
            }
        }

        [Category("Appearance"),DefaultValue(false)]
        [Description("the current alpha value for this window.")]
        public bool InheritsAlpha
        {
            get { return this.isHeritsAlpha; }
            set
            {
                isHeritsAlpha = value;
                OnInheritsAlphaChanged(new GUIEventArgs());
            }
        }

        [Category("Behavior"),DefaultValue(false)]
        [Description("whether this window is 'always on top' or not.")]
        public bool AlwaysOnTop
        {
            get { return isAlwaysOnTop; }
            set
            {
                if (isAlwaysOnTop != value)
                {
                    isAlwaysOnTop = value;

                    // move us in front of sibling windows with the same 'always-on-top' setting as we have.
                    if (parent != null)
                    {
                        Window origParent = parent;

                        // remove the window from parents list
                        origParent.RemoveChildImpl(this);

                        // re-add window to parent, which will place it behind all top-most windows,
                        // which in either case is the right place for this window
                        origParent.AddChildImpl(this);
                    }
                }
                OnAlwaysOnTopChanged(new GUIEventArgs());
            }
        }

        /// <summary>
        ///     Returns true if this window is in a disabled state.
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Always), Browsable(true)]
        [Category("Behavior")]
        [Description("Returns true if this window is in a disabled state.")]
        public bool IsEnabled
        {
            get
            {
                bool parentEnabled =
                    (parent == null) ? true : parent.IsEnabled;

                return (isEnabled && parentEnabled);
            }
            set
            {
                if (isEnabled != value)
                {
                    isEnabled = value;

                    if (isEnabled)
                    {
                        OnEnabled(new GUIEventArgs());
                    }
                    else
                    {
                        OnDisabled(new GUIEventArgs());
                    }
                }
            }
        }

        [Browsable(false)]
        public bool IsDisabled
        {
            get
            {
                return !IsEnabled;
            }
            set
            {
                IsEnabled = !value;
            }
        }

        /// <summary>
        ///     Returns true if this window is visible (not hidden).
        /// </summary>
        [Category("Appearance"),DefaultValue(true)]
        [Description("true/false if this window is visible (not hidden).")]
        public bool Visible
        {
            get
            {
                bool parentVisible =
                    (parent == null) ? true : parent.Visible;

                return (isVisible && parentVisible);
            }
            set
            {
                if (isVisible != value)
                {
                    isVisible = value;

                    if (isVisible)
                    {
                        OnShown(new GUIEventArgs());
                    }
                    else
                    {
                        OnHidden(new GUIEventArgs());
                    }
                }
            }
        }

        /// <summary>
        ///     Returns true if this window is active. 
        /// </summary>
        /// <remarks>
        ///     The active window is always the front most window.
        /// </remarks>
        [EditorBrowsable(EditorBrowsableState.Never),Browsable(false)]
        public bool IsActive
        {
            get
            {
                bool parentActive =
                    (parent == null) ? true : parent.IsActive;

                return (isActive && parentActive);
            }
        }

        /// <summary>
        ///     Returns true if this window is to be clipped by it's parent.
        /// </summary>
        [Category("Appearance")]
        [Description("true/false this window is to be clipped by it's parent.")]
        public bool IsClippedByParent
        {
            get
            {
                return isClippedByParent;
            }
            set
            {
                if (isClippedByParent != value)
                {
                    isClippedByParent = value;
                    OnClippingChanged(new GUIEventArgs());
                }
            }
        }

        /// <summary>
        ///     Gets/Sets the width of the window (in unspecified units).
        /// </summary>
        [Category("Data")]
        [Description("the width of the window (in unspecified units).")]
        public UIDim Width
        {
            get
            {
                return width;
            }
            set
            {
                if( width != value )
                {
                    width = value;
                    OnSized(new GUIEventArgs());
                }
            }
        }

        /// <summary>
        ///     Gets/Sets the height of the window (in unspecified units).
        /// </summary>
        /// <remarks>
        ///		Interpretation of the value is dependent upon the current metrics system set for the Window.
        /// </remarks>
        [Category("Data"),DefaultValue(50)]
        [Description("the height of the window.")]
        public UIDim Height
        {
            get
            {
                return height;
            }
            set
            {
                if( height != value )
                {
                    height = value;
                    OnSized(new GUIEventArgs());
                }
            }
        }

        /// <summary>
        ///     Get/Set the x position of the window.
        /// </summary>
        /// <remarks>Interpretation of return value depends upon the metric type in use by this window.</remarks>
        /// <value>
        ///		float value that specifies the x position of the Window relative to it's parent, depending on the metrics system in use for this
        ///		Window, this value will specify either pixels or a decimal fraction of the width of the parent Window.
        /// </value>
        [Category("Data")]
        [Description("the x position of the window.")]
        public UIDim X
        {
            get
            {
                return x;
            }
            set
            {
                if( x != value )
                {
                    x = value;

                    OnMoved(new GUIEventArgs());
                }
            }
        }

        /// <summary>
        ///     Get/Set the y position of the window.
        /// </summary>
        /// <remarks>Interpretation of return value depends upon the metric type in use by this window.</remarks>
        /// <value>
        ///		float value that specifies the y position of the Window relative to it's parent, depending on the metrics system in use for this
        ///		Window, this value will specify either pixels or a decimal fraction of the width of the parent Window.
        /// </value>
        [Category("Data")]
        [Description("the y position of the window.")]
        public UIDim Y
        {
            get
            {
                return y;
            }
            set
            {
                if (y != value)
                {
                    y = value;

                    OnMoved(new GUIEventArgs());
                }
            }
        }

        /// <summary>
        ///	允许控件的最大大小
        /// </summary>
        [Category("Data")]
        [Description("the maximum size for this window")]
        public Vector2 MaximumSize
        {
            get
            {
                return maxSize;
            }
            set
            {
                maxSize = value;

                // 重新计算窗口区域
                InvalidateWindowRect();
            }
        }

        /// <summary>
        ///	允许控件的最小大小
        /// </summary>
        [Category("Data")]
        [Description("the minimum size for this window.")]
        public Vector2 MinimumSize
        {
            get
            {
                return minSize;
            }
            set
            {
                minSize = value;

                // 重新计算窗口区域
                InvalidateWindowRect();
            }
        }


        /// <summary>
        /// 设置窗口的绝对坐标
        /// </summary>
        [Browsable(false)]
        public Vector2 AbsolutePosition
        {
            get
            {
                return area.Position;
            }
            set
            {
                if( parent != null )
                {
                    x.offest = value.x - parent.Rect.left - parent.Rect.Width * x.scale;
                    y.offest = value.y - parent.Rect.top - parent.Rect.Height * y.scale;
                }
                else
                {
                    x.offest = value.x;
                    y.offest = value.y;
                }

                OnMoved(new GUIEventArgs());
            }
        }

        [Browsable(false)]
        public Vector2 AbsoluteSize
        {
            get 
            {
                return area.Size;
            }
            set
            {
                width.offest = value.x - (parent == null ? 0 : parent.Rect.Width) * width.scale;
                height.offest = value.y - (parent == null ? 0 : parent.Rect.Height) * height.scale;

                OnSized(new GUIEventArgs());
            }
        }

        /// <summary>
        ///		The name of this window.
        /// </summary>
        /// <value>The unique name of this window.</value>
        [Category("Action")]
        [Description("The name of this window.")]
        public string Name
        {
            get
            {
                return name;
            }
            set
            {
                if( GUISystem.Instance.RootWindow.FindChild(value, true) != null )
                {
                    throw new AlreadyExitsException("The window name {0} already exits.",value);
                }

                name = value;
            }
        }

        /// <summary>
        ///     Gets/Sets a reference to this window's parent window.
        /// </summary>
        [Category("Action")]
        [Description("this window's parent window.")]
        public Window Parent
        {
            get
            {
                return parent;
            }
        }

        /// <summary>
        ///     Gets the number of child windows attached to this window.
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never),Browsable(false)]
        public int ChildCount
        {
            get
            {
                return children.Count;
            }
        }

        /// <summary>
        /// get children
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public WindowCollection Children
        {
            get { return children; }
        }

        /// <summary>
        ///     Gets a reference to the top-most active child window starting at 'this'.
        /// </summary>
        /// <remarks>
        ///     Returns 'this' if it is 'this' window which is active.
        ///     Returns null if neither this window nor any children are active.
        /// </remarks>
        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public Window ActiveChild
        {
            get
            {
                // if this window is not active just return null, since it's children can't be
                // active if 'this' is not.
                if (!this.IsActive)
                {
                    return null;
                }

                // Travel through the child list(s) until we find the active window
                foreach( Window child in children )
                {
                    if (child.IsActive)
                    {
                        return child.ActiveChild;
                    }
                }

                // no child is active (or has no children), so return 'this' as active window
                return this;
            }
        }

        /// <summary>
        ///		Get/Set the current alpha value for this window.
        /// </summary>
        /// <remarks>
        ///		The alpha value set for any given window may or may not be the final alpha value that is used when rendering.  All window
        ///		objects, by default, inherit alpha from thier parent window(s) - this will blend child windows, relatively, down the line of
        ///		inheritance.  This behavior can be overridden via <see cref="InheritsAlpha"/>.  To return the true alpha value that will be
        ///		applied when rendering, use <see cref="EffectiveAlpha"/>.
        /// </remarks>
        [Category("Appearance"),DefaultValue(1.0f)]
        [Description("the current alpha value for this window.")]
        public float Alpha
        {
            get
            {
                return alpha;
            }
            set
            {
                alpha = value;
                OnAlphaChanged(new GUIEventArgs());
            }
        }

        /// <summary>
        ///		Gets the effective alpha value that will be used when rendering this window, taking into account inheritance of parent
        ///		window(s) alpha.
        /// </summary>
        /// <value>The effective alpha that will be applied to this Window when rendering.  Will be between 0.0f and 1.0f.</value>
        public float EffectiveAlpha
        {
            get
            {
                if ((parent == null) || (!this.isHeritsAlpha))
                {
                    return alpha;
                }

                return alpha * parent.EffectiveAlpha;
            }
        }

        /// <summary>
        ///		Gets a Rect object that describes the Window area.
        /// </summary>
        /// <value>
        ///		Rect that describes the area covered by the Window.  The values in the returned Rect are in whatever form is set
        ///		as the current metric type.  The returned Rect is unclipped and relative to the Window objects parent.
        /// </value>
        [Category("Data")]
        [Description("Rect object that describes the Window area.")]
        public Rect Rect
        {
            get
            {
                return area;
            }
        }

        /// <summary>
        ///     Returns true if input is captured by 'this'.
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never),Browsable(false)]
        public bool IsCapturedByThis
        {
            get
            {
                return captureWindow == this;
            }
        }

        /// <summary>
        ///     Returns true if input is captured by some ancestor of 'this'.
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public bool IsCapturedByAncestor
        {
            get
            {
                return IsAncestor(captureWindow);
            }
        }

        /// <summary>
        ///     Returns true if input is captured by a child of 'this'.
        /// </summary>
        [EditorBrowsable(EditorBrowsableState.Never), Browsable(false)]
        public bool IsCapturedByChild
        {
            get
            {
                return IsChild(captureWindow);
            }
        }

        #endregion

        #region Methods
        
        /// <summary>
        /// 窗口初始化
        /// </summary>
        public virtual void Initialize()
        {}

        /// <summary>
        /// 字符串Key/Value字典发生变化，需要窗体重新定位字符内容
        /// </summary>
        abstract protected void OnStringDictionaryChanged();
        /// <summary>
        /// 供外部通知的接口
        /// </summary>
        public void NotifyStringDictionaryChanged()
        {
            OnStringDictionaryChanged();

            foreach (Window child in children)
            {
                child.NotifyStringDictionaryChanged();
            }
        }

        /// <summary>
        ///  渲染自己和子窗口之前调用
        /// </summary>
        protected virtual void PreRender()
        {
        }
        /// <summary>
        /// 渲染自己和子窗口之后调用
        /// </summary>
        protected virtual void PostRender()
        {
        }

        /// <summary>
        /// 渲染自己之后,渲染子窗口之前调用
        /// </summary>
        protected virtual void PostDrawSelf()
        {
        }

        /// <summary>
        /// 渲染窗口
        /// </summary>
        public virtual void Render()
        {
            if( !Visible )
                return;

            PreRender();

            // 设置裁剪区域
            if (isClippedByParent && parent != null)
            {
                GUISystem.Instance.Renderer.PushScissorRect(this.parent.Rect);
            }
       
            // 渲染自己
            DrawSelf();

            // Forces all batched sprites to be submitted to the device. 
            GUISystem.Instance.Renderer.Flush();

            PostDrawSelf();

            if (isClippedByParent && parent != null)
            {
                GUISystem.Instance.Renderer.PopScissorRect();
            }

            // 渲染所有的子窗口
            foreach ( Window child in children )
            {
                child.Render();
            }

            PostRender();
        }

        /// <summary>
        /// 窗口自身的渲染
        /// </summary>
        protected abstract void DrawSelf();

        /// <summary>
        ///		Signal the System object to redraw (at least) this Window on the next render cycle.
        /// </summary>
        public void RequestRedraw()
        {
            GUISystem.Instance.SignalRedraw();
        }

        /// <summary>
        ///		Activate the Window giving it input focus and bringing it to the top of all non always-on-top Windows.
        /// </summary>
        /// <remarks>
        ///		A Window cannot be programmatically 'disabled', as such.  To disable a Window, you must activate another one.
        /// </remarks>
        public void Activate()
        {
            MoveToFront();
        }

        /// <summary>
        ///     Add the specified Window as a child of this Window.  If the Window \a window is already attached to a Window, it is detached before
        ///     being added to this Window.
        /// </summary>
        /// <param name="window">Reference to a window to add as a child.</param>
        public void AddChild(Window window)
        {
            AddChildImpl(window);
            OnChildAdded(new WindowEventArgs(window));
        }

        /// <summary>
        ///		Removes the child window with the specified name.
        /// </summary>
        /// <param name="name">Name of the child window to remove.</param>
        public void RemoveChild(string inName)
        {
            Window child = GetChild(inName);

            if (child != null)
            {
                RemoveChild(child);
            }
        }

        /// <summary>
        ///     Removes 'window' from this window's child list.
        /// </summary>
        /// <param name="window">Reference to a window to add as a child.</param>
        public void RemoveChild(Window window)
        {
            RemoveChildImpl(window);
            OnChildRemoved(new WindowEventArgs(window));
        }

        /// <summary>
        /// 移除所有的子窗口
        /// </summary>
        public void RemoveChildren()
        {
            children.Clear();

            // 参数为NULL表示全部移除
            OnChildRemoved(new WindowEventArgs(null));
        }

        /// <summary>
        ///		Return a reference to the child window with the specified name.
        /// </summary>
        /// <remarks>
        ///		This function will throw an exception if no child object with the given name is attached.  This decision
        ///		was made (over returning 'null' if no window was found) so that client code can assume that if the call
        ///		returns it has a valid window pointer.  We provide the <see cref="IsChild"/> functions for checking if a given window
        ///		is attached.
        /// </remarks>
        /// <param name="name">The name of the child window to return.</param>
        /// <returns>Window object attached to this window that has the specified <paramref name="name"/>.</returns>
        /// <exception cref="UnknownObjectException">Thrown if no window named <paramref name="name"/> is attached to this Window.</exception>
        public Window GetChild(string inName)
        {
            foreach (Window wnd in children)
            {
                if( wnd.Name == inName )
                {
                    return wnd;
                }
            }

            throw new UnknownObjectException("There is no child named '{0}' attached to window '{1}'", inName, this.Name);
        }

        /// <summary>
        ///     Returns the child Window that is 'hit' by the given position
        /// </summary>
        /// <param name="position">Point that describes the position to check in screen pixels.</param>
        /// <returns>Child located at the specified position, or null if none exists.</returns>
        public Window GetChildAtPosition(Vector2 position)
        {
            // scan child list backwards (Windows towards the end of the list are considered 'in front of' those towards the begining)
			for (int i = children.Count - 1; i >= 0; i--) 
            {
				Window child = children[i];
                // only check if the child is visible
                if (child.Visible)
                {
                    // recursively scan children of this child windows...
                    Window window = child.GetChildAtPosition(position);

                    // return window if we found a 'hit' down the chain somewhere
                    if (window != null)
                    {
                        return window;
                    }
                    // none of our children hit
                    else
                    {
                        if (child is Nexus.GUI.Widgets.GUISheet)
                            continue; // can't click a GuiSheet.
                        // see if this child is hit and return it if it is
                        if (child.IsHit(position))
                        {
                            return child;
                        }
                    }
                }
            }

            // nothing hit
            return null;
        }

        /// <summary>
        /// 根据名称查找子窗口
        /// </summary>
        /// <param name="inName">要查找的子控件的名称</param>
        /// <param name="recursion">是否递归迭代查找所有的子窗口，而不管窗口有多少层</param>
        /// <returns></returns>
        public Window FindChild(string inName, bool recursion)
        {
            Window ret = null;
            foreach (Window child in children)
            {
                if (child.Name.Equals(inName))
                {
                    ret = child;
                    break;
                }
                else if (recursion)
                {
                    // 迭代查找
                    ret = child.FindChild(inName, recursion);
                    if (ret != null)
                        break;
                }
            }
            return ret;
        }

        /// <summary>
        ///		returns whether a Window with the specified name is currently 
        ///		attached to this Window as a child.
        /// </summary>
        /// <param name="name">The name of the Window to look for.</param>
        /// <returns>True if a Window named <paramref name="name" /> is currently attached to this Window as a child, else false.</returns>
        public bool IsChild(string inName)
        {
            for (int i = 0; i < this.ChildCount; i++)
            {
                if (children[i].Name == inName)
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        ///     Returns true if <paramref name="window"/> is a child window of 'this'.
        /// </summary>
        /// <param name="window">Window to look for.</param>
        /// <returns>True if <paramref name="window"/> is a child of 'this'.</returns>
        public bool IsChild(Window window)
        {
            foreach (Window child in children)
            {
                if (child == window)
                {
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        ///     Returns true if the Window named <paramref name="name"/> is some ancestor of 'this'.
        /// </summary>
        /// <param name="name">Name of the Window to look for.</param>
        /// <returns>True if a Window named <paramref name="name"/> is an ancestor of 'this'.</returns>
        public bool IsAncestor(string inName)
        {
            // if parent is false, we have no ancestors
            if (parent == null)
            {
                return false;
            }

            // return true if our immediate parent is a match
            if (parent.name == inName)
            {
                return true;
            }
            else
            {
                // scan back up the line until we get a result
                return parent.IsAncestor(inName);
            }
        }

        /// <summary>
        ///     Returns true is <paramref name="window"/> is some ancestor of 'this'.
        /// </summary>
        /// <param name="window">Window to look for.</param>
        /// <returns>True if <paramref name="window"/> is an ancestor of 'this'.</returns>
        public bool IsAncestor(Window window)
        {
            // if parent is false, we have no ancestors
            if (parent == null)
            {
                return false;
            }

            // return true if our immediate parent is a match
            if (parent == window)
            {
                return true;
            }
            else
            {
                // scan back up the line until we get a result
                return parent.IsAncestor(window);
            }
        }

        /// <summary>
        ///     check if the given position would hit this window.
        /// </summary>
        /// <param name="position">Point describing the position to check in screen pixels.</param>
        /// <returns>True if the given point is within this window's area.</returns>
        public virtual bool IsHit(Vector2 position)
        {
            // if window is disabled, default is not to react
            if (!IsEnabled || !Visible)
            {
                return false;
            }

            // 如果是被父窗口剪裁还需要判断父窗口
            if (isClippedByParent && parent != null)
            {
                return Rect.IsPointInRect(position) && parent.IsHit(position);
            }
            else
            {
                //return whether point is within this area
                return Rect.IsPointInRect(position);
            }
        }

        /// <summary>
        ///		Move the Window to the bottom of the Z order.
        /// </summary>
        /// <remarks>
        ///		- If the window is non always-on-top the Window is sent to the very bottom of its sibling windows and the process repeated for all ancestors.
        ///		- If the window is always-on-top, the Window is sent to the bottom of all sibling always-on-top windows and the process repeated for all ancestors.
        /// </remarks>
        public void MoveToBack()
        {
            // if the window is active, deactivate it
            if (IsActive)
            {
                OnDeactivated(new WindowEventArgs(this));
            }

            // if the window has no parent then we can have no siblings and have nothing more to do.
            if (parent == null)
            {
                return;
            }

            // move us behind all sibling windows with the same 'always-on-top' setting as we have.
            Window orgParent = parent;
            parent.RemoveChildImpl(this);

            int pos = 0;

            if (AlwaysOnTop)
            {
                while ((pos != ChildCount - 1) && (!children[pos].AlwaysOnTop))
                {
                    pos++;
                }
            }

            children.Insert(pos, this);
            parent = orgParent;

            parent.MoveToBack();
        }

        /// <summary>
        ///		Move the Window to the top of the z order.
        /// </summary>
        /// <remarks>
        ///		- If the Window is a non always-on-top window it is moved the the top of all other non always-on-top sibling windows, and the process
        ///		repeated for all ancestors.
        ///		- If the Window is an always-on-top window it is moved to the of of all sibling Windows, and the process repeated for all ancestors.
        /// </remarks>
        public void MoveToFront()
        {
            // // if the window has no parent then we can have no siblings and have nothing more to do.
            if (parent == null)
            {
                // perform initial activation if required
                if (!IsActive)
                {
                    OnActivated(new WindowEventArgs(null));
                }

                return;
            }

            // bring parent window to front of its siblings
            parent.MoveToFront();

            // get our sibling window which is currently active (if any)
            Window activeWindow = null;

            int idx = parent.ChildCount;

            while (--idx >= 0)
            {
                if (parent.children[idx].IsActive)
                {
                    activeWindow = parent.children[idx];
                    break;
                }
            }

            // move us infront of sibling windows with the same 'always-on-top' setting as we have.
            Window orgParent = parent;
            orgParent.RemoveChildImpl(this);
            orgParent.AddChildImpl(this);

            // notify ourselves that we have become active
            if (activeWindow != this)
            {
                OnActivated(new WindowEventArgs(activeWindow));
            }

            // notify previously active window that it is no longer active
            if ((activeWindow != null) && (activeWindow != this))
            {
                activeWindow.OnDeactivated(new WindowEventArgs(this));
            }
        }

        /// <summary>
        ///     Capture input to this window.
        /// </summary>
        public void CaptureInput()
        {
            // we can only capture if we are the active window
            if (!IsActive)
            {
                return;
            }

            Window currentCapture = captureWindow;
            captureWindow = this;

            // inform any window which previously had mouse that it doesn't anymore!
            if ((currentCapture != null) && (currentCapture != this) && (!restoreOldCapture))
            {
                currentCapture.OnCaptureLost(new GUIEventArgs());
            }

            if (restoreOldCapture)
            {
                oldCapture = currentCapture;
            }

            // event notification
            OnCaptureGained(new GUIEventArgs());
        }

        /// <summary>
        ///     Releases the capture of input from this window.
        /// </summary>
        /// <remarks>
        ///		If this Window does not have inputs captured, nothing happens.
        /// </remarks>
        public void ReleaseInput()
        {
            // if we are not the window that has capture, do nothing
            if (!IsCapturedByThis)
            {
                return;
            }

            // restore old captured window if that mode is set
            if (restoreOldCapture)
            {
                captureWindow = oldCapture;

                // check for case when there was no previously captured window
                if (oldCapture != null)
                {
                    oldCapture = null;
                    captureWindow.MoveToFront();
                }
            }
            else
            {
                captureWindow = null;
            }

            // event notification
            OnCaptureLost(new GUIEventArgs());
        }

        /// <summary>
        ///		Add given window to child list at an appropriate position.
        /// </summary>
        /// <param name="window">Window to add.</param>
        private void AddChildImpl(Window window)
        {
            bool prentChanged = window.parent != this;

            // if window is already attached, detach it first (will fire normal events)
            if (window.Parent != null)
            {
                window.Parent.RemoveChild(window);
            }

            int position = (ChildCount == 0) ? 0 : ChildCount;

            if (!window.AlwaysOnTop)
            {
                // find last non-topmost window
                while ((position != 0) && children[position - 1].AlwaysOnTop)
                {
                    position--;
                }
            }

            // add window at the end
            if (position == ChildCount)
            {
                children.Add(window);
            }
            else
            {
                // insert before position
                children.Insert(position, window);
            }

            window.parent = this;

            if (prentChanged)
            {
                // force an update for the area Rects for 'window' so they are correct for its new parent
                window.OnParentSized(new WindowEventArgs(this));
                window.OnAlphaChanged(new GUIEventArgs());
            }
        }

        /// <summary>
        ///		Remove given window from child list.
        /// </summary>
        /// <param name="window">Window to remove.</param>
        private void RemoveChildImpl(Window window)
        {
            children.Remove(window);
            window.parent = null;
        }

        /// <summary>
        ///		Gets the size of the specified window.
        /// </summary>
        /// <param name="window">Window to get the size for.</param>
        protected Vector2 GetWindowSizeImpl(Window window)
        {
            if (window == null)
            {
                return GUISystem.Instance.Renderer.GetViewPortSize();
            }
            else
            {
                return window.area.Size;
            }
        }

        protected virtual void InvalidateWindowRect()
        {
            // 重新计算窗口区域
            if (parent != null)
            {
                area.left = parent.Rect.left + parent.Rect.Width * x.scale + x.offest;
                area.top = parent.Rect.top + parent.Rect.Height * y.scale + y.offest;
                area.right = area.left + parent.Rect.Width * width.scale + width.offest;
                area.bottom = area.top + parent.Rect.Height * height.scale + height.offest;
            }
            else
            {
                area.left = x.offest;
                area.top = y.offest;
                area.right = area.left + width.offest;
                area.bottom = area.top + height.offest;
            }

            area.ConstrainSize(maxSize, minSize);

            if( RectChanged != null)
            {
                RectChanged(this,new GUIEventArgs());
            }
        }

        /// <summary>
        ///     Disable this window.
        /// </summary>
        public void Disable()
        {
            IsEnabled = false;
        }

        /// <summary>
        ///     Enable this window.
        /// </summary>
        public void Enable()
        {
            IsEnabled = true;
        }

        /// <summary>
        ///     Show this window (make visible).
        /// </summary>
        public void Show()
        {
            Visible = true;
        }
        /// <summary>
        ///     Hide this window.
        /// </summary>
        public void Hide()
        {
            Visible = false;
        }

        #region Trigger

        protected internal virtual void OnTimePulse(TimePulseEventArgs e)
        {
            if( TimePulse != null )
            {
                TimePulse(this,e);
            }

            foreach (Window child in children )
            {
                child.OnTimePulse(e);
            }
        }

        protected internal virtual void OnSized(GUIEventArgs e)
        {
            RequestRedraw();

            // 重新计算窗口区域
            InvalidateWindowRect();

            if( Sized != null )
            {
                Sized(this,e);
            }

            foreach (Window wnd in children)
            {
                wnd.OnParentSized(new WindowEventArgs(this));
            }
        }

        protected internal virtual void OnMoved(GUIEventArgs e)
        {
            RequestRedraw();

            // 重新计算窗口区域
            InvalidateWindowRect();

            // 响应事件
            if( Moved != null )
            {
                Moved(this, e);
            }

            foreach (Window wnd in children)
            {
                wnd.OnParentMoved(new WindowEventArgs(this));
            }
        }

        protected internal virtual void OnTextChanged(WindowEventArgs e)
        {
            RequestRedraw();

            if( TextChange != null )
            {
                TextChange(this, e);
            }
        }

        /// <summary>
        ///      Event trigger method for the <see cref="FontChanged"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnFontChanged(GUIEventArgs e)
        {
            RequestRedraw();

            if (FontChanged != null)
            {
                FontChanged(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="AlphaChanged"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnAlphaChanged(GUIEventArgs e)
        {
            // scan child list and call this method for all children that inherit alpha
            foreach( Window wnd in children )
            {
                if ( wnd.InheritsAlpha )
                {
                    wnd.OnAlphaChanged(e);
                }
            }

            RequestRedraw();

            if (AlphaChanged != null)
            {
                AlphaChanged(this, e);
            }
        }

        /// <summary>
        ///      Event trigger method for the <see cref="Shown"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnShown(GUIEventArgs e)
        {
            RequestRedraw();

            if (Shown != null)
            {
                Shown(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="Hidden"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnHidden(GUIEventArgs e)
        {
            RequestRedraw();

            if (Hidden != null)
            {
                Hidden(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="Enabled"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnEnabled(GUIEventArgs e)
        {
            RequestRedraw();

            if (Enabled != null)
            {
                Enabled(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="Disabled"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnDisabled(GUIEventArgs e)
        {
            RequestRedraw();

            if (Disabled != null)
            {
                Disabled(this, e);
            }
        }

        /// <summary>
        ///      Event trigger method for the <see cref="ClippingChanged"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnClippingChanged(GUIEventArgs e)
        {
            RequestRedraw();

            if (ClippingChanged != null)
            {
                ClippingChanged(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="ParentDestroyChanged"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnParentDestroyChanged(GUIEventArgs e)
        {
            if (ParentDestroyChanged != null)
            {
                ParentDestroyChanged(this, e);
            }
        }

        protected internal virtual void OnInheritsAlphaChanged(GUIEventArgs e)
        {
            RequestRedraw();

            if (InheritsAlphaChanged != null)
            {
                InheritsAlphaChanged(this, e);
            }
        }

        protected internal virtual void OnAlwaysOnTopChanged(GUIEventArgs e)
        {
            RequestRedraw();

            if (AlwaysOnTopChanged != null)
            {
                AlwaysOnTopChanged(this, e);
            }
        }

        /// <summary>
        ///      Event trigger method for the <see cref="CaptureGained"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnCaptureGained(GUIEventArgs e)
        {
            if (CaptureGained != null)
            {
                CaptureGained(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="CaptureLost"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnCaptureLost(GUIEventArgs e)
        {
            // todo:
            //// handle restore of previous capture window as required.
            //if (restoreOldCapture && (oldCapture != null))
            //{
            //    oldCapture.OnCaptureLost(e);
            //    oldCapture = null;
            //}

            //// handle case where mouse is now in a different window
            //// (this is a bit of a hack that uses the mouse input injector to handle this for us).
            //GUISystem.Instance.InjectMouseMove(0, 0);

            if (CaptureLost != null)
            {
                CaptureLost(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="RenderingStarted"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnRenderingStarted(GUIEventArgs e)
        {
            if (RenderingStarted != null)
            {
                RenderingStarted(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="RenderingEnded"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnRenderingEnded(GUIEventArgs e)
        {
            if (RenderingEnded != null)
            {
                RenderingEnded(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="DestructionStarted"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnDestructionStarted(GUIEventArgs e)
        {
            if (DestructionStarted != null)
            {
                DestructionStarted(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="Activated"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnActivated(WindowEventArgs e)
        {
            isActive = true;

            RequestRedraw();

            if (Activated != null)
            {
                Activated(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="Deactivated"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnDeactivated(WindowEventArgs e)
        {
            // first, de-activate all children
            for (int i = 0; i < ChildCount; i++)
            {
                if (children[i].IsActive)
                {
                    children[i].OnDeactivated(e);
                }
            }

            isActive = false;

            RequestRedraw();

            if (Deactivated != null)
            {
                Deactivated(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="ParentSized"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnParentSized(WindowEventArgs e)
        {
            OnSized(null);

            if (ParentSized != null)
            {
                ParentSized(this, e);
            }
        }

        protected internal virtual void OnParentMoved(WindowEventArgs e)
        {
            OnMoved(new GUIEventArgs());
            RequestRedraw();
            if (ParentMoved != null)
            {
                ParentMoved(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="ChildAdded"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnChildAdded(WindowEventArgs e)
        {
            RequestRedraw();

            if (ChildAdded != null)
            {
                ChildAdded(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="ChildRemoved"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnChildRemoved(WindowEventArgs e)
        {
            RequestRedraw();

            if (ChildRemoved != null)
            {
                ChildRemoved(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseEnters"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnMouseEnters(GUIMouseEventArgs e)
        {
            // todo:
            // set the mouse cursor
            //MouseCursor.Instance.SetImage(this.Cursor);

            if (MouseEnter != null)
            {
                MouseEnter(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseLeaves"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnMouseLeaves(GUIMouseEventArgs e)
        {
            if (MouseLeave != null)
            {
                MouseLeave(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseMove"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual bool OnMouseMove(GUIMouseEventArgs e)
        {
            if (MouseMove != null)
            {
                MouseMove(this, e);
            }
            return true;
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseWheel"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnMouseWheel(GUIMouseEventArgs e)
        {
            if (MouseWheel != null)
            {
                MouseWheel(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseButtonDown"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual bool OnMouseButtonDown(GUIMouseEventArgs e)
        {
            if (e.Button == GUISystem.ClickButton)
            {
                MoveToFront();
            }

            if (MouseButtonDown != null)
            {
                MouseButtonDown(this, e);
            }
            return true;
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseButtonUp"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual bool OnMouseButtonUp(GUIMouseEventArgs e)
        {
            if (MouseButtonUp != null)
            {
                MouseButtonUp(this, e);
            }
            return true;
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseClicked"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual bool OnMouseClicked(GUIMouseEventArgs e)
        {
            if (MouseClicked != null)
            {
                MouseClicked(this, e);
            }
            return true;
        }
        /// <summary>
        ///      Event trigger method for the <see cref="MouseDoubleClicked"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual bool OnMouseDoubleClicked(GUIMouseEventArgs e)
        {
            if (MouseDoubleClicked != null)
            {
                MouseDoubleClicked(this, e);
            }
            return true;
        }

        /// <summary>
        ///      Event trigger method for the <see cref="KeyDown"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnKeyDown(System.Windows.Forms.KeyEventArgs e)
        {
            if (KeyDown != null)
            {
                KeyDown(this, e);
            }
        }
        /// <summary>
        ///      Event trigger method for the <see cref="KeyUp"/> event.
        /// </summary>
        /// <param name="e">Event information.</param>
        protected internal virtual void OnKeyUp(System.Windows.Forms.KeyEventArgs e)
        {
            if (KeyUp != null)
            {
                KeyUp(this, e);
            }
        }

        protected internal virtual void OnKeyPress(System.Windows.Forms.KeyPressEventArgs e)
        {
            if (KeyPress != null)
            {
                KeyPress(this, e);
            }
        }

        #endregion Trigger

        #endregion Methods

        #region IDisposable

        /// <summary>
        // The bulk of the clean-up code is implemented in Dispose(bool)
        /// </summary>
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                // free managed resources

            }
            // free native resources if there are any.
        }

        /// <summary>
        /// IDisposable Members
        /// 遵守.NET最佳实践
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        #endregion

        #region CustomSerialization
        public void PostSerial(Window _parent)
        {
            if (_parent == null)
                return;

            _parent.AddChild(this);

            // 控件初始化自己所需的数据
            PostCustomSerial(_parent);
        }

        /// <summary>
        /// 序列化完成之后调用，用于完成构建自身的Component关系
        /// </summary>
        protected virtual void PostCustomSerial(Window _parent)
        {
            // 因为序列化能保证子控件的顺序，所以这里只需要重新定位父节点
            this.parent = _parent;
            this.OnParentSized(new WindowEventArgs(this));

            foreach (Window wnd in children)
            {
                wnd.PostCustomSerial(this);
            }
        }

        #endregion
    }
}
