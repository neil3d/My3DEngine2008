using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using System.Windows.Forms;

namespace NexusEditor.ResourceEditor
{
    public abstract class AttributeInfoEditorBase
    {
        public abstract void ReloadFromFile();
        public abstract void SaveToFile();
        public abstract void NewInfo();
        public abstract void RemoveInfo(object item);
        public abstract void ClearInfo();

        // 刷新到ListView界面
        public void ShowToListView(ref ListView viewControl, string filter)
        {
            viewControl.BeginUpdate();
            viewControl.Items.Clear();

            RefreshListViewItem(ref viewControl, filter);

            viewControl.EndUpdate();
        }

        protected abstract void RefreshListViewItem(ref ListView viewControl, string filter);
    }


    #region NSpellInfoEditor
    public class NSpellInfoEditor : AttributeInfoEditorBase
    {
        public NSpellInfoEditor()
        {
        }

        public override void ClearInfo()
        {
            NSpellInfo.ClearInfo();
        }

        public override void RemoveInfo(object item)
        {
            NSpellInfo info = item as NSpellInfo;
            if (info != null)
            {
                NSpellInfo.RemoveInfo(info.ID);
            }
        }

        public override void NewInfo()
        {
            NSpellInfo info = new NSpellInfo(Guid.NewGuid().ToString());
        }

        public override void SaveToFile()
        {
            NSpellInfo.SaveToFile();
        }

        public override void ReloadFromFile()
        {
            NSpellInfo.ClearInfo();
            NSpellInfo.LoadFromFile();
        }

        protected override void RefreshListViewItem(ref ListView viewControl, string filter)
        {
            Dictionary<UInt32, NSpellInfo> infos = NSpellInfo.ConvertNativeInfos();
            // 刷新界面
            foreach (var item in infos)
            {
                if (item.Value.ID.IndexOf(filter) == -1
                    && item.Value.Name.IndexOf(filter) == -1
                    && item.Value.Description.IndexOf(filter) == -1)
                    continue;


                ListViewItem listItem = new ListViewItem();
                listItem.Text = item.Value.ID;
                listItem.Tag = item.Value;

                listItem.SubItems.Add(item.Value.Name);
                listItem.SubItems.Add(item.Value.Description);

                viewControl.Items.Add(listItem);
            }
        }
    }
    #endregion


    #region NSpellEffectInfoEditor
    public class NSpellEffectInfoEditor : AttributeInfoEditorBase
    {
        public NSpellEffectInfoEditor()
        {
        }

        public override void ClearInfo()
        {
            NSpellEffectInfo.ClearInfo();
        }

        public override void RemoveInfo(object item)
        {
            NSpellEffectInfo info = item as NSpellEffectInfo;
            if (info != null)
            {
                NSpellEffectInfo.RemoveInfo(info.ID);
            }
        }

        public override void NewInfo()
        {
            NSpellEffectInfo info = new NSpellEffectInfo(Guid.NewGuid().ToString());
        }

        public override void SaveToFile()
        {
            NSpellEffectInfo.SaveToFile();
        }

        public override void ReloadFromFile()
        {
            NSpellEffectInfo.ClearInfo();
            NSpellEffectInfo.LoadFromFile();
        }

        protected override void RefreshListViewItem(ref ListView viewControl, string filter)
        {
            Dictionary<UInt32, NSpellEffectInfo> infos = NSpellEffectInfo.ConvertNativeInfos();
            // 刷新界面
            foreach (var item in infos)
            {
                if (item.Value.ID.IndexOf(filter) == -1
    && item.Value.Name.IndexOf(filter) == -1
    && item.Value.Description.IndexOf(filter) == -1)
                    continue;

                ListViewItem listItem = new ListViewItem();
                listItem.Text = item.Value.ID;
                listItem.Tag = item.Value;

                listItem.SubItems.Add(item.Value.Name);
                listItem.SubItems.Add(item.Value.Description);

                viewControl.Items.Add(listItem);
            }
        }
    }
    #endregion

    #region NSpellAuraInfoEditor
    public class NSpellAuraInfoEditor : AttributeInfoEditorBase
    {
        public NSpellAuraInfoEditor()
        {
        }

        public override void ClearInfo()
        {
            NSpellAuraInfo.ClearInfo();
        }

        public override void RemoveInfo(object item)
        {
            NSpellAuraInfo info = item as NSpellAuraInfo;
            if (info != null)
            {
                NSpellAuraInfo.RemoveInfo(info.ID);
            }
        }

        public override void NewInfo()
        {
            NSpellAuraInfo info = new NSpellAuraInfo(Guid.NewGuid().ToString());
        }

        public override void SaveToFile()
        {
            NSpellAuraInfo.SaveToFile();
        }

        public override void ReloadFromFile()
        {
            NSpellAuraInfo.ClearInfo();
            NSpellAuraInfo.LoadFromFile();
        }

        protected override void RefreshListViewItem(ref ListView viewControl, string filter)
        {
            Dictionary<UInt32, NSpellAuraInfo> infos = NSpellAuraInfo.ConvertNativeInfos();
            // 刷新界面
            foreach (var item in infos)
            {
                if (item.Value.ID.IndexOf(filter) == -1
    && item.Value.Name.IndexOf(filter) == -1
    && item.Value.Description.IndexOf(filter) == -1)
                    continue;

                ListViewItem listItem = new ListViewItem();
                listItem.Text = item.Value.ID;
                listItem.Tag = item.Value;

                listItem.SubItems.Add(item.Value.Name);
                listItem.SubItems.Add(item.Value.Description);

                viewControl.Items.Add(listItem);
            }
        }
    }
    #endregion


    #region NLevelupInfoEditor
    public class NLevelupInfoEditor : AttributeInfoEditorBase
    {
        public NLevelupInfoEditor()
        {
        }

        public override void ClearInfo()
        {
            NLevelupInfo.ClearInfo();
        }

        public override void RemoveInfo(object item)
        {
            NLevelupInfo info = item as NLevelupInfo;
            if (info != null)
            {
                NLevelupInfo.RemoveInfo(info.ID);
            }
        }

        public override void NewInfo()
        {
            NLevelupInfo info = new NLevelupInfo(Guid.NewGuid().ToString());
        }

        public override void SaveToFile()
        {
            NLevelupInfo.SaveToFile();
        }

        public override void ReloadFromFile()
        {
            NLevelupInfo.ClearInfo();
            NLevelupInfo.LoadFromFile();
        }

        protected override void RefreshListViewItem(ref ListView viewControl, string filter)
        {
            Dictionary<UInt32, NLevelupInfo> infos = NLevelupInfo.ConvertNativeInfos();
            // 刷新界面
            foreach (var item in infos)
            {
                if (item.Value.ID.IndexOf(filter) == -1
    && item.Value.Name.IndexOf(filter) == -1
    && item.Value.Description.IndexOf(filter) == -1)
                    continue;

                ListViewItem listItem = new ListViewItem();
                listItem.Text = item.Value.ID;
                listItem.Tag = item.Value;

                listItem.SubItems.Add(item.Value.Name);
                listItem.SubItems.Add(item.Value.Description);

                viewControl.Items.Add(listItem);
            }
        }
    }
    #endregion

    #region NSpellTalentTableEditor
    public class NSpellTalentTableEditor : AttributeInfoEditorBase
    {
        public NSpellTalentTableEditor()
        {
        }

        public override void ClearInfo()
        {
            NSpellTalentTable.ClearInfo();
        }

        public override void RemoveInfo(object item)
        {
            NSpellTalentTable info = item as NSpellTalentTable;
            if (info != null)
            {
                NSpellTalentTable.RemoveInfo(info.ID);
            }
        }

        public override void NewInfo()
        {
            NSpellTalentTable info = new NSpellTalentTable(Guid.NewGuid().ToString());
        }

        public override void SaveToFile()
        {
            NSpellTalentTable.SaveToFile();
        }

        public override void ReloadFromFile()
        {
            NSpellTalentTable.ClearInfo();
            NSpellTalentTable.LoadFromFile();
        }

        protected override void RefreshListViewItem(ref ListView viewControl, string filter)
        {
            Dictionary<UInt32, NSpellTalentTable> infos = NSpellTalentTable.ConvertNativeInfos();
            // 刷新界面
            foreach (var item in infos)
            {
                if (item.Value.ID.IndexOf(filter) == -1
    && item.Value.Name.IndexOf(filter) == -1
    && item.Value.Description.IndexOf(filter) == -1)
                    continue;

                ListViewItem listItem = new ListViewItem();
                listItem.Text = item.Value.ID;
                listItem.Tag = item.Value;

                listItem.SubItems.Add(item.Value.Name);
                listItem.SubItems.Add(item.Value.Description);

                viewControl.Items.Add(listItem);
            }
        }
    }
    #endregion
}
