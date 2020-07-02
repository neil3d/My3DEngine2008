using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{

    public class CodePartialMethod : CodeSnippetTypeMember, ICustomCodeDomObject
    {

        public CodePartialMethod(CodeMemberMethod method)
        {
            _method = method;
        }

        private CodeMemberMethod _method;

        public CodeMemberMethod Method
        {
            get
            {
                return _method;
            }
            set
            {
                _method = value;
            }
        }

        #region ICustomCodeDomObject Members

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            if (_method == null)
                return;

            if (_method.ReturnType != null && _method.ReturnType.BaseType != "System.Void")
                throw new NotSupportedException("Partial methods must have a void return type");

            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    GenerateCS();
                    break;
                case CodeDomGenerator.Language.VB:
                    GenerateVB();
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }

        #region "VB Code"

        private void GenerateVB()
        {
            using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
            {
                CodeGeneratorOptions opts = new CodeGeneratorOptions();
                StringWriter sw = new StringWriter();
                if (_method.Statements.Count > 0)
                {
                    _method.Attributes = MemberAttributes.Private;
                    provider.GenerateCodeFromMember(_method, sw, opts);
                    Text = sw.GetStringBuilder().ToString();
                }
                else
                {
                    provider.GenerateCodeFromMember(_method, sw, opts);
                    StringReader sr = new StringReader(sw.GetStringBuilder().ToString());
                    string line = sr.ReadLine();
                    while (string.IsNullOrEmpty(line))
                        line = sr.ReadLine();
                    int idx = line.IndexOf(" " + _method.Name + "(");
                    idx = line.LastIndexOf(' ', idx - 1);
                    Text = "Partial Private" + line.Remove(0, idx);
                    Text = sw.GetStringBuilder().Replace(line, Text).ToString();
                }
            }
        }

        #endregion

        #region "CS Code"

        private void GenerateCS()
        {
            using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
            {
                CodeGeneratorOptions opts = new CodeGeneratorOptions();
                StringWriter sw = new StringWriter();
                provider.GenerateCodeFromMember(_method, sw, opts);
                StringReader sr = new StringReader(sw.GetStringBuilder().ToString());
                string line = sr.ReadLine();
                while (string.IsNullOrEmpty(line))
                    line = sr.ReadLine();
                int idx = line.IndexOf(" " + _method.Name + "(");
                idx = line.LastIndexOf(' ', idx - 1);

                if (_method.Statements.Count > 0)
                {
                    Text = "partial" + line.Remove(0, idx);
                    Text = sw.GetStringBuilder().Replace(line, Text).ToString();
                }
                else
                {
                    line = "partial" + line.Remove(0, idx);
                    idx = line.LastIndexOf(')');
                    Text = line.Remove(idx + 1) + ";" + Environment.NewLine;
                }
            }
        }

        #endregion

        #endregion
    }
}
