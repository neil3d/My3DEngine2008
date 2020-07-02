using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace NexusEngineExtension
{
    /// <summary>
    /// 绘制在缓冲区中进行,双重缓冲区可防止由控件重绘引起的闪烁。
    /// </summary>
    public partial class DoubleBufferedForm : Form
    {
        public DoubleBufferedForm()
        {
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer 
                | ControlStyles.UserPaint 
                | ControlStyles.AllPaintingInWmPaint,
                true);
        }
    }
}
