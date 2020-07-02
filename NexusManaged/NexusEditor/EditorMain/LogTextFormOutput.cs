using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NexusEngine;

namespace NexusEditor.EditorMain
{
    public sealed class LogTextFormOutput : ILogListener
    {
        private EditorLogForm logForm;

        public LogTextFormOutput(EditorLogForm form)
        {
            logForm = form;
        }

        public void WriteString(LogType type, string message)
        {
            logForm.WriteString(type, message);
        }
    }
}
