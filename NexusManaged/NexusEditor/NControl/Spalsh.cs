#region Using Directives
using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Diagnostics;
using System.Threading;
#endregion

namespace NControl
{
    public delegate void DelegateCloseSplash();

    public class SplashForm : Form
    {
        #region Constructor
        private SplashForm()
        {
            // ====================================================================================
            // Setup the form
            // ==================================================================================== 
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.ShowInTaskbar = false;
            this.TopMost = true;

            // make form transparent
            this.TransparencyKey = this.BackColor;

            // tie up the events
            this.KeyUp += new System.Windows.Forms.KeyEventHandler(this.SplashForm_KeyUp);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.SplashForm_Paint);
            this.MouseDown += new MouseEventHandler(SplashForm_MouseClick);

            // load and make the bitmap transparent        
            if (m_bmp == null)
                throw new Exception("Splash bitmap CAN'T be null.");
            //--m_bmp.MakeTransparent(col);

            // resize the form to the size of the iamge
            this.Width = m_bmp.Width;
            this.Height = m_bmp.Height;

            // center the form
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;

            // thread handling
            m_delegateClose = new DelegateCloseSplash(InternalCloseSplash);

        }
        #endregion // Constructor

        #region Public methods
        // this can be used for About dialogs
        public static void ShowModal(Bitmap bmp, Color col)
        {

            MySplashThreadFunc();

        }
        // Call this method with the image file path and the color 
        // in the image to be rendered transparent
        public static void StartSplash(Bitmap bmp)
        {

            m_bmp = bmp;
            // Create and Start the splash thread
            Thread InstanceCaller = new Thread(new ThreadStart(MySplashThreadFunc));
            InstanceCaller.Start();

        }

        // Call this at the end of your apps initialization to close the splash screen
        public static void CloseSplash()
        {
            if (m_instance != null)
                m_instance.Invoke(m_instance.m_delegateClose);


        }
        #endregion // Public methods

        #region Dispose
        protected override void Dispose(bool disposing)
        {
            m_bmp.Dispose();
            base.Dispose(disposing);
            m_instance = null;

        }
        #endregion // Dispose

        #region Threading code
        // ultimately this is called for closing the splash window
        void InternalCloseSplash()
        {
            this.Close();
            this.Dispose();
        }
        // this is called by the new thread to show the splash screen
        private static void MySplashThreadFunc()
        {
            m_instance = new SplashForm();
            m_instance.TopMost = false;
            m_instance.ShowDialog();

        }
        #endregion // Multithreading code

        #region Event Handlers

        void SplashForm_MouseClick(object sender, MouseEventArgs e)
        {
            this.InternalCloseSplash();

        }

        private void SplashForm_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            Rectangle rc = new Rectangle(0, 0, this.Width, this.Height);
            e.Graphics.DrawImage(m_bmp, rc);

        }

        private void SplashForm_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
                this.InternalCloseSplash();

        }
        #endregion // Event Handlers

        #region Private variables
        private static SplashForm m_instance;
        private static Bitmap m_bmp;
        private DelegateCloseSplash m_delegateClose;
        #endregion
    }
}
