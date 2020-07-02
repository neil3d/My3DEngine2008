using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeWhileStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeExpression m_testExpression;
        private CodeStatement[] m_statements;

        public CodeWhileStatement()
        {

        }

        public CodeWhileStatement(CodeExpression testExpression, params CodeStatement[] statements)
        {
            m_testExpression = testExpression;
            m_statements = statements;
        }


        public CodeExpression TestExpression
        {
            get { return m_testExpression; }
            set
            {
                m_testExpression = value;
            }
        }

        public CodeStatement[] Statements
        {
            get { return m_statements; }
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

                            tw.WriteLine("do");
                            tw.WriteLine("{");
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.Write("} while (");
                            if (TestExpression != null)
                                provider.GenerateCodeFromExpression(TestExpression, tw, opts);
                            tw.WriteLine(");");

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
                            tw.WriteLine("Do");
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.Write("Loop Until ");
                            if (TestExpression != null)
                                provider.GenerateCodeFromExpression(TestExpression, tw, opts);
                            tw.WriteLine();

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
