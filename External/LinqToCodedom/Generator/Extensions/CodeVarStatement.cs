using System;
using System.CodeDom;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeVarStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        public string VariableName { get; set; }
        public CodeExpression InitExpression { get; set; }

        public CodeVarStatement(string name, CodeExpression initExpression)
        {
            VariableName = name;
            InitExpression = initExpression;
        }

        public void GenerateCode(CodeDomGenerator.Language language)
        {
            switch (language)
            {
                case CodeDomGenerator.Language.CSharp:
                    using (Microsoft.CSharp.CSharpCodeProvider provider = new Microsoft.CSharp.CSharpCodeProvider())
                    {
                        System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                        using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                        {
                            tw.Write("var {0} = ", provider.CreateEscapedIdentifier(VariableName));
                            provider.GenerateCodeFromExpression(InitExpression, tw, opts);
                            tw.Write(";");
                            Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;    
                case CodeDomGenerator.Language.VB:
                    using (Microsoft.VisualBasic.VBCodeProvider provider = new Microsoft.VisualBasic.VBCodeProvider())
                    {
                        System.CodeDom.Compiler.CodeGeneratorOptions opts = new System.CodeDom.Compiler.CodeGeneratorOptions();
                        using (System.CodeDom.Compiler.IndentedTextWriter tw = new System.CodeDom.Compiler.IndentedTextWriter(new StringWriter(), opts.IndentString))
                        {
                            tw.Write("Dim {0} = ", provider.CreateEscapedIdentifier(VariableName));
                            provider.GenerateCodeFromExpression(InitExpression, tw, opts);
                            Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }
    }
}
