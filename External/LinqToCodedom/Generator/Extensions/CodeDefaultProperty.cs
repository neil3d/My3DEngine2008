using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{

    public class CodeDefaultProperty : CodeSnippetTypeMember, ICustomCodeDomObject
    {
        public CodeParameterDeclarationExpressionCollection Parameters { get; set; }
        public bool IsDefault { get; set; }

        public CodeDefaultProperty(CodeMemberProperty property, 
            CodeParameterDeclarationExpressionCollection parameters,
            bool isDefault)
        {
            Parameters = parameters;
            IsDefault = isDefault;
            _property = property;
        }

        public CodeDefaultProperty(CodeMemberProperty property,
            CodeParameterDeclarationExpressionCollection parameters)
        {
            Parameters = parameters;
            _property = property;
        }

        private CodeMemberProperty _property;

        public CodeMemberProperty Property
        {
            get
            {
                return _property;
            }
            set
            {
                _property = value;
            }
        }

        #region ICustomCodeDomObject Members

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            if (_property == null)
                return;

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
                provider.GenerateCodeFromMember(_property, sw, opts);
                StringReader sr = new StringReader(sw.GetStringBuilder().ToString());
                string line = sr.ReadLine();
                while (string.IsNullOrEmpty(line))
                    line = sr.ReadLine();

                int idx = line.IndexOf(" " + _property.Name + "(");
                int asIdx = line.LastIndexOf(" As ");
                StringBuilder sb = new StringBuilder();
                sb.Append((IsDefault?"Default ":string.Empty) + line.Substring(0, idx+_property.Name.Length+2));
                foreach (CodeParameterDeclarationExpression parameter in Parameters)
                {
                    provider.GenerateCodeFromExpression(parameter, new StringWriter(sb), opts);
                    sb.Append(", ");
                }
                sb.Length -= 2;
                sb.Append(")").Append(line.Substring(asIdx));
                Text = sw.GetStringBuilder().Replace(line, sb.ToString()).ToString();
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
                provider.GenerateCodeFromMember(_property, sw, opts);
                StringReader sr = new StringReader(sw.GetStringBuilder().ToString());
                string line = sr.ReadLine();
                while (string.IsNullOrEmpty(line))
                    line = sr.ReadLine();
                int idx = line.IndexOf(" " + _property.Name);

                StringBuilder sb = new StringBuilder();
                sb.Append(" this[");
                foreach (CodeParameterDeclarationExpression parameter in Parameters)
                {
                    provider.GenerateCodeFromExpression(parameter, new StringWriter(sb), opts);
                    sb.Append(", ");
                }
                sb.Length -= 2;
                sb.Append("]");
                Text = sw.GetStringBuilder().Replace(line, line.Substring(0, idx) + sb.ToString() + line.Substring(idx+_property.Name.Length+1)).ToString();
            }
        }

        #endregion

        #endregion
    }
}
