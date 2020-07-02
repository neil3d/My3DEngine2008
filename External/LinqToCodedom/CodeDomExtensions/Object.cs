using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;
using System.CodeDom;

namespace LinqToCodedom.Extensions
{
    public static class CodeObjectExtensions
    {
        public static CodeObject Clone(this CodeObject codeNamespace)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                BinaryFormatter f = new BinaryFormatter();
                f.Serialize(ms, codeNamespace);
                ms.Seek(0, SeekOrigin.Begin);
                return f.Deserialize(ms) as CodeObject;
            }
        }

    }
}
