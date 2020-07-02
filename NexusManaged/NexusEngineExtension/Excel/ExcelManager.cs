using System;
using System.Text;
using System.Globalization;
using Microsoft.Office.Interop.Excel;
using System.Threading;
using System.Collections;
using System.Diagnostics;
using System.Collections.Generic;

namespace NexusEngineExtension
{
    /// <summary>
    /// <para>Main Helper class to use excel functionality in dotNet.</para>
    /// <para>It works as a wrapper around official Microsof.Office.Interop.Excel</para>
    /// </summary>
    public class ExcelManager : IDisposable
    {
        ApplicationClass App;

        CultureInfo OriginalCulture;

        private string openFileName;
        /// <summary>
        /// The name of currently opened file
        /// </summary>
        public string OpenFileName
        {
            get { return openFileName; }
        }

        /// <summary>
        /// Returns a boolean value indicates if any file (workbook) is open or not
        /// </summary>
        public bool AnyFileOpen
        {
            // After opening a file, we assign its name to openFileName. After closing,
            // we clear openFileName by assigning String.Empty to it.
            // So a String.Empty value shows no file is open
            get { return !String.IsNullOrEmpty(openFileName); }
        }

        /// <summary>
        /// Default Constructor of ExcelManager.
        /// </summary>
        /// <remarks>
        /// <example>Following example shows how to use this constructor:
        /// <code>
        /// using(ExcelManager em = new ExcelManager())
        /// {
        ///     // codes using excel file go here, for example:
        ///     em.Open(filename);
        /// }
        /// </code>
        /// </example>
        /// <b>Note:</b> This Constructor changes current thread's culture to "en-US" and returns it to 
        /// previous state after dispose.
        /// </remarks>
        public ExcelManager()
        {
            // During any access to the excel interop functions, current thread's culture must be "en-US".
            // We save current thread's culture, which will be restored in Dispose method
            OriginalCulture = Thread.CurrentThread.CurrentCulture;
            Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");

            // Loads Excel
            App = new ApplicationClass();
            // Disables excel alerts (like save-as overwrite) which are meaningless during 
            // programmatically access to excel 
            App.DisplayAlerts = false;
        }

        #region Colum Name/Number Convert

        /// <summary>
        /// Colum index number to string name
        /// </summary>
        public static string ConvertToCellName(int number)
        {
            string ret = "";
            while (number > 0)
            {
                ret = (char)(--number % 26 + 'A') + ret;
                number /= 26;
            }
            return ret;
        } 

        /// <summary>
        /// Colum  string name to index number
        /// </summary>
        public static int ColNumber(string strCol)
        {
            strCol = strCol.ToUpper();
            int intColNumber = 0;
            if (strCol.Length > 1)
            {
                intColNumber = Convert.ToInt16(Convert.ToByte(strCol[1]) - 65);
                intColNumber += Convert.ToInt16(Convert.ToByte(strCol[1]) - 64) * 26;
            }
            else
                intColNumber = Convert.ToInt16(Convert.ToByte(strCol[0]) - 65);
            return intColNumber;
        }
        #endregion

        #region Functions to work with Files (workbooks)

        /// <summary>
        /// Creates a new excel file.
        /// </summary>
        /// <param name="fileName">A string that indicates the name of the file to be created</param>
        public void Create(string fileName)
        {
            try
            {
                Close();
                App.Workbooks.Add(XlWBATemplate.xlWBATWorksheet);
                App.ActiveWorkbook.SaveAs(fileName,
                                          XlFileFormat.xlWorkbookNormal,
                                          System.Type.Missing,
                                          System.Type.Missing,
                                          System.Type.Missing,
                                          System.Type.Missing,
                                          XlSaveAsAccessMode.xlNoChange,
                                          System.Type.Missing,
                                          System.Type.Missing,
                                          System.Type.Missing,
                                          System.Type.Missing,
                                          System.Type.Missing);
                openFileName = fileName;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Error Creating File '{0}'", fileName), err);
            }
        }

        /// <summary>
        /// Opens an excel file.
        /// </summary>
        /// <param name="fileName">The name of the file to open</param>
        public void Open(string fileName)
        {
            try
            {
                Close();
                App.Workbooks.Open(fileName,
                                    false,
                                    false,
                                    System.Type.Missing,
                                    System.Type.Missing,
                                    System.Type.Missing,
                                    System.Type.Missing,
                                    System.Type.Missing,
                                    System.Type.Missing,
                                    false,
                                    System.Type.Missing,
                                    System.Type.Missing,
                                    false,
                                    System.Type.Missing,
                                    System.Type.Missing);
                openFileName = fileName;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Error Opening File '{0}'", fileName), err);
            }
        }

        /// <summary>
        /// Closes currently open file.
        /// </summary>
        public void Close()
        {
            try
            {
                App.Workbooks.Close();
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Error Closing File '{0}'", openFileName), err);
            }
        }

        /// <summary>
        /// Saves current file. If no file is open, throws ExcelException
        /// </summary>
        public void Save()
        {
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            App.ActiveWorkbook.Save();
        }

        /// <summary>
        /// Saves current file by a new name. If no file is open, throws ExcelException
        /// </summary>
        /// <param name="newFileName">New name of file</param>
        public void SaveAs(string newFileName)
        {
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                App.ActiveWorkbook.SaveAs(newFileName,
                                        XlFileFormat.xlWorkbookNormal,
                                        System.Type.Missing,
                                        System.Type.Missing,
                                        System.Type.Missing,
                                        System.Type.Missing,
                                        XlSaveAsAccessMode.xlNoChange,
                                        System.Type.Missing,
                                        System.Type.Missing,
                                        System.Type.Missing,
                                        System.Type.Missing,
                                        System.Type.Missing);
                openFileName = newFileName;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not save file as '{0}'", newFileName), err);
            }
        }

        #endregion

        #region Functions to work with Worksheets

        public int SheetCount
        {
            get
            {
                return App.ActiveWorkbook.Sheets.Count;
            }
        }

        public List<string> SheetNames
        {
            get
            {
                List<string> sheetNames = new List<string>();
                foreach (_Worksheet wsheet in App.ActiveWorkbook.Sheets)
                {
                    sheetNames.Add(wsheet.Name);
                }
                return sheetNames;
            }
        }

        public bool HasSheet(string sheetName)
        {
            foreach (_Worksheet wsheet in App.ActiveWorkbook.Sheets)
            {
                if (String.Compare(wsheet.Name, sheetName, true) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Activates given sheet.
        /// </summary>
        /// <param name="sheetName">Name of sheet to be activated. Case in-sensitive.</param>
        public void ActivateSheet(string sheetName)
        {
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                // "_Worksheet" is used instead of "Worksheet" to resolve ambiguity 
                // between _Worksheet.Activate() method and DocEvents_Event.Activate event.
                foreach (_Worksheet wsheet in App.ActiveWorkbook.Sheets)
                    if (String.Compare(wsheet.Name, sheetName, true) == 0)
                    {
                        wsheet.Activate();
                        return;
                    }
                throw new ExcelException(String.Format("Can not find sheet '{0}'", sheetName));
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not activate sheet '{0}'", sheetName), err);
            }
        }

        /// <summary>
        /// Renames a sheet.
        /// </summary>
        /// <param name="oldName">Current name of sheet</param>
        /// <param name="newName">New name of sheet</param>
        public void RenameSheet(string oldName, string newName)
        {
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                // "_Worksheet" is used instead of "Worksheet" to resolve ambiguity 
                // between _Worksheet.Activate() method and DocEvents_Event.Activate event.
                foreach (_Worksheet wsheet in App.Sheets)
                {
                    if (String.Compare(wsheet.Name, oldName, true) == 0)
                    {
                        wsheet.Name = newName;
                        return;
                    }
                }
                throw new ExcelException(String.Format("Can not find sheet '{0}'", oldName));
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not rename sheet '{0}' to '{1}'", oldName, newName), err);
            }
        }

        /// <summary>
        /// Renames current sheet.
        /// </summary>
        /// <param name="newName">New name of current sheet</param>
        public void RenameCurrentSheet(string newName)
        {
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                (App.ActiveSheet as _Worksheet).Name = newName;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    "Can not rename current sheet", err);
            }
        }
        #endregion

        #region Functions to work with cell and range values

        public object GetValue(string cellAddress, Category category)
        {
            if (String.IsNullOrEmpty(cellAddress))
                throw new ArgumentNullException("cellAddress");
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                Range range = (App.ActiveSheet as _Worksheet).get_Range(cellAddress, System.Type.Missing);
                if (category == Category.Numeric)
                    return range.Value2;
                else
                    return range.Text;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not access values at address '{0}'", cellAddress), err);
            }
        }

        /// <summary>
        /// Gets numeric value of cell content. 
        /// </summary>
        /// <param name="cellAddress">Cell's address (for example "A2")</param>
        /// <returns>nullable double precision value of cell's contents</returns>
        /// <remarks>
        /// Numeric value of cell content is an unformatted, floating-point value 
        /// which can be used in calculations. Specially useful in Currency types where,for example, -12 will 
        /// return instead of -$12.00
        /// <b>Note:</b> This method works only with a single cell. For multiple cells see <see cref="GetRangeNumericValues"/>
        /// </remarks>
        /// <seealso cref="GetFormattedValue"/>
        public double? GetNumericValue(string cellAddress)
        {
            return (double?)GetValue(cellAddress, Category.Numeric);
        }

        /// <summary>
        /// Gets formatted content of a cell.
        /// </summary>
        /// <param name="cellAddress">Cell's address (for example "A2")</param>
        /// <returns>cell content</returns>
        /// <remarks>
        /// <para>Formatted content is based on specified category for cell (DateTime, Currency, Numeric and so on)</para>
        /// <para>This method works only with a single cell. For multiple cells see <see cref="GetRangeNumericValues"/></para>
        /// </remarks>
        /// <seealso cref="GetNumericValue"/>
        /// <seealso cref="GetRangeNumericValues"/>
        public object GetFormattedValue(string cellAddress)
        {
            return GetValue(cellAddress, Category.Formatted);
        }

        /// <summary>
        /// Determines if a cell blongs to "waste" cells of merge area.
        /// </summary>
        /// <remarks>
        /// In a merge area the first cell contains useful value and the others are being considered as "waste".
        /// This private method determines if given cell is a "waste" or not.
        /// </remarks>
        /// <param name="range">Cell address in the form of a Range</param>
        /// <returns>True if cell is Waste,false if it contains a useful value</returns>
        private bool IsWasteCellInMergeArea(Range range)
        {
            // If it is not a part of merge area, it can not be a waste!
            if (!((bool)range.MergeCells))
                return false;
            Range firstCellInMergeArea = range.MergeArea.Cells[1, 1] as Range;
            // If it is not first cell of merge area, it is a waste cell
            return !(range.Column == firstCellInMergeArea.Column && range.Row == firstCellInMergeArea.Row);
        }

        public ArrayList GetRangeValues(string startCellAddress, string endCellAddress, Category category)
        {
            if (String.IsNullOrEmpty(startCellAddress))
                throw new ArgumentNullException("startCellAddress");
            if (String.IsNullOrEmpty(endCellAddress))
                throw new ArgumentNullException("endCellAddress");
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                Range range = App.get_Range(startCellAddress, endCellAddress);

                ArrayList arr = new ArrayList();
                foreach (Range r in range)
                {
                    if (IsWasteCellInMergeArea(r))
                        continue;
                    if (category == Category.Formatted)
                        arr.Add(r.Text);
                    else
                        arr.Add((double?)r.Value2);
                }
                return arr;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not get values of range '{0}:{1}'", startCellAddress, endCellAddress), err);
            }
        }

        /// <summary>
        /// Gets values of multiple cells and put them in an array of objects.
        /// </summary>
        /// <param name="startCellAddress">First cell's address (for example "A1")</param>
        /// <param name="endCellAddress">Last cell's address (for example "A1")</param>
        /// <returns>An array of formatted values</returns>
        /// <remarks>
        /// <para>Returning array of this method is always single-dimension, even with rectangular ranges (like "A1:C5")</para>
        /// </remarks>
        /// <seealso cref="GetFormattedValue"/>
        public ArrayList GetRangeFormattedValues(string startCellAddress, string endCellAddress)
        {
            return GetRangeValues(startCellAddress, endCellAddress, Category.Formatted);
        }

        /// <summary>
        /// Gets numeric values of multiple cells and put them in an array of nullable doubles.
        /// </summary>
        /// <param name="startCellAddress">First cell's address (for example "A1")</param>
        /// <param name="endCellAddress">Last cell's address (for example "A1")</param>
        /// <returns>An array of numeric values but with type of object[]</returns>
        /// <remarks>
        /// <para>Returning array of this method is always single-dimension, even with rectangular ranges (like "A1:C5")</para>
        /// </remarks>
        /// <seealso cref="GetNumericValue"/>
        public ArrayList GetRangeNumericValues(string startCellAddress, string endCellAddress)
        {
            return GetRangeValues(startCellAddress, endCellAddress, Category.Numeric);
        }

        /// <summary>
        /// Sets a cell content.
        /// </summary>
        /// <param name="cellAddress">Cell's address (for example "A2")</param>
        /// <param name="value">Any desired value</param>
        public void SetValue(string cellAddress, object value)
        {
            if (String.IsNullOrEmpty(cellAddress))
                throw new ArgumentNullException("cellAddress");
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                App.get_Range(cellAddress, System.Type.Missing).Value2 = value;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not set value of cell '{0}'", cellAddress), err);
            }
        }

        /// <summary>
        /// Sets same value in a given range of cells.
        /// </summary>
        /// <param name="startCellAddress">First cell's address (for example "A1")</param>
        /// <param name="endCellAddress">Last cell's address (for example "A1")</param>
        /// <param name="value">Any desired value</param>
        /// <remarks>
        /// <para>For setting different values in range see <see cref="SetRangeValue"/></para>
        /// </remarks>
        /// <seealso cref="SetRangeValues" />
        public void SetRangeValue(string startCellAddress, string endCellAddress, object value)
        {
            if (String.IsNullOrEmpty(startCellAddress))
                throw new ArgumentNullException("startCellAddress");
            if (String.IsNullOrEmpty(endCellAddress))
                throw new ArgumentNullException("endCellAddress");
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                App.get_Range(startCellAddress, endCellAddress).Value2 = value;
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not set values of range '{0}:{1}'", startCellAddress, endCellAddress), err);
            }
        }

        /// <summary>
        /// Sets an array of values in a Range.
        /// </summary>
        /// <param name="startCellAddress">First cell's address (for example "A1")</param>
        /// <param name="endCellAddress">Last cell's address (for example "A1")</param>
        /// <param name="values">Any array of desired values</param>
        /// <remarks>
        /// <para>Array and Range can be of diffrent sizes. If array is smaller, extra cells remain intact. 
        /// If range is smaller, extra values of array will not be used.</para>
        /// <para>Merge areas will also be considered. The whole merge area assumes as one cell.</para>
        /// </remarks>
        public void SetRangeValues(string startCellAddress, string endCellAddress, IList values)
        {
            if (values == null)
                throw new ArgumentNullException("values");
            if (String.IsNullOrEmpty(startCellAddress))
                throw new ArgumentNullException("startCellAddress");
            if (String.IsNullOrEmpty(endCellAddress))
                throw new ArgumentNullException("endCellAddress");
            if (!AnyFileOpen)
                throw new ExcelException("No file is Open");

            try
            {
                int index = 0;
                Range range = App.get_Range(startCellAddress, endCellAddress);
                foreach (Range r in range)
                {
                    if (index >= values.Count)
                        return;
                    if (IsWasteCellInMergeArea(r))
                        continue;
                    r.Value2 = values[index];
                    index++;
                }
            }
            catch (Exception err)
            {
                throw new ExcelException(
                    String.Format(CultureInfo.InvariantCulture, "Can not set values of range '{0}:{1}'", startCellAddress, endCellAddress), err);
            }
        }
        #endregion

        #region IDisposable Members

        private bool _disposedValue; // To detect redundant calls

        /// <summary>
        /// This function is a part of Disposable pattern.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        /// <remarks>
        /// As a part of Disposable pattern, releases resources and return thread's CurrentCulture to its original state.
        /// </remarks>
        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
                if (disposing)
                {
                    if (App != null)
                    {
                        // Disables excel alerts (like save-as overwrite) which are meaningless during 
                        // programmatically access to excel 
                        App.DisplayAlerts = true;
                        Close();
                        App.Quit();
                        App = null;
                        Thread.CurrentThread.CurrentCulture = OriginalCulture;
                    }
                }
            _disposedValue = true;
        }
        #endregion
    }
}
