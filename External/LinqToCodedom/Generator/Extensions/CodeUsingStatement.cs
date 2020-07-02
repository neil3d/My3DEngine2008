using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeUsingStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeExpression m_usingExpression;
        private CodeStatement[] m_statements;

        public CodeUsingStatement()
        {

        }

        public CodeUsingStatement(CodeExpression usingExpression, params CodeStatement[] statements)
        {
            m_usingExpression = usingExpression;
            m_statements = statements;
        }

        public CodeExpression UsingExpression
        {
            get { return m_usingExpression; }
            set
            {
                m_usingExpression = value;
            }
        }

        public CodeStatement[] Statements
        {
            get
            {
            	return m_statements;
            }
            set
            {
                m_statements = value;
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

                            tw.Write("using (");
                            if (UsingExpression != null)
                                provider.GenerateCodeFromExpression(UsingExpression, tw, opts);

                            tw.WriteLine(")");
                            tw.WriteLine("{");
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.WriteLine("}");
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
                            tw.Write("Using ");
                            if (UsingExpression != null)
                                provider.GenerateCodeFromExpression(UsingExpression, tw, opts);

                            tw.WriteLine();
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.WriteLine("End Using");
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
