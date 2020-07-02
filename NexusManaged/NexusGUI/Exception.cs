using System;
using System.Text;
using System.Globalization;

namespace Nexus.GUI
{
    /// <summary>
    /// Application exception of GUI Lib library
    /// </summary>
    public class BaseGUIException : ApplicationException
    {
        public BaseGUIException(string message, params object[] args)
            : base(string.Format(CultureInfo.CurrentCulture,message, args))
        {
        }
    }

    public class AlreadyExitsException : BaseGUIException
    {
        public AlreadyExitsException(string message, params object[] args)
            : base(string.Format(CultureInfo.CurrentCulture,message, args))
        {
        }
    }

    public class UnknownObjectException : BaseGUIException
    {
        public UnknownObjectException(string message, params object[] args)
            : base(string.Format(CultureInfo.CurrentCulture,message, args))
        {
        }
    }

    public class ScriptCompileException : BaseGUIException
    {
        public ScriptCompileException(string message, params object[] args)
            : base(string.Format(CultureInfo.CurrentCulture,message, args))
        {
        }
    }

    public class InvalidRequestException : BaseGUIException
    {
        public InvalidRequestException(string message, params object[] args)
            : base(string.Format(CultureInfo.CurrentCulture,message, args)) { }
    }

    public class FileIOException : BaseGUIException
    {
        public FileIOException(string message, params object[] args)
            : base(string.Format(CultureInfo.CurrentCulture,message, args)) { }
    }

} // end of namespace Nexus.GUI
