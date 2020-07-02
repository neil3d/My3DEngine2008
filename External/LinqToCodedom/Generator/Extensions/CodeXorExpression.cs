using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeXorExpression : CodeSnippetExpression, ICustomCodeDomObject
    {
        private CodeExpression m_left;
        private CodeExpression m_right;

        public CodeXorExpression(CodeExpression left, CodeExpression right)
        {
            m_left = left;
            m_right = right;
        }

        public CodeExpression Left
        {
            get { return m_left; }
            set
            {
                if (m_left != value)
                {
                    m_left = value;
                }
            }
        }

        public CodeExpression Right
        {
            get { return m_right; }
            set
            {
                if (m_right != value)
                {
                    m_right = value;
                }
            }
        }

        #region ICustomCodeDomObject Members

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
                            tw.Write("(");
                            provider.GenerateCodeFromExpression(Left, tw, opts);
                            tw.Write(" ^ ");
                            provider.GenerateCodeFromExpression(Right, tw, opts);
                            tw.Write(")");
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
                            tw.Write("(");
                            provider.GenerateCodeFromExpression(Left, tw, opts);
                            tw.Write(" Xor ");
                            provider.GenerateCodeFromExpression(Right, tw, opts);
                            tw.Write(")");
                            Value = tw.InnerWriter.ToString();
                        }
                    }
                    break;
                default:
                    throw new NotImplementedException(language.ToString());
            }
        }

        #endregion
    }
}
