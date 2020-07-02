using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.CodeDom;
using LinqToCodedom.CustomCodeDomGeneration;

namespace LinqToCodedom.CodeDomPatterns
{
    public class CodeForeachStatement : CodeSnippetStatement, ICustomCodeDomObject
    {
        private CodeTypeReference m_iterationItemType;
        private string m_iterationItemName;
        private CodeExpression m_iterExpression;
        private CodeStatement[] m_statements;

        public CodeForeachStatement()
        {

        }

        public CodeForeachStatement(CodeTypeReference iterationItemType, string iterationItemName,
            CodeExpression iterExpression, params CodeStatement[] statements)
        {
            m_iterationItemType = iterationItemType;
            m_iterationItemName = iterationItemName;
            m_iterExpression = iterExpression;
            m_statements = statements;
        }


        public CodeTypeReference IterationItemType
        {
            get { return m_iterationItemType; }
            set
            {
                m_iterationItemType = value;
            }
        }

        public string IterationItemName
        {
            get { return m_iterationItemName; }
            set
            {
                m_iterationItemName = value;
            }
        }

        public CodeExpression IterExpression
        {
            get { return m_iterExpression; }
            set
            {
                m_iterExpression = value;
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

                            tw.Write("foreach (");
                            provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(IterationItemType), tw, opts);
                            tw.Write(" {0} in ", provider.CreateValidIdentifier(IterationItemName));
                            provider.GenerateCodeFromExpression(IterExpression, tw, opts);
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
                            tw.Write("For Each {0} As ", provider.CreateValidIdentifier(IterationItemName));
                            provider.GenerateCodeFromExpression(new CodeTypeReferenceExpression(IterationItemType), tw, opts);
                            tw.Write(" in ");
                            provider.GenerateCodeFromExpression(IterExpression, tw, opts);
                            tw.WriteLine();
                            tw.Indent++;
                            if (Statements != null)
                                foreach (CodeStatement statement in Statements)
                                {
                                    provider.GenerateCodeFromStatement(statement, tw, opts);
                                }
                            tw.Indent--;
                            tw.WriteLine("Next");
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
