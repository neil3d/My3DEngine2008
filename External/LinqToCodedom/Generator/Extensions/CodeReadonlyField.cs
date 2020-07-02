using System;
using System.CodeDom.Compiler;
using System.IO;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeReadOnlyField : CodeSnippetTypeMember, ICustomCodeDomObject
    {
        public CodeMemberField Field { get; set; }

        public CodeReadOnlyField(CodeMemberField field)
        {
            Field = field;
        }

        #region ICustomCodeDomObject Members

        public void GenerateCode(CodeDomGenerator.Language language)
        {
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
                StringBuilder sb = new StringBuilder();
                using (IndentedTextWriter tw = new IndentedTextWriter(new StringWriter(sb), opts.IndentString))
                {
                    provider.GenerateCodeFromMember(Field, tw, opts);
                    sb.Replace(" " + Field.Name, " ReadOnly " + Field.Name);
                }
                Text = sb.ToString();
            }
        }

        #endregion

        #region "CS Code"

        private void GenerateCS()
        {
            using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
            {
                CodeGeneratorOptions opts = new CodeGeneratorOptions();
                StringBuilder sb = new StringBuilder();
                using (IndentedTextWriter tw = new IndentedTextWriter(new StringWriter(sb), opts.IndentString))
                {
                    provider.GenerateCodeFromMember(Field, tw, opts);
                    var type = new StringWriter();
                    provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(Field.Type), type, opts);
                    sb.Replace(" " + type.GetStringBuilder(), " readonly " + type.GetStringBuilder());
                }
                Text = sb.ToString();
            }
        }

        #endregion

        #endregion
    }
}
