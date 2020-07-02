using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeSwitchStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeExpression m_switchExpression;
        private List<CaseBlock> m_cases = new List<CaseBlock>();
 
        public class CaseBlock
        {
            private CodeExpression m_caseExpression;

            public CodeExpression CaseExpression
            {
                get { return m_caseExpression; }
                set { m_caseExpression = value; }
            }
            private CodeStatement[] m_statements;

            public CodeStatement[] Statements
            {
                get { return m_statements; }
                set { m_statements = value; }
            }
        }

        public CodeSwitchStatement()
        {

        }

        public CodeSwitchStatement(CodeExpression switchExpression)
        {
            m_switchExpression = switchExpression;
        }
        
        public CodeExpression SwitchExpression
        {
            get { return m_switchExpression; }
            set
            {
                m_switchExpression = value;
            }
        }

        public CaseBlock[] Cases
        {
            get { return m_cases.ToArray(); }
        }

        public CodeSwitchStatement Case(object caseValue, params CodeStatement[] stmt)
        {
            m_cases.Add(new CaseBlock() { CaseExpression = new CodePrimitiveExpression(caseValue), Statements = stmt});
            return this;
        }

        public CodeSwitchStatement CaseElse(params CodeStatement[] stmt)
        {
            m_cases.Add(new CaseBlock() { Statements = stmt });
            return this;
        }

        //public CodeSwitchStatement Break()
        //{
        //    m_cases.Add(new CaseBlock());
        //    return this;
        //}

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

                            tw.Write("switch (");
                            if (SwitchExpression != null)
                                provider.GenerateCodeFromExpression(SwitchExpression, tw, opts);
                            tw.WriteLine(")");
                            tw.WriteLine("{");
                            tw.Indent++;
                            foreach (CaseBlock c in m_cases)
                            {
                                if (c.Statements != null)
                                {
                                    if (c.CaseExpression != null)
                                    {
                                        tw.Write("case ");
                                        provider.GenerateCodeFromExpression(c.CaseExpression, tw, opts);
                                        tw.WriteLine(":");
                                    }
                                    else
                                    {
                                        tw.WriteLine("default:");
                                    }

                                    tw.Indent++;
                                    foreach (CodeStatement stmt in c.Statements)
                                    {
                                        provider.GenerateCodeFromStatement(stmt, tw, opts);
                                    }
                                    tw.Indent--;
                                }
                                //else
                                //{
                                //    tw.Indent++;
                                //    tw.WriteLine("break;");
                                //    tw.Indent--;
                                //}
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
                            tw.Write("Select Case ");
                            if (SwitchExpression != null)
                                provider.GenerateCodeFromExpression(SwitchExpression, tw, opts);
                            tw.WriteLine();
                            tw.Indent++;
                            foreach (CaseBlock c in m_cases)
                            {
                                if (c.Statements != null)
                                {
                                    if (c.CaseExpression != null)
                                    {
                                        tw.Write("Case ");
                                        provider.GenerateCodeFromExpression(c.CaseExpression, tw, opts);
                                        tw.WriteLine();
                                    }
                                    else
                                    {
                                        tw.WriteLine("Case Else");
                                    }

                                    tw.Indent++;
                                    for (int i = 0; i < c.Statements.Length; i++)
                                    {
                                        CodeStatement stmt = c.Statements[i];
                                        if (!(stmt is CodeExitSelectStatement) || i < c.Statements.Length-1)
                                            provider.GenerateCodeFromStatement(stmt, tw, opts);
                                    }
                                    tw.Indent--;
                                }
                                //else if (i < m_cases.Count - 1)
                                //{
                                //    tw.Indent++;
                                //    tw.WriteLine("Exit Select");
                                //    tw.Indent--;
                                //}
                            }
                            tw.Indent--;
                            tw.WriteLine("End Select");
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
