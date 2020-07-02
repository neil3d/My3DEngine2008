using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;
using System.Reflection;
using NexusEngineExtension;
using System.IO;

namespace Nexus.GameFramework
{
    public sealed class AttInfoDataHelper 
    {
        public static void ImportFromExcel<T>(string fileName) where T : class
        {
            string extension = Path.GetExtension(fileName);
            if (String.Compare(extension, ".xls", true) != 0
                && String.Compare(extension, ".xlsx", true) != 0)
            {
                NLogger.Instance.WriteString(LogType.Error, "Error Opening File:" + fileName);
                return;
            }

            // 打开Excel文件
            using (ExcelManager em = new ExcelManager())
            {
                em.Open(fileName);

                // 遍历所有的sheet
                List<string> sheetNames = em.SheetNames;
                foreach (string sheet in sheetNames)
                {
                    em.ActivateSheet(sheet);

                    // 建立头
                    Dictionary<string, int> ColumnMaps = new Dictionary<string, int>();
                    for (int index = 1; index < int.MaxValue; ++index)
                    {
                        string CellAddress = ExcelManager.ConvertToCellName(index) + "1";
                        string value = em.GetValue(CellAddress, Category.Formatted) as string;
                        if (string.IsNullOrEmpty(value))
                        {
                            break;
                        }

                        ColumnMaps.Add(value, index);
                    }
                    if (ColumnMaps.Count == 0) break;

                    // 读取表格内容
                    Type objType = typeof(T);
                    // 迭代所有的行
                    for (UInt32 row = 2; row < UInt32.MaxValue; ++row)
                    {
                        // 约定第一行和第一列为必填内容
                        string CellAddress = ExcelManager.ConvertToCellName(1) + row.ToString();
                        string value = em.GetValue(CellAddress, Category.Formatted) as string;
                        if (string.IsNullOrEmpty(value))
                        {
                            break;
                        }

                        // 构造新的对象
                        T info = (T)Activator.CreateInstance(objType, new object[] { value });
                        // 反射所有属性，解析并设置属性的值
                        foreach (PropertyInfo propInfo in objType.GetProperties())
                        {
                            object[] attributes = propInfo.GetCustomAttributes(true);
                            if (attributes == null)
                                continue;

                            foreach (Attribute att in attributes)
                            {
                                TitleMapper fromCell = att as TitleMapper;
                                int column = 0;
                                if (fromCell != null
                                    && ColumnMaps.TryGetValue(fromCell.TitleName, out column))
                                {
                                    CellAddress = ExcelManager.ConvertToCellName(column) + row.ToString();
                                    string CellValue = em.GetValue(CellAddress, Category.Formatted) as string;

                                    ConvertStringToPropertyValue(propInfo, info, CellValue);
                                }
                            }
                        }
                    }
                }
            }
        }

        public static void ConvertStringToPropertyValue(PropertyInfo propInfo, object obj, string value)
        {
            try
            {
                Type propType = propInfo.PropertyType;
                if (propType == typeof(Boolean))
                {
                    propInfo.SetValue(obj, Convert.ToBoolean(value), null);
                }
                else if (propType == typeof(Byte))
                {
                    propInfo.SetValue(obj, Convert.ToByte(value), null);
                }
                else if (propType == typeof(Char))
                {
                    propInfo.SetValue(obj, Convert.ToChar(value), null);
                }
                else if (propType == typeof(DateTime))
                {
                    propInfo.SetValue(obj, Convert.ToDateTime(value), null);
                }
                else if (propType == typeof(Decimal))
                {
                    propInfo.SetValue(obj, Convert.ToDecimal(value), null);
                }
                else if (propType == typeof(Double))
                {
                    propInfo.SetValue(obj, Convert.ToDouble(value), null);
                }
                else if (propType == typeof(Int16))
                {
                    propInfo.SetValue(obj, Convert.ToInt16(value), null);
                }
                else if (propType == typeof(Int32))
                {
                    propInfo.SetValue(obj, Convert.ToInt32(value), null);
                }
                else if (propType == typeof(Int64))
                {
                    propInfo.SetValue(obj, Convert.ToInt64(value), null);
                }
                else if (propType == typeof(SByte))
                {
                    propInfo.SetValue(obj, Convert.ToSByte(value), null);
                }
                else if (propType == typeof(Single))
                {
                    propInfo.SetValue(obj, Convert.ToSingle(value), null);
                }
                else if (propType == typeof(UInt16))
                {
                    propInfo.SetValue(obj, Convert.ToUInt16(value), null);
                }
                else if (propType == typeof(UInt32))
                {
                    propInfo.SetValue(obj, Convert.ToUInt32(value), null);
                }
                else if (propType == typeof(UInt64))
                {
                    propInfo.SetValue(obj, Convert.ToUInt64(value), null);
                }
                else
                {
                    propInfo.SetValue(obj, (object)(value), null);
                }

            }
            catch (System.FormatException e)
            {
                NLogger.Instance.WriteString(LogType.Error, "Set Property Value [" + value + "] to " + propInfo.ToString() + " failed!"
                    + Environment.NewLine + e.ToString());
            }
        }
    }
}
