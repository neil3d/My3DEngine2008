using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    /// <summary>
    /// 显示,修改CameraController属性
    /// </summary>
    public partial class CameraCtrlSettingDlg : Form
    {
        public CameraCtrlSettingDlg()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 绑定Camera Controller对象
        /// </summary>        
        public void SetCameraCtrl(NEditorCameraController cameraCtrl)
        {
            this.propertyGrid.SelectedObject = cameraCtrl;
        }
    }
}
