using System;

namespace NexusEngineExtension
{
    public enum SystemCommands
    {
        SC_SIZE = 0xF000,
        SC_MOVE = 0xF010,
        SC_MINIMIZE = 0xF020,
        ///<summary>    
        /// Sent when form maximizes    
        ///</summary>    
        SC_MAXIMIZE = 0xF030,
        ///<summary>    
        /// Sent when form maximizes because of doubcle click on caption   
        /// JTE: Don't use this constant. As per the documentation, you   
        ///      must mask off the last 4 bits of wParam by AND'ing it    
        ///      with 0xFFF0. You can't assume the last 4 bits.    
        ///</summary>   
        SC_MAXIMIZE2 = 0xF032,
        SC_NEXTWINDOW = 0xF040,
        SC_PREVWINDOW = 0xF050,
        ///<summary>    
        /// Closes the form    
        ///</summary>    
        SC_CLOSE = 0xF060,
        SC_VSCROLL = 0xF070,
        SC_HSCROLL = 0xF080,
        SC_MOUSEMENU = 0xF090,
        SC_KEYMENU = 0xF100,
        SC_ARRANGE = 0xF110,
        ///<summary>    
        /// Sent when form is maximized from the taskbar   
        ///</summary>    
        SC_RESTORE = 0xF120,
        ///<summary>    
        /// Sent when form maximizes because of doubcle click on caption    
        /// JTE: Don't use this constant. As per the documentation, you   
        ///      must mask off the last 4 bits of wParam by AND'ing it   
        ///      with 0xFFF0. You can't assume the last 4 bits.     
        ///</summary>    
        SC_RESTORE2 = 0xF122,
        SC_TASKLIST = 0xF130,
        SC_SCREENSAVE = 0xF140,
        SC_HOTKEY = 0xF150,
        SC_DEFAULT = 0xF160,
        SC_MONITORPOWER = 0xF170,
        SC_CONTEXTHELP = 0xF180,
        SC_SEPARATOR = 0xF00F
    }


    public static class WindowConstants
    {
        public const int WM_SIZE = 0x5;
        public const int WM_SYSCOMMAND = 0x112;
        public const int WM_ACTIVATEAPP = 0x001C;
        public const int WM_POWERBROADCAST = 0x0218;

        public const int VK_LWIN = 0x5B;
        public const int VK_RWIN = 0x5C;

        public static readonly IntPtr SIZE_MINIMIZED = new IntPtr(1);
        public static readonly IntPtr SIZE_MAXIMIZED = new IntPtr(2);
        public static readonly IntPtr SIZE_RESTORED = new IntPtr(0);

        public static readonly IntPtr PBT_APMQUERYSUSPEND = new IntPtr(0x0000);
        public static readonly IntPtr PBT_APMRESUMESUSPEND = new IntPtr(0x0007);

        public const int WPF_RESTORETOMAXIMIZED = 2;

        public const int SW_RESTORE = 9;
        public const int SW_SHOWMINIMIZED = 2;
        public const int SW_MAXIMIZE = 3;
        public const int SW_SHOW = 5;
        public const int SW_MINIMIZE = 6;

        public const int GWL_STYLE = -16;
        public const int GWL_EXSTYLE = -20;

        public const long WS_MAXIMIZE = 0x01000000;
        public const long WS_MINIMIZE = 0x20000000;
        public const long WS_POPUP = 0x80000000;
        public const long WS_SYSMENU = 0x00080000;

        public const long WS_EX_TOPMOST = 0x00000008;

        public const uint SWP_NOSIZE = 0x0001;
        public const uint SWP_NOMOVE = 0x0002;
        public const uint SWP_NOZORDER = 0x0004;
        public const uint SWP_NOREDRAW = 0x0008;

        public const uint ES_CONTINUOUS = 0x80000000;
        public const uint ES_DISPLAY_REQUIRED = 0x00000002;

        public const int MONITOR_DEFAULTTOPRIMARY = 1;

        public const Int32 CURSOR_SHOWING = 0x00000001;

        public const int
            IDC_ARROW = 32512,
            IDC_IBEAM = 32513,
            IDC_WAIT = 32514,
            IDC_CROSS = 32515,
            IDC_UPARROW = 32516,
            IDC_SIZE = 32640,
            IDC_ICON = 32641,
            IDC_SIZENWSE = 32642,
            IDC_SIZENESW = 32643,
            IDC_SIZEWE = 32644,
            IDC_SIZENS = 32645,
            IDC_SIZEALL = 32646,
            IDC_NO = 32648,
            IDC_HAND = 32649,
            IDC_APPSTARTING = 32650,
            IDC_HELP = 32651;

        public const uint OCR_NORMAL = 32512;
    }
}
