using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using System.Resources;
using System.Collections;

namespace NexusEditor
{
    /// <summary>
    /// 提供一个编辑*.resx文件的编辑器
    /// </summary>
    public partial class ResXEditor : Form
    {
        public EventHandler<EventArgs> ResXFileOpened;
        public EventHandler<EventArgs> ResXFileSaved;

        private readonly string DefauleFormTitle = "ResX Editor";
        private readonly string DefauleFileFilter = "ResX files (*.resx)|*.resx|" + "All files (*.*)|*.*";
        private readonly string KeyHeaderTitle = "Key";

        private string resxFileName = "";

        private ResXResourceSet resourceSet;
        private XmlDataDocument xmlDocument;

        private bool isDocumentOpen = false;
        private bool isDataModified = false;

        #region Constructor
        public ResXEditor(string fileName)
        {
            InitializeComponent();

            this.xmlGridView.CellValueChanged += new DataGridViewCellEventHandler(xmlGridView_CellValueChanged);
            this.xmlGridView.CurrentCellDirtyStateChanged += new EventHandler(xmlGridView_CurrentCellDirtyStateChanged);
            this.xmlGridView.CellValidating += new DataGridViewCellValidatingEventHandler(xmlGridView_CellValidating);
            this.xmlGridView.CellEndEdit += new DataGridViewCellEventHandler(xmlGridView_CellEndEdit);
            this.xmlGridView.DataError += new DataGridViewDataErrorEventHandler(xmlGridView_DataError);

            OpenResXFile(fileName);
        }

        #endregion

        #region Properties

        public ResXResourceSet ResourceSet
        {
            get { return resourceSet; }
        }

        #endregion

        private void SaveFile()
        {
            this.Validate();
            try
            {
                xmlGridView.EndEdit();
                CurrencyManager cm = (CurrencyManager)xmlGridView.BindingContext[xmlGridView.DataSource, "data"];
                cm.EndCurrentEdit();
                xmlDocument.Save(resxFileName);
                OnResXFileSaved();
                isDataModified = false;
                UpdateForm();
            }
            catch (System.Exception e)
            {
                Program.ShowException(e, "Save ResX File Error!");
            }
        }

        /// <summary>
        /// 引发ResXFileSaved事件
        /// </summary>
        protected virtual void OnResXFileSaved()
        {
            if (ResXFileSaved != null)
            {
                ResXFileSaved(this, new EventArgs());
            }
        }

        private void OpenResXFile(string fileName)
        {
            FileStream file;
            try
            {
                xmlDocument = new XmlDataDocument();
                xmlDocument.DataSet.ReadXmlSchema(fileName);
                file = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                xmlDocument.Load(file);
                file.Close();
                xmlGridView.DataSource = xmlDocument.DataSet;
                xmlGridView.DataMember = "data";

                //we need this to exclude non-text fields
                file = new FileStream(fileName, FileMode.Open, FileAccess.Read);
                resourceSet = new ResXResourceSet(file);

                OnResXFileOpened();

            }
            catch (Exception ex)
            {
                isDocumentOpen = false;
                Program.ShowException(ex, "Open ResX File Error!");
                return;
            }

            resxFileName = fileName;
            InitGrid();

            UpdateGridViewFilter();

            file.Close();

            isDataModified = false;
            isDocumentOpen = true;
            UpdateForm();
        }

        /// <summary>
        /// 用于引发ResXFileOpened事件
        /// </summary>
        protected virtual void OnResXFileOpened()
        {
            if (ResXFileOpened != null)
            {
                ResXFileOpened(this, new EventArgs());
            }
        }

        private void UpdateGridViewFilter()
        {
              //hide non-text fields
            foreach (DictionaryEntry d in resourceSet)
            {
                if (d.Value != null
                    && d.Value.GetType() != typeof(string))
                {
                    foreach (DataGridViewRow row in xmlGridView.Rows)
                    {
                        if (row.Cells["name"].Value.ToString() == d.Key.ToString())
                            row.Visible = false;
                    }
                }
            }
        }
        private void UpdateForm()
        {
            if (resxFileName == null || resxFileName == string.Empty)
            {
                this.Text = DefauleFormTitle;
            }
            else
            {
                string star = isDataModified ? "*" : "";

                int index = Math.Max(resxFileName.LastIndexOf('\\') + 1, 0);
                this.Text = resxFileName.Substring(index) + star + " - " + DefauleFormTitle;
            }

            saveMenuItem.Enabled = isDocumentOpen;
            saveAsMenuItem.Enabled = isDocumentOpen;
        }

        private void InitGrid()
        {
            xmlGridView.AutoGenerateColumns = true;
            xmlGridView.DefaultCellStyle.WrapMode = DataGridViewTriState.True;

            //hiding all columns
            foreach (DataGridViewColumn col in xmlGridView.Columns)
            {
                col.Visible = false;
            }

            //show and set up the Value column
            xmlGridView.Columns["value"].Visible = true;
            xmlGridView.Columns["value"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            xmlGridView.Columns["value"].HeaderText = "Value To be translated";
            xmlGridView.Columns["value"].ToolTipText = "需要被翻译内容";
            xmlGridView.Columns["value"].SortMode = DataGridViewColumnSortMode.NotSortable;

            //show and set up the Comment column
            xmlGridView.Columns["comment"].Visible = true;
            xmlGridView.Columns["comment"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            xmlGridView.Columns["comment"].HeaderText = "Designer comment";
            xmlGridView.Columns["comment"].ToolTipText = "备注";
            xmlGridView.Columns["comment"].DefaultCellStyle.BackColor = Color.LightGray;
            xmlGridView.Columns["comment"].SortMode = DataGridViewColumnSortMode.NotSortable;

            //set up the Name column
            xmlGridView.Columns["name"].Visible = true;
            xmlGridView.Columns["name"].HeaderText = KeyHeaderTitle;
            xmlGridView.Columns["name"].AutoSizeMode = DataGridViewAutoSizeColumnMode.Fill;
            xmlGridView.Columns["name"].DefaultCellStyle.BackColor = Color.LightGray;
            xmlGridView.Columns["name"].SortMode = DataGridViewColumnSortMode.NotSortable;

            // auto-resize rows
            xmlGridView.AutoResizeRows(DataGridViewAutoSizeRowsMode.AllCells);
        }

        void xmlGridView_CellValidating(object sender, DataGridViewCellValidatingEventArgs e)
        {
            // 保证key为字母和数字的组合，并且唯一
            if (xmlGridView.Columns[e.ColumnIndex].HeaderText.Equals(KeyHeaderTitle))
            {
                if (e.RowIndex == xmlGridView.Rows.Count - 1)
                    return;

                string value = e.FormattedValue.ToString();
                if (string.IsNullOrEmpty(value)
                    || !System.Text.RegularExpressions.Regex.IsMatch(value, @"^[a-zA-Z_][A-Za-z0-9_]*$"))
                {
                    e.Cancel = true;
                    xmlGridView.Rows[e.RowIndex].ErrorText = "Key必须是字母、数字和下划线的组合，并且不能为空";
                }

                foreach (DataGridViewRow item in xmlGridView.Rows)
                {
                    if (item.Cells["name"].Value != null
                        && item.Index != e.RowIndex
                        && item.Cells["name"].Value.ToString().Equals(value))
                    {
                        e.Cancel = true;
                        xmlGridView.Rows[e.RowIndex].ErrorText = "Key必须唯一";
                        break;
                    }
                }
            }
        }

        void xmlGridView_CellEndEdit(object sender, DataGridViewCellEventArgs e)
        {
            // Clear the row error in case the user presses ESC.       
            xmlGridView.Rows[e.RowIndex].ErrorText = String.Empty;
        }

        void xmlGridView_DataError(object sender, DataGridViewDataErrorEventArgs e)
        {
            // If the data source raises an exception when a cell value is     
            // commited, display an error message.    
            if (e.Exception != null &&
                e.Context == DataGridViewDataErrorContexts.Commit)
            {
                e.Cancel = true;
                MessageBox.Show(e.Context.ToString());
            }
        }

        void xmlGridView_CurrentCellDirtyStateChanged(object sender, EventArgs e)
        {
            isDataModified = true;
        }

        void xmlGridView_CellValueChanged(object sender, DataGridViewCellEventArgs e)
        {
            UpdateForm();
        }

        private bool ConfirmFileSave(string message)
        {
            DialogResult res;
            res = MessageBox.Show(message, NexusEditor.Properties.Resources.UnsavedChanges, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Exclamation);
            switch (res)
            {
                case DialogResult.Cancel:
                    return false;
                case DialogResult.No:
                    return true;
                default:// DialogResult.Yes:
                    SaveFile();
                    return true;
            }
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            if (isDataModified)
            {
                if (!ConfirmFileSave(NexusEditor.Properties.Resources.UnsavedChangesExit))
                {
                    e.Cancel = true;
                    return;
                }
            }
            base.OnClosing(e);
        }

        protected override void OnClosed(EventArgs e)
        {
            if (resourceSet != null)
            {
                //release resources
                resourceSet.Close();
            }
            base.OnClosed(e);
        }

        private void openMenuItem_Click(object sender, EventArgs e)
        {
            OpenFile();
        }

        private void OpenFile()
        {
            if (isDataModified)
            {
                if (!ConfirmFileSave(NexusEditor.Properties.Resources.UnsavedChangesExit))
                    return;
            }
            OpenFileDialog dialog = new OpenFileDialog();

            dialog.Filter = DefauleFileFilter;
            dialog.FilterIndex = 1;
            //dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                OpenResXFile(dialog.FileName);
            }
        }

        private void saveMenuItem_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        private void saveAsMenuItem_Click(object sender, EventArgs e)
        {
            SaveAsFile();
        }

        private void SaveAsFile()
        {
            Stream fileStream;
            SaveFileDialog dialog = new SaveFileDialog();

            dialog.Filter = DefauleFileFilter;
            dialog.FilterIndex = 1;
            //dialog.RestoreDirectory = true;

            if (dialog.ShowDialog() == DialogResult.OK)
            {
                if ((fileStream = dialog.OpenFile()) != null)
                {
                    fileStream.Close();
                    resxFileName = dialog.FileName;
                    SaveFile();
                }

                isDataModified = false;
                UpdateForm();
            }
        }
        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void toolStripButtonOpen_Click(object sender, EventArgs e)
        {
            OpenFile();
        }

        private void toolStripButtonSave_Click(object sender, EventArgs e)
        {
            SaveFile();
        }

        private void toolStripButtonSaveAs_Click(object sender, EventArgs e)
        {
            SaveAsFile();
        }

        private void viewKeysToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Show/hide Name column of the DataGrid
            xmlGridView.Columns["name"].Visible ^= true; //bool xor true == !bool
        }

        private void wordToolStripMenuItem_Click(object sender, EventArgs e)
        {
            int wordCount = 0;
            foreach (DataGridViewRow row in xmlGridView.Rows)
            {
                string rowText = (string)row.Cells["value"].Value;
                wordCount += rowText.Split(
                    new char[] { ' ', ',', ';', '.', '\t', '\r', '\n' },
                    StringSplitOptions.RemoveEmptyEntries).Length;
            }

            MessageBox.Show("Word Count : " + wordCount,
                "Word Count", MessageBoxButtons.OK);
        }

        private void toolTextSearch_TextChanged(object sender, EventArgs e)
        {
            UpdateGridViewFilter();
        }

        private void toolStripButtonDelete_Click(object sender, EventArgs e)
        {
            this.xmlGridView.Rows.Remove(this.xmlGridView.CurrentRow);
        }
    }
}
