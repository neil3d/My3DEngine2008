using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeDoStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeExpression m_testExpression;
        private CodeStatement[] m_statements;

        public CodeDoStatement()
        {

        }

        public CodeDoStatement(CodeExpression testExpression, params CodeStatement[] statements)
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

                            tw.Write("while (");
                            if (TestExpression != null)
                                provider.GenerateCodeFromExpression(TestExpression, tw, opts);
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
                            tw.Write("Do While ");
                            if (TestExpression != null)
                                provider.GenerateCodeFromExpression(TestExpression, tw, opts);
                            tw.WriteLine();
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.WriteLine("Loop");
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
