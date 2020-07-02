using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;
using System.Reflection;
using NexusEditor.EditorMain;

namespace NexusEditor.ResourceEditor
{
    public partial class ResSFXEditor : Form
    {
        private ResourcePreview m_preview;  // 预览窗口控件
        private List<SFXElement> m_elementsCtrl;
        private bool m_realtimeMode;
        private NResourceSpecialEffect m_res;
        private List<Type> m_distributionClasses;
		private bool m_finalClose;

        public ResSFXEditor()
        {
            m_realtimeMode = true;
            m_elementsCtrl = new List<SFXElement>();

            InitializeComponent();

            m_res = NResourceManager.Instance.NewSpecialEffect("DefaultSFXRes");
            m_preview = new ResourcePreview("ResSFXPreview_LV");
            m_preview.ShowSpecialEffect(m_res);
            this.splitContainerV.Panel1.Controls.Add(m_preview);
            this.splitContainerH.Panel2.Enabled = true;

            NControl.NCurveEditorCtrl curveCtrl = new NexusEditor.NControl.NCurveEditorCtrl();
            this.groupBoxCurve.Controls.Add(curveCtrl);
            curveCtrl.Dock = DockStyle.Fill;

            //-- 根据特效元素子类型，创建菜单内容
            System.Collections.ArrayList nativeClassList = NEffectElement.GetNativeClassNameList();
            foreach (String className in nativeClassList)
            {
                ToolStripItem menuItem = this.contextMenuStripCreate.Items.Add("new " + className);
                menuItem.Tag = className;
                menuItem.Click += new EventHandler(effectMenuItem_Click);
            }            

            //-- 取得所有Distribution相关的class
            m_distributionClasses = new List<Type>();
            Type baseDistType = typeof(NexusEngine.NDistribution);
            Assembly engineDll = baseDistType.Assembly;
            foreach (Type t in engineDll.GetTypes())
            {
                if (t.IsSubclassOf(baseDistType)
                    && !t.IsAbstract)
                    m_distributionClasses.Add(t);
            }

			m_Timeline.TooltipDelegate = this.OnTimelineTooltip;
        }

		private void OnTimelineTooltip(string text)
		{
			this.statusStripMain.Items[0].Text = text;
		}

        private void effectMenuItem_Click(object sender, EventArgs e)
        {
            ToolStripItem menuItem = sender as ToolStripMenuItem;
            string nativeClassName = menuItem.Tag as String;

            //-- 创建特效元素并添加到资源对象中
            string elemName = ("SFX_" + DateTime.Now.ToFileTime());
            NEffectElement newElem = NEffectElement.CreateEffectElement(nativeClassName, elemName);
            if (newElem == null) return;
            m_res.AddElement(newElem);
            m_preview.ShowSpecialEffect(m_res);
            m_preview.Refresh();

            //-- 创建对应的控件
            int numCtrl = m_elementsCtrl.Count;
            SFXElement newCtrl = new SFXElement(newElem);
            this.splitContainerV.Panel2.Controls.Add(newCtrl);
            newCtrl.Location = new Point(2 + numCtrl * (newCtrl.Width + 2), 2);
            int h = this.splitContainerV.Panel2.Height - 20;
            newCtrl.Height = h;
            m_elementsCtrl.Add(newCtrl);
            newCtrl.SetActive(true);
            newCtrl.RemoveElement += new EventHandler(OnRemoveElement);
            newCtrl.PropertySelected += new EventHandler(OnElementPropertySelected);
            newCtrl.PostPropertyEdit += new EventHandler(OnElementPostPropertyEdit);
            newCtrl.ActiveElement += new EventHandler(OnActiveElement);
            newCtrl.RestartInstance += new EventHandler(OnRestartInstance);
        }

        void OnElementPostPropertyEdit(object sender, EventArgs e)
        {
            SFXElement elemCtrl = sender as SFXElement;
            NActorComponent comp = m_preview.ResourceActorComponent;
            NSpecialEffectInstance sfxComp = comp as NSpecialEffectInstance;
            if (sfxComp != null)
                sfxComp.ResetResource(m_res);
        }

        void OnElementPropertySelected(object sender, EventArgs e)
        {
            SFXElement senderSFX = sender as SFXElement;
            SelectedGridItemChangedEventArgs se = e as SelectedGridItemChangedEventArgs;
            object value = se.NewSelection.Value;
            if (value == null) return;
            string name = se.NewSelection.Label;
            this.propertyGridDist.SelectedObject = value;
            this.propertyGridDist.Text = name;

            Type valueType = value.GetType();
            Type baseType = valueType.BaseType;
            bool isDist = valueType.IsSubclassOf(typeof(NexusEngine.NDistribution));
            if ( isDist )
            {
                this.groupBoxObject.Text = se.NewSelection.Label;
                this.groupBoxObject.Tag = senderSFX.EditElement;
                this.comboBoxType.Items.Clear();
                int sel = 0;
                foreach (Type t in m_distributionClasses)
                {
                    if (t.IsSubclassOf(baseType))
                    {
                        this.comboBoxType.Items.Add(t.Name);
                        if (t.Name == valueType.Name)
                            sel = this.comboBoxType.Items.Count - 1;
                    }
                }
                this.comboBoxType.SelectedIndex = sel;
                if(valueType==typeof(NexusEngine.NDistributionFloatCurve))
                {
                    FloatCurve item = new FloatCurve();
                    item.Name = name;
                    item.Bind((NexusEngine.NDistributionFloatCurve)value);
                    m_Timeline.AddItem(item);
                }
                else if (valueType==typeof(NexusEngine.NDistributionVector2Curve))
                {
                    VectorCurve2 item = new VectorCurve2();
                    item.Name = name;
                    item.Bind((NexusEngine.NDistributionVector2Curve)value);
                    m_Timeline.AddItem(item);
                }
                else if (valueType==typeof(NexusEngine.NDistributionVector3Curve))
                {
                    VectorCurve3 item = new VectorCurve3();
                    item.Name = name;
                    item.Bind((NexusEngine.NDistributionVector3Curve)value);
                    m_Timeline.AddItem(item);
                }
				else if (valueType == typeof(NexusEngine.NDistributionColorCurve))
				{
					ColorCurve item = new ColorCurve();
					item.Name = name;
					item.Bind((NexusEngine.NDistributionColorCurve)value);
					m_Timeline.AddItem(item);
				}
				else
				{
					m_Timeline.Clear();
				}
            }
            else
            {
                m_Timeline.Clear();
            }

            this.splitContainerH.Panel2.Enabled = isDist;
        }

        void OnRemoveElement(object sender, EventArgs e)
        {
            SFXElement elemCtrl = sender as SFXElement;

            //-- 处理资源和渲染对象
            m_res.RemoveElement(elemCtrl.EditElement.Name);
            NActorComponent comp = m_preview.ResourceActorComponent;
            NSpecialEffectInstance sfxComp = comp as NSpecialEffectInstance;
            if (sfxComp != null)
                sfxComp.ResetResource(m_res);

            //-- 更新UI
			m_Timeline.Clear();
			this.splitContainerV.Panel2.Controls.Clear();
            m_elementsCtrl.Remove(elemCtrl);
			//更新控制器
			for (int i = 0; i < m_res.ElementCount(); ++i)
			{
				NEffectElement elem = m_res.GetElement(i);
				int numCtrl = i;
				SFXElement newCtrl = new SFXElement(elem);
				this.splitContainerV.Panel2.Controls.Add(newCtrl);
				newCtrl.Location = new Point(2 + numCtrl * (newCtrl.Width + 2), 2);
				int h = this.splitContainerV.Panel2.Height - 20;
				newCtrl.Height = h;
				newCtrl.SetActive(true);
				newCtrl.RemoveElement += new EventHandler(OnRemoveElement);
				newCtrl.PropertySelected += new EventHandler(OnElementPropertySelected);
				newCtrl.PostPropertyEdit += new EventHandler(OnElementPostPropertyEdit);
				newCtrl.ActiveElement += new EventHandler(OnActiveElement);
				newCtrl.RestartInstance += new EventHandler(OnRestartInstance);
			}
            m_preview.Refresh();
        }

        void OnActiveElement(object sender, EventArgs e)
        {
            SFXElement elemCtrl = sender as SFXElement;

            if (elemCtrl.EditElement != null)
                elemCtrl.EditElement.Visible = elemCtrl.IsActive();

            //-- 更新UI
            m_preview.Refresh();
        }

        void OnRestartInstance(object sender, EventArgs e)
        {
            SFXElement elemCtrl = sender as SFXElement;

            //-- 处理资源和渲染对象
            NActorComponent comp = m_preview.ResourceActorComponent;
            NSpecialEffectInstance sfxComp = comp as NSpecialEffectInstance;
            if (sfxComp != null)
                sfxComp.ResetResource(m_res);
        }

		public new void Close()
		{
			m_preview.Dispose();
			m_finalClose = true;
			base.Close();
		}

        private void ResSFXEditor_FormClosing(object sender, FormClosingEventArgs e)
        {
			if (!m_finalClose)
			{
				e.Cancel = true;
				this.Hide();
			}  
        }

        private void splitContainerV_Panel2_SizeChanged(object sender, EventArgs e)
        {
            int h = this.splitContainerV.Panel2.Height - 20;
            foreach (SFXElement elem in m_elementsCtrl)
            {
                elem.Height = h;
            }
        }

        private void splitContainerH_Panel2_SizeChanged(object sender, EventArgs e)
        {
            int w = this.splitContainerH.Panel2.Width - this.groupBoxCurve.Location.X - 10;
            this.groupBoxCurve.Width = w;
        }

        private void propertyGridDist_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            
        }

        private void timerRender_Tick(object sender, EventArgs e)
        {
            if (m_realtimeMode)
                m_preview.Refresh();
        }

        private void btnResetPropertyType_Click(object sender, EventArgs e)
        {
            //-- 找到选中的type
            string selectedTypeName = this.comboBoxType.SelectedItem as string;
            Type newDistType = null;
            foreach (Type t in m_distributionClasses)
            {
                if (t.Name == selectedTypeName)
                {
                    newDistType = t;
                    break;
                }
            }
            if (newDistType == null)
                return;
                       
            //-- 创建新对象
            NDistribution newDistObj = Activator.CreateInstance(newDistType) as NDistribution;
            if (newDistObj == null)
                return;

            //-- 把新的Distribution对象设置给特效元素
            NEffectElement sfxElem = this.groupBoxObject.Tag as NEffectElement;
            string propertyName = this.groupBoxObject.Text;
            Type elemType = sfxElem.GetType();
            PropertyInfo propertyObj = elemType.GetProperty(propertyName);
            propertyObj.SetValue(sfxElem, newDistObj, null);

            //-- 更新自己的显示
            this.propertyGridDist.SelectedObject = newDistObj;

            string name = this.propertyGridDist.Text;
            Type valueType = newDistObj.GetType();

            //-- 更新编辑网格
            if (valueType == typeof(NexusEngine.NDistributionFloatCurve))
            {
                FloatCurve item = new FloatCurve();
                item.Name = name;
                item.Bind((NexusEngine.NDistributionFloatCurve)newDistObj);
                m_Timeline.AddItem(item);
            }
            else if (valueType == typeof(NexusEngine.NDistributionVector2Curve))
            {
                VectorCurve2 item = new VectorCurve2();
                item.Name = name;
                item.Bind((NexusEngine.NDistributionVector2Curve)newDistObj);
                m_Timeline.AddItem(item);
            }
            else if (valueType == typeof(NexusEngine.NDistributionVector3Curve))
            {
                VectorCurve3 item = new VectorCurve3();
                item.Name = name;
                item.Bind((NexusEngine.NDistributionVector3Curve)newDistObj);
                m_Timeline.AddItem(item);
            }
            else if (valueType == typeof(NexusEngine.NDistributionColorCurve))
            {
                ColorCurve item = new ColorCurve();
                item.Name = name;
                item.Bind((NexusEngine.NDistributionColorCurve)newDistObj);
                m_Timeline.AddItem(item);
            }
            else
            {
                m_Timeline.Clear();
            }
        }

        private void newNToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //新建特效
			m_res.Clear();
			m_elementsCtrl.Clear();
			m_Timeline.Clear();
			this.splitContainerV.Panel2.Controls.Clear();
			NActorComponent comp = m_preview.ResourceActorComponent;
			NSpecialEffectInstance sfxComp = comp as NSpecialEffectInstance;
			if (sfxComp != null)
				sfxComp.ResetResource(m_res);
			m_preview.Refresh();
        }

        private void openOToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //打开特效
			using (VirtualFileDialog dlg = new VirtualFileDialog())
			{
				dlg.Text = "加载特效 ...";
				if (dlg.ShowDialog(this) == DialogResult.OK)
				{
					NResourceLoc loc = dlg.GetResourceLocation();
					m_res.Clear();
					m_elementsCtrl.Clear();
					m_Timeline.Clear();
					this.splitContainerV.Panel2.Controls.Clear();
					m_res.LoadFromFile(loc);
					//更新控制器
					for(int i=0;i<m_res.ElementCount();++i)
					{
						NEffectElement elem = m_res.GetElement(i);
						int numCtrl = m_elementsCtrl.Count;
						SFXElement newCtrl = new SFXElement(elem);
						this.splitContainerV.Panel2.Controls.Add(newCtrl);
						newCtrl.Location = new Point(2 + numCtrl * (newCtrl.Width + 2), 2);
						int h = this.splitContainerV.Panel2.Height - 20;
						newCtrl.Height = h;
						m_elementsCtrl.Add(newCtrl);
						newCtrl.SetActive(true);
						newCtrl.RemoveElement += new EventHandler(OnRemoveElement);
						newCtrl.PropertySelected += new EventHandler(OnElementPropertySelected);
						newCtrl.PostPropertyEdit += new EventHandler(OnElementPostPropertyEdit);
						newCtrl.ActiveElement += new EventHandler(OnActiveElement);
						newCtrl.RestartInstance += new EventHandler(OnRestartInstance);
					}
					NActorComponent comp = m_preview.ResourceActorComponent;
					NSpecialEffectInstance sfxComp = comp as NSpecialEffectInstance;
					if (sfxComp != null)
						sfxComp.ResetResource(m_res);
					m_preview.Refresh();
				}
			}
        }

        private void saveSToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //保存特效
			using (VirtualFileDialog dlg = new VirtualFileDialog())
			{
				dlg.Text = "保存特效 ...";
				if (dlg.ShowDialog(this) == DialogResult.OK)
				{
					NResourceLoc loc = dlg.GetResourceLocation();
					m_res.SaveToFile(loc,true);
				}
			}
        }
    }
}
