using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeLockStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeExpression m_lockExpression;
        private CodeStatement[] m_statements;

        public CodeLockStatement()
        {

        }

        public CodeLockStatement(CodeExpression lockExpression, params CodeStatement[] statements)
        {
            m_lockExpression = lockExpression;
            m_statements = statements;
        }


        public CodeExpression LockExpression
        {
            get { return m_lockExpression; }
            set
            {
                m_lockExpression = value;
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

                            tw.Write("lock (");
                            if (LockExpression != null)
                                provider.GenerateCodeFromExpression(LockExpression, tw, opts);
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
                            tw.Write("SyncLock ");
                            if (LockExpression != null)
                                provider.GenerateCodeFromExpression(LockExpression, tw, opts);
                            tw.WriteLine();
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.WriteLine("End SyncLock");
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
