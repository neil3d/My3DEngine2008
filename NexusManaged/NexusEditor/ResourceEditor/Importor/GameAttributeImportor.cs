using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using NexusEngine;
using Nexus.GameFramework;

namespace NexusEditor.ResourceEditor
{
    public abstract class GameAttributeImportorBase
    {
        internal bool ImportFromFile()
        {
            // 导入属性数据
            // 打开文件
            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Excel 2003 files (*.xls)|*.xls|Excel 2007 files (*.xlsx)|*.xlsx";
                openFileDialog.FilterIndex = 2;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    bool ret = false;
                    try
                    {
                        ret = ImportExcelContent(openFileDialog.FileName);
                    }
                    catch (System.Exception e)
                    {
                        Program.ShowException(e, "Import Excel Faild!");
                    }
                    finally
                    {
                    }
                    return ret;
                }
            }

            return false;
        }

        protected abstract bool ImportExcelContent(string fullPath);
    }

    internal class LevelupInfoImportor : GameAttributeImportorBase
    {
        override protected bool ImportExcelContent(string fullPath)
        {
            // 先清理内存
            NLevelupInfo.ClearInfo();
            // 导入数据到内存
            AttInfoDataHelper.ImportFromExcel<NLevelupInfo>(fullPath);
            // 存储新的数据
            NLevelupInfo.SaveToFile();
            // 清理内存
            NLevelupInfo.ClearInfo();

            return true;
        }

    }


    internal class SpellInfoImportor : GameAttributeImportorBase
    {
        override protected bool ImportExcelContent(string fullPath)
        {
            // 先清理内存
            NSpellInfo.ClearInfo();
            // 导入数据到内存
            AttInfoDataHelper.ImportFromExcel<NSpellInfo>(fullPath);
            // 存储新的数据
            NSpellInfo.SaveToFile();
            // 清理内存
            NSpellInfo.ClearInfo();
            return true;
        }

    }

    internal class SpellEffectInfoImportor : GameAttributeImportorBase
    {
        override protected bool ImportExcelContent(string fullPath)
        {
            // 先清理内存
            NSpellEffectInfo.ClearInfo();
            // 导入数据到内存
            AttInfoDataHelper.ImportFromExcel<NSpellEffectInfo>(fullPath);
            // 存储新的数据
            NSpellEffectInfo.SaveToFile();
            // 清理内存
            NSpellEffectInfo.ClearInfo();
            return true;
        }

    }

    internal class SpellAuraInfoImportor : GameAttributeImportorBase
    {
        override protected bool ImportExcelContent(string fullPath)
        {
            // 先清理内存
            NSpellAuraInfo.ClearInfo();
            // 导入数据到内存
            AttInfoDataHelper.ImportFromExcel<NSpellAuraInfo>(fullPath);
            // 存储新的数据
            NSpellAuraInfo.SaveToFile();
            // 清理内存
            NSpellAuraInfo.ClearInfo();
            return true;
        }

    }
}
