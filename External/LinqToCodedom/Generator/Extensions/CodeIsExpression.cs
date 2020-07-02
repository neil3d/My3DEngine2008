using System;
using System.Collections.Generic;
using System.Text;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;
using System.IO;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeIsExpression : CodeSnippetExpression, ICustomCodeDomObject
    {
        private CodeTypeReference m_typeReference;
        private CodeExpression m_expression;

        public CodeIsExpression(CodeTypeReference type, CodeExpression expression)
        {
            m_typeReference = type;
            m_expression = expression;
        }

        public CodeTypeReference TypeReference
        {
            get { return m_typeReference; }
            set
            {
                if (m_typeReference != value)
                {
                    m_typeReference = value;
                }
            }
        }

        public CodeExpression Expression
        {
            get { return m_expression; }
            set
            {
                if (m_expression != value)
                {
                    m_expression = value;
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
                            provider.GenerateCodeFromExpression(Expression, tw, opts);
                            tw.Write(" is ");
                            provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(TypeReference), tw, opts);
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
                            tw.Write("( TypeOf ");
                            provider.GenerateCodeFromExpression(Expression, tw, opts);
                            tw.Write(" Is ");
                            provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(TypeReference), tw, opts);
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
