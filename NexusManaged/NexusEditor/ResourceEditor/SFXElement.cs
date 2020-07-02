using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using NexusEngine;

namespace NexusEditor.ResourceEditor
{
    public partial class SFXElement : UserControl
    {
        public event EventHandler RemoveElement;
        public event EventHandler PropertySelected;
        public event EventHandler PostPropertyEdit;
        public event EventHandler ActiveElement;
        public event EventHandler RestartInstance;

        private NEffectElement m_obj;

        public SFXElement(NEffectElement obj)
        {
            InitializeComponent();

            m_obj = obj;
            this.labelType.Text = obj.NativeClassName;
            this.propertyGridEffect.SelectedObject = obj;
        }

        public bool IsActive()
        {
            return checkBoxHide.Checked;
        }

        public void SetActive(bool v)
        {
            checkBoxHide.Checked = v;
        }

        private void SFXElement_SizeChanged(object sender, EventArgs e)
        {
            this.propertyGridEffect.Height = this.Height - this.propertyGridEffect.Location.Y - 4;
        }

        private void btnRemove_Click(object sender, EventArgs e)
        {
            if (RemoveElement!=null)
                RemoveElement(this, e);
        }

        public NEffectElement EditElement
        {
            get { return m_obj; }
        }

        private void propertyGridEffect_SelectedGridItemChanged(object sender, SelectedGridItemChangedEventArgs e)
        {
            if (PropertySelected != null)
                PropertySelected(this, e);
        }

        private void propertyGridEffect_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            if (PostPropertyEdit != null)
                PostPropertyEdit(this, e);
        }

        private void checkBoxHide_CheckedChanged(object sender, EventArgs e)
        {
            if (ActiveElement != null)
                ActiveElement(this, e);
        }

        private void Restart_Click(object sender, EventArgs e)
        {
            if(RestartInstance!=null)
            {
                RestartInstance(this, e);
            }
        }
    }
}
