using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;
using Nexus.GameFramework;
using System.Globalization;
using System.Configuration;

namespace NexusEditor.EditorMain
{
	public partial class GlobalSettingForm : Form
	{
		public GlobalSettingForm()
		{
			InitializeComponent();
			UpdatePropertyList();
		}

		private void UpdatePropertyList()
		{
			TreeNode node = new TreeNode("全局环境光");
			node.Tag = new GlobalSkyLightProperty();
			m_List.Nodes.Add(node);
			node = new TreeNode("SSAO");
			node.Tag = new SSAOProperty();
			m_List.Nodes.Add(node);
		}

		private void GlobalSettingForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			if (e.CloseReason == CloseReason.UserClosing)
			{
				e.Cancel = true;
				this.Hide();
			}
		}

		private void m_List_AfterSelect(object sender, TreeViewEventArgs e)
		{
			if(e.Node!=null)
			{
				m_PropertyGrid.SelectedObject = e.Node.Tag;
			}
		}
	}

	class GlobalSkyLightProperty
	{
		[Category("属性"), Description("顶部的色彩")]
		public Color4f HighColor
		{
			get
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return new Color4f(0);
				return lv.GetRenderSetting().SkyLightHightColor;
			}
			set
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return;
				lv.GetRenderSetting().SkyLightHightColor=value;
			}
		}

		[Category("属性"), Description("底部的色彩")]
		public Color4f LowColor
		{
			get
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return new Color4f(0);
				return lv.GetRenderSetting().SkyLightLowColor;
			}
			set
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return;
				lv.GetRenderSetting().SkyLightLowColor = value;
			}
		}
	}

	class SSAOProperty
	{
		[Category("属性"), Description("是否开始SSAO")]
		public bool	Enable
		{
			get
			{
				return NLevelEditorEngine.Instance.Config.EnableSSAO;
			}
			set
			{
				CustomConfigurationSection configuration = CustomConfigurationSection.Open();
				NLevelEditorEngine.Instance.Config.EnableSSAO=value;
				configuration.EngineSetting.EnableSSAO = value;
                configuration.Save();
			}
		}

		[Category("属性"), Description("遮挡半径")]
		public float OcclustionRadius
		{
			get
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return 0;
				return lv.GetRenderSetting().SSAOOcclustionRadius;
			}
			set
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return;
				lv.GetRenderSetting().SSAOOcclustionRadius = value;
			}
		}

		[Category("属性"), Description("遮挡指数")]
		public float OcclustionPower
		{
			get
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return 0;
				return lv.GetRenderSetting().SSAOOcclustionPower;
			}
			set
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return;
				lv.GetRenderSetting().SSAOOcclustionPower = value;
			}
		}

		[Category("属性"), Description("遮挡衰减指数")]
		public float BlockerPower
		{
			get
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return 0;
				return lv.GetRenderSetting().SSAOBlockerPower;
			}
			set
			{
				NLevel lv = NLevelEditorEngine.Instance.MainLevel;
				if (lv == null) return;
				lv.GetRenderSetting().SSAOBlockerPower = value;
			}
		}
	}
}
