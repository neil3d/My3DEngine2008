using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEngineExtension
{
    public interface IPlugin
    {
        void Start();

        void Stop();
    }
}
