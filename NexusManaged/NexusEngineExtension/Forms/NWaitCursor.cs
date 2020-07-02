
namespace NexusEngineExtension
{
    /// <summary>
    /// 为窗口显示Wait Cursor, 请与"using"关键字配合使用
    /// </summary>
    public class NWaitCursor : System.IDisposable
    {
        private System.Windows.Forms.Form m_frm;

        public NWaitCursor(System.Windows.Forms.Form frm)
        { 
            m_frm = frm;
            frm.Cursor = System.Windows.Forms.Cursors.WaitCursor;
        }

        #region IDisposable Interface
        public void Dispose()
        {
            m_frm.Cursor = System.Windows.Forms.Cursors.Default;
        }
        #endregion
    }
}
