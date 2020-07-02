using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NexusEditor.ResourceEditor.Importor
{
    interface IImportor
    {
        bool ImportFromFile(string fullPath);
    }
}
